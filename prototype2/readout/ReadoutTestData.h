/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

/** @file
 *
 *  @brief Dataset for running unit tests - do not edit if unsure of what they
 * do!
 */

#include <cinttypes>
#include <vector>

using namespace std;

// clang-format off

vector<uint8_t> ok_one_hit
{
// type        word cnt    seqno       reserved
   0x01, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00,
// channel     hitcount    Hit data1
   0x07, 0x00, 0x01, 0x00, 0x34, 0x12, 0x78, 0x56,
//                         Pad
   0x78, 0x56, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                         Checksum
   0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
};


// clang-format on