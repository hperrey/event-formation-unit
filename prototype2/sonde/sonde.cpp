/* Copyright (C) 2017-2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#include <cinttypes>
#include <common/Detector.h>
#include <common/EFUArgs.h>
#include <common/FBSerializer.h>
#include <common/Producer.h>
#include <common/RingBuffer.h>
#include <common/Trace.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <libs/include/SPSCFifo.h>
#include <libs/include/Socket.h>
#include <libs/include/TSCTimer.h>
#include <libs/include/Timer.h>
#include <memory>
#include <sonde/ideas/Data.h>
#include <stdio.h>
#include <unistd.h>

//#undef TRC_LEVEL
//#define TRC_LEVEL TRC_L_DEB

using namespace memory_sequential_consistent; // Lock free fifo

const char *classname = "SoNDe detector using IDEA readout";

const int TSC_MHZ = 2900; // MJC's workstation - not reliable

/** ----------------------------------------------------- */

class SONDEIDEA : public Detector {
public:
  SONDEIDEA(BaseSettings settings);
  ~SONDEIDEA() { printf("sonde destructor called\n"); }

  void input_thread();
  void processing_thread();

  const char *detectorname();

  /** \todo figure out the right size  of the .._max_entries  */
  static const int eth_buffer_max_entries = 20000;
  static const int eth_buffer_size = 9000;
  static const int kafka_buffer_size = 124000; /**< events */

private:
  /** Shared between input_thread and processing_thread*/
  CircularFifo<unsigned int, eth_buffer_max_entries> input2proc_fifo;
  RingBuffer<eth_buffer_size> *eth_ringbuf;

  NewStats ns{"efu.sonde."}; //

  struct {
    // Input Counters
    int64_t rx_packets;
    int64_t rx_bytes;
    int64_t fifo_push_errors;
    int64_t rx_pktlen_0;
    int64_t pad[4];

    // Processing and Output counters
    int64_t rx_idle1;
    int64_t rx_events;
    int64_t rx_geometry_errors;
    int64_t tx_bytes;
    int64_t rx_seq_errors;
    int64_t fifo_synch_errors;
    // Kafka stats below are common to all detectors
    int64_t kafka_produce_fails;
    int64_t kafka_ev_errors;
    int64_t kafka_ev_others;
    int64_t kafka_dr_errors;
    int64_t kafka_dr_noerrors;
  } ALIGN(64) mystats;
};

struct DetectorSettingsStruct {
  std::string fileprefix{""};
} DetectorSettings;

void SetCLIArguments(CLI::App __attribute__((unused)) & parser) {
  parser.add_option("--dumptofile", DetectorSettings.fileprefix,
                    "dump to specified file")->group("Sonde");
}

PopulateCLIParser PopulateParser{SetCLIArguments};

SONDEIDEA::SONDEIDEA(BaseSettings settings) : Detector("SoNDe detector using IDEA readout", settings) {
  Stats.setPrefix("efu.sonde");

  XTRACE(INIT, ALW, "Adding stats");
  // clang-format off
  Stats.create("input.rx_packets",                mystats.rx_packets);
  Stats.create("input.rx_bytes",                  mystats.rx_bytes);
  Stats.create("input.dropped",                   mystats.fifo_push_errors);
  Stats.create("processing.idle",                 mystats.rx_idle1);
  Stats.create("processing.rx_events",            mystats.rx_events);
  Stats.create("processing.rx_geometry_errors",   mystats.rx_geometry_errors);
  Stats.create("processing.rx_seq_errors",        mystats.rx_seq_errors);
  Stats.create("output.tx_bytes",                 mystats.tx_bytes);
  /// \todo below stats are common to all detectors and could/should be moved
  Stats.create("kafka_produce_fails",             mystats.kafka_produce_fails);
  Stats.create("kafka_ev_errors",                 mystats.kafka_ev_errors);
  Stats.create("kafka_ev_others",                 mystats.kafka_ev_others);
  Stats.create("kafka_dr_errors",                 mystats.kafka_dr_errors);
  Stats.create("kafka_dr_others",                 mystats.kafka_dr_noerrors);
  // clang-format on
  std::function<void()> inputFunc = [this]() { SONDEIDEA::input_thread(); };
  Detector::AddThreadFunction(inputFunc, "input");

  std::function<void()> processingFunc = [this]() {
    SONDEIDEA::processing_thread();
  };
  Detector::AddThreadFunction(processingFunc, "processing");

  XTRACE(INIT, ALW, "Creating %d SONDE Rx ringbuffers of size %d",
         eth_buffer_max_entries, eth_buffer_size);
  eth_ringbuf = new RingBuffer<eth_buffer_size>(
      eth_buffer_max_entries + 11); /** \todo testing workaround */
  assert(eth_ringbuf != 0);
}

const char *SONDEIDEA::detectorname() { return classname; }

void SONDEIDEA::input_thread() {
  /** Connection setup */
  Socket::Endpoint local(EFUSettings.DetectorAddress.c_str(),
                         EFUSettings.DetectorPort);
  UDPReceiver sondedata(local);
  sondedata.setBufferSizes(0, EFUSettings.DetectorRxBufferSize);
  sondedata.printBufferSizes();
  sondedata.setRecvTimeout(0, 100000); // secs, usecs, 1/10 second

  int rdsize;
  for (;;) {
    unsigned int eth_index = eth_ringbuf->getDataIndex();

    /** this is the processing step */
    eth_ringbuf->setDataLength(eth_index, 0);
    if ((rdsize = sondedata.receive(eth_ringbuf->getDataBuffer(eth_index),
                                    eth_ringbuf->getMaxBufSize())) > 0) {
      mystats.rx_packets++;
      mystats.rx_bytes += rdsize;
      eth_ringbuf->setDataLength(eth_index, rdsize);

      if (input2proc_fifo.push(eth_index) == false) {
        mystats.fifo_push_errors++;
      } else {
        eth_ringbuf->getNextBuffer();
      }
    }

    // Checking for exit
    if (not runThreads) {
      XTRACE(INPUT, ALW, "Stopping input thread.");
      return;
    }
  }
}

void SONDEIDEA::processing_thread() {
  SoNDeGeometry geometry;

  IDEASData ideasdata(&geometry, DetectorSettings.fileprefix);
  Producer eventprod(EFUSettings.KafkaBroker, "SKADI_detector");
  FBSerializer flatbuffer(kafka_buffer_size, eventprod);

  unsigned int data_index;

  TSCTimer produce_timer;
  while (1) {
    if ((input2proc_fifo.pop(data_index)) == false) {
      mystats.rx_idle1++;
      usleep(10);

    } else {

      auto len = eth_ringbuf->getDataLength(data_index);
      if (len == 0) {
        mystats.fifo_synch_errors++;
      } else {
        int events =
            ideasdata.parse_buffer(eth_ringbuf->getDataBuffer(data_index), len);

        mystats.rx_geometry_errors += ideasdata.errors;
        mystats.rx_events += ideasdata.events;
        mystats.rx_seq_errors = ideasdata.ctr_outof_sequence;

        if (events > 0) {
          for (int i = 0; i < events; i++) {
            XTRACE(PROCESS, DEB, "flatbuffer.addevent[i: %d](t: %d, pix: %d)",
                   i, ideasdata.data[i].time, ideasdata.data[i].pixel_id);
            mystats.tx_bytes += flatbuffer.addevent(ideasdata.data[i].time,
                                                    ideasdata.data[i].pixel_id);
          }
        }

        if (produce_timer.timetsc() >=
            EFUSettings.UpdateIntervalSec * 1000000 * TSC_MHZ) {
          mystats.tx_bytes += flatbuffer.produce();

          /// Kafka stats update - common to all detectors
          /// don't increment as producer keeps absolute count
          mystats.kafka_produce_fails = eventprod.stats.produce_fails;
          mystats.kafka_ev_errors = eventprod.stats.ev_errors;
          mystats.kafka_ev_others = eventprod.stats.ev_others;
          mystats.kafka_dr_errors = eventprod.stats.dr_errors;
          mystats.kafka_dr_noerrors = eventprod.stats.dr_noerrors;
          produce_timer.now();
        }
      }
    }
    if (not runThreads) {
      XTRACE(INPUT, ALW, "Stopping input thread.");
      return;
    }
  }
}

/** ----------------------------------------------------- */

DetectorFactory<SONDEIDEA> Factory;
