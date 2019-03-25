/** Copyright (C) 2017-2018 European Spallation Source */
//===----------------------------------------------------------------------===//
///
/// \file
/// Class to parse detector readout for Multi-Blade using Jadaq UDP
/// Data format is described in
/// https://github.com/ess-dmsc/jadaq/blob/devel/DataFormat.hpp
///
//===----------------------------------------------------------------------===//

#include <common/Trace.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <caen/DataParser.h>
#include <memory>

#undef TRC_LEVEL
#define TRC_LEVEL TRC_L_DEB

namespace Multiblade {

  void DataParser::parseList422(struct ListElement422 *data){
    printf(" parsing list 422 at %p \n", (void*)data);
    // readouts.resize(MBHeader->numElements, prototype);
    // for (size_t i=0; i < MBHeader->numElements; ++i) {
    //   auto& r = readouts[i];
    //   const auto& d = Data[i];
    //   r.local_time = d.localTime;
    //   r.channel = d.channel;
    //   r.adc = d.adcValue;
    //   //XTRACE(DATA, DEB, "readout %s", r.debug().c_str());
    // }

    return;
  }


  // TODO: add total length of package to function to make it more robust
  // TODO: additional parsing and exposing variables to grafana?
  void DataParser::parseWavFrm751(void *data){
    for (size_t i=0; i < MBHeader->numElements; ++i) {
      struct WavFrmElement751* trg = (struct WavFrmElement751*) data;
      printf(" parsing std waveform %p (%zu of %d elements), %d samples, %x mask, %d trgt no \n ",
             (void*)trg, i, MBHeader->numElements,trg->num_samples, trg->channelMask, trg->eventNo);
      if ((trg->eventNo - PreviousEvtNumWavFrm) != 1) {
        XTRACE(DATA, WAR, "Event trigger number inconsistency: current=%lu, previous=%lu",
               trg->eventNo, PreviousEvtNumWavFrm);
        Stats.seq_trg_errors++;
        // But we continue anyways
      }
      PreviousEvtNumWavFrm = trg->eventNo;
      //uint16_t* sample = &trg->samples;
      //for (size_t s=0; s < trg->num_samples; ++s) {
        //printf("%d, ", sample[s]);
      //}
      //printf("\n");
      data = (char*)data + sizeof(struct WavFrmElement751)+ trg->num_samples * sizeof(uint16_t) - sizeof(uint16_t);
    }
    return;
  }


int DataParser::parse(const char *buffer, unsigned int size) {

  readouts.clear();
  MBHeader = nullptr;
  Data = nullptr;
  memset(&Stats, 0, sizeof(struct Stats));

  auto headerlen = sizeof(struct Header);
  if (size < headerlen) {
    XTRACE(DATA, WAR, "Invalid data size: received %d, min. expected: %lu", size, headerlen);
    Stats.error_bytes += size;
    return -error::ESIZE;
  }

  MBHeader = (struct Header *)buffer;

  if (MBHeader->version != Version) {
    XTRACE(DATA, WAR, "Unsupported data version: 0x%04x (expected 0x%04x)", MBHeader->version, Version);
    Stats.error_bytes += size;
    return -error::EHEADER;
  }

  if ((MBHeader->seqNum - PreviousSeqNum) != 1) {
    XTRACE(DATA, WAR, "Sequence number inconsistency: current=%lu, previous=%lu",
        MBHeader->seqNum, PreviousSeqNum);
    Stats.seq_errors++;
    // But we continue anyways
  }
  PreviousSeqNum = MBHeader->seqNum;

  if (MBHeader->elementType == ElementTypeList422) {
    auto expectedsize = sizeof(struct Header) + MBHeader->numElements * sizeof(struct ListElement422);

    if (size != expectedsize) {
      XTRACE(DATA, WAR, "Data length mismatch: received %d, expected %lu", size, expectedsize);
      Stats.error_bytes += size;
      return -error::ESIZE;
    }
    parseList422((struct ListElement422 *)(buffer + sizeof(struct Header)));
  } else if (MBHeader->elementType == ElementTypeStandard) {
    parseWavFrm751((struct WavFrmElement751 *)(buffer + sizeof(struct Header)));
  } else {
    XTRACE(DATA, WAR, "Unsupported data type: 0x%04x", MBHeader->elementType);
    Stats.error_bytes += size;
    return -error::EHEADER;
  }

  prototype.global_time = MBHeader->globalTime;
  prototype.digitizer = MBHeader->digitizerID;

  return MBHeader->numElements;
}

}
