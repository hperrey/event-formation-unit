/* Copyright (C) 2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#include <vector>

// clang-format off
std::vector<uint8_t> header_only {
  0x00, 0x33, 0x71, 0x37, 0x56, 0x4d, 0x32, 0x00, 0x4c, 0x39, 0x2f, 0x60 // hdr
};

std::vector<uint8_t> data_3_ch0 {
  0x00, 0x33, 0x71, 0x37,             // Frame Counter
  0x56, 0x4d, 0x33, 0x10,             // Data ID 564d33 and fecId 1
  0x4c, 0x39, 0x2f, 0x60,             // UDP timestamp
  0x00, 0x00, 0x00, 0x00,             // Offset overflow last frame
  0xe0, 0x92, 0x24, 0x02, 0x80, 0x00, // hit 1
  0xe0, 0x92, 0x34, 0x01, 0x80, 0x00, // hit 2
  0xe0, 0x92, 0x20, 0x22, 0x80, 0x00, // hit 3
};

std::vector<uint8_t> marker_3_vmm1_3 {
  0x00, 0x33, 0x71, 0x37,             // Frame Counter
  0x56, 0x4d, 0x33, 0x10,             // Data ID 564d33 and fecId 1
  0x4c, 0x39, 0x2f, 0x60,             // UDP timestamp
  0x00, 0x00, 0x00, 0x00,             // Offset overflow last frame
  0x00, 0x00, 0x00, 0x00, 0x04, 0x01, // marker 1: vmm1, timeStamp 1
  0x00, 0x00, 0x00, 0x00, 0x08, 0x02, // marker 2: vmm2, timeStamp 2
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, // marker 3: vmm3, timeStamp 3
};

std::vector<uint8_t> marker_3_data_3 {
  0x00, 0x33, 0x71, 0x37,             // Frame Counter
  0x56, 0x4d, 0x33, 0x10,             // Data ID 564d33 and fecId 1
  0x4c, 0x39, 0x2f, 0x60,             // UDP timestamp
  0x00, 0x00, 0x00, 0x00,             // Offset overflow last frame
  0x00, 0x00, 0x00, 0x00, 0x04, 0x01, // marker 1: vmm1, timeStamp 1
  0x00, 0x00, 0x00, 0x00, 0x08, 0x02, // marker 2: vmm2, timeStamp 2
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, // marker 3: vmm3, timeStamp 3
  0xe0, 0x92, 0x24, 0x02, 0x80, 0x00, // hit 1
  0xe0, 0x92, 0x34, 0x01, 0x80, 0x00, // hit 2
  0xe0, 0x92, 0x20, 0x22, 0x80, 0x00, // hit 3
};

std::vector<uint8_t> marker_data_mixed_3 {
  0x00, 0x33, 0x71, 0x37,             // Frame Counter
  0x56, 0x4d, 0x33, 0x10,             // Data ID 564d33 and fecId 1
  0x4c, 0x39, 0x2f, 0x60,             // UDP timestamp
  0x00, 0x00, 0x00, 0x00,             // Offset overflow last frame
  0xe0, 0x92, 0x34, 0x01, 0x80, 0x00, // hit 1
  0x00, 0x00, 0x00, 0x00, 0x08, 0x02, // marker 1: vmm2, timeStamp 2
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, // marker 2: vmm3, timeStamp 3
  0xe0, 0x92, 0x24, 0x02, 0x80, 0x00, // hit 2
  0x00, 0x00, 0x00, 0x00, 0x04, 0x01, // marker 3: vmm1, timeStamp 1
  0xe0, 0x92, 0x20, 0x22, 0x80, 0x00, // hit 3
};


std::vector<uint8_t> no_data {
  //                        **    **    **    **
  0x00, 0x33, 0x71, 0x37, 0x56, 0x41, 0x32, 0x00, 0x4c, 0x39, 0x2f, 0x60, // hdr
  0x00, 0x00, 0x00, 0x00, 0x04, 0x01, // marker 1: vmm1, timeStamp 1
  0x00, 0x00, 0x00, 0x00, 0x08, 0x02, // marker 2: vmm2, timeStamp 2
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, // marker 3: vmm3, timeStamp 3
};

std::vector<uint8_t> invalid_dataid {
  0x00, 0x33, 0x71, 0x37,
//  **    **    **    **
  0xaa, 0xbb, 0xcc, 0xdd,
  0x4c, 0x39, 0x2f, 0x60,             // Udp timestamp
  0x00, 0x00, 0x00, 0x00,             // Offset overflow last frame
  0x00, 0x00, 0x00, 0x00, 0x04, 0x01, // marker 1: vmm1, timeStamp 1
  0x00, 0x00, 0x00, 0x00, 0x08, 0x02, // marker 2: vmm2, timeStamp 2
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, // marker 3: vmm3, timeStamp 3
};

std::vector<uint8_t> inconsistent_datalen {
  0x00, 0x33, 0x71, 0x37,             // Frame Counter
  0x56, 0x4d, 0x33, 0x10,             // Data ID 564d33 and fecId 1
  0x4c, 0x39, 0x2f, 0x60,             // UDP timestamp
  0x00, 0x00, 0x00, 0x00,             // Offset overflow last frame

  0xe0, 0x92, 0x34, 0x01, 0x80, 0x00, // hit 1
  0x00, 0x00, 0x00, 0x00, 0x08, 0x02, // marker 1: vmm2, timeStamp 2
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, // marker 2: vmm3, timeStamp 3
  0xe0, 0x92, 0x24, 0x02, 0x80, 0x00, // hit 2
  0x00, 0x00, 0x00, 0x00, 0x04, 0x01, // marker 3: vmm1, timeStamp 1
  0xe0, 0x92, 0x20, 0x22, // hit 3 - truncated
};

// clang-format on
