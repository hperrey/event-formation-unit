/* Copyright (C) 2017-2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
/// \brief Parser for JADAQ
//===----------------------------------------------------------------------===//

#pragma once

#include <caen/Readout.h>
#include <vector>

namespace Multiblade {

/// \todo consider using the real values  and using
/// htons() in the parser
// 0x0102 (1.3) byteswapped
constexpr uint16_t Version = 0x0103;
// 0x0100 byteswapped)
 constexpr uint16_t ElementTypeList422 = 0x0001;
 constexpr uint16_t ElementTypeStandard = 0x0003;

class DataParser {
public:
  enum error { OK = 0, ESIZE, EHEADER };

  struct __attribute__ ((__packed__)) Header // 32 bytes
  {
    uint64_t runID;
    uint64_t globalTime;
    uint32_t digitizerID;
    uint16_t elementType;
    uint16_t numElements;
    uint16_t version;
    uint32_t seqNum;
    uint8_t __pad[2];
  };

  struct __attribute__ ((__packed__)) ListElement422 {
    uint32_t localTime;
    uint16_t channel;
    uint16_t adcValue;
  };

  struct __attribute__ ((__packed__)) WavFrmElement751 {
    uint32_t localTime;
    uint8_t channelMask;
    uint32_t eventNo;
    uint16_t num_samples;
    uint16_t samples;
  };

  DataParser() {}

  int parse(const char * /*void **/ buffer, unsigned int size);
  void parseList422(struct ListElement422 *data);
  void parseWavFrm751(void *data);

  struct Stats {
    uint64_t error_bytes{0};
    uint64_t seq_errors{0};
    uint64_t seq_trg_errors{0};
  } Stats;

  Readout prototype;
  std::vector<Readout> readouts;

  struct Header *MBHeader{nullptr};
  struct ListElement422 *Data{nullptr};

  uint32_t PreviousSeqNum{0};
  uint32_t PreviousEvtNumWavFrm{0};
};

}
