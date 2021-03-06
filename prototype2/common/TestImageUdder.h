/* Copyright (C) 2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
/// \brief Generate next pixelid of the Udder upon each call to getPixel()
/// Image is a bitmap and must be multiples of 8 bits in both width and height.
///
//===----------------------------------------------------------------------===//

#pragma once

#include <logical_geometry/ESSGeometry.h>
#include <vector>

class Udder {
public:
  const uint8_t ImageWidth = 184;
  const uint8_t ImageHeight = 224;
  const uint16_t ImageSize = ImageWidth * ImageHeight;
  static const std::vector<uint8_t> UdderImage;

  static unsigned int PixelNumber;
  unsigned int byte, bit, index;

  uint32_t getPixel(uint16_t w, uint16_t h, ESSGeometry * geom) {
    do {
      index = PixelNumber % ImageSize;
      byte = index / 8;
      bit = 7 - index % 8;
      assert( byte < ImageSize / 8 );
      assert( bit < 8);
      PixelNumber++;
    } while ((UdderImage[byte] & (1 << bit)) == 0x00);

    uint16_t x = 1.0 * w * (index % ImageWidth) / ImageWidth;
    uint16_t y = 1.0 * h * (index / ImageWidth) / ImageHeight;
    auto pixel = geom->pixel2D(x, y);
    //printf("p: %u, i: %u, x: %u, y: %u, pix: %u\n", PixelNumber, index, x, y, pixel);
    return pixel;
  }
};

unsigned int Udder::PixelNumber = 0;

const std::vector<uint8_t> Udder::UdderImage = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x7f, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x1f, 0xe0, 0x00, 0x00, 0x03, 0xc0, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x03, 0xf0, 0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x01, 0xf8, 0x00, 0x00, 0x03, 0xc0, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0x00, 0x03, 0xc0, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x7c, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x3e, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x1f, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x1f, 0x80, 0x00, 0x03, 0xc0, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x0f, 0x80, 0x00, 0x03, 0xc0, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x07, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x07, 0xe0, 0x00, 0x03, 0xe0, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x03, 0xe0, 0x00, 0x03, 0xe0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x01, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x01, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0xf8, 0x00, 0x01, 0xe0, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7c, 0x00, 0x01, 0xf0, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7c, 0x00, 0x00, 0xf0, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x1f, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x78, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3e, 0x0f, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x78, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0x07, 0xc0, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x07, 0x80, 0x00, 0x7c, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0x03, 0xe0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x7c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x78, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x78, 0x00, 0x00, 0x07, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x3e, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x7c, 0x00, 0x00, 0x07, 0x80, 0x00, 0x01, 0xe0, 0x00, 0x3e, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x3e, 0x00, 0x00, 0x07, 0x80, 0x00, 0x01, 0xf0, 0x00, 0x1e, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x1f, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0xf0, 0x00, 0x1f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x0f, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0xf8, 0x00, 0x1f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x07, 0x80, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x78, 0x00, 0x0f, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x07, 0xc0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x7c, 0x00, 0x0f, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x03, 0xc0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x01, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x3e, 0x00, 0x07, 0x80, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x1e, 0x00, 0x07, 0x80, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x78, 0x00, 0x00, 0x78, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x78, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00,
0x0f, 0x80, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x07, 0x80, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00,
0x3f, 0xe0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x07, 0x80, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
0x7f, 0xf8, 0x00, 0x00, 0x07, 0x80, 0x00, 0x03, 0xc0, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
0x7f, 0xfc, 0x00, 0x00, 0x03, 0x80, 0x00, 0x03, 0xe0, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00,
0xf0, 0x3e, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x01, 0xe0, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
0xf0, 0x1f, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0xf0, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00,
0xe0, 0x0f, 0x80, 0x00, 0x01, 0xc0, 0x00, 0x00, 0xf3, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00,
0xe0, 0x07, 0xc0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00,
0xe0, 0x03, 0xe0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00,
0xe0, 0x01, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00,
0xf0, 0x00, 0xf8, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00,
0xf0, 0x00, 0x78, 0x00, 0x00, 0x70, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
0xf0, 0x00, 0x3c, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00,
0xf0, 0x00, 0x3e, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00,
0xf0, 0x00, 0x1e, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00,
0xf0, 0x00, 0x0f, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3f, 0x00, 0x00,
0xf8, 0x00, 0x07, 0x80, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x1f, 0x00, 0x00,
0x78, 0x00, 0x07, 0x80, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x0f, 0x80, 0x00,
0x78, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x0f, 0x80, 0x00,
0x78, 0x00, 0x03, 0xe0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00,
0x7c, 0x00, 0x01, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00,
0x3c, 0x00, 0x00, 0xf0, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00,
0x3c, 0x00, 0x00, 0xf0, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00,
0x3e, 0x00, 0x00, 0x78, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00,
0x3e, 0x00, 0x00, 0x78, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00,
0x1e, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00,
0x1f, 0x00, 0x00, 0x3e, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00,
0x1f, 0x00, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00,
0x0f, 0x00, 0x00, 0x1f, 0x00, 0x7e, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x01, 0xe0, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00,
0x0f, 0x80, 0x00, 0x1f, 0x00, 0x7c, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x07, 0xe0, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00,
0x07, 0x80, 0x00, 0x0f, 0x80, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00,
0x07, 0x80, 0x00, 0x0f, 0x81, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xf0, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00,
0x07, 0xc0, 0x00, 0x07, 0xc3, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00,
0x03, 0xc0, 0x00, 0x07, 0xc3, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x80,
0x03, 0xe0, 0x00, 0x03, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0x80,
0x03, 0xe0, 0x00, 0x03, 0xef, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xdf, 0xf8, 0x00, 0x00, 0x00, 0x07, 0x80,
0x01, 0xe0, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xc0,
0x01, 0xf0, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xc0,
0x00, 0xf0, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x03, 0xc0,
0x00, 0xf8, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x03, 0xe0,
0x00, 0xf8, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x03, 0xe0,
0x00, 0x7c, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x01, 0xe0,
0x00, 0x7c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x01, 0xe0,
0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x01, 0xf0,
0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xf0,
0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x7f, 0x80, 0x00, 0x18, 0x03, 0xc0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xf0,
0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x1f, 0xc0, 0x00, 0x18, 0x1f, 0xf0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf8,
0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x0f, 0xe0, 0x00, 0x18, 0x3f, 0xf8, 0x00, 0x70, 0x00, 0x00, 0x00, 0xf8,
0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x03, 0xe0, 0x00, 0x18, 0x7f, 0xfe, 0x00, 0x70, 0x00, 0x00, 0x00, 0x78,
0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x03, 0xf0, 0x00, 0x18, 0x7f, 0xfe, 0x00, 0x70, 0x00, 0x00, 0x00, 0x78,
0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x01, 0xf0, 0x00, 0x18, 0xff, 0xff, 0x00, 0x30, 0x00, 0x00, 0x00, 0x38,
0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0xf8, 0x00, 0x18, 0xfe, 0x7f, 0x00, 0x38, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0xf8, 0x00, 0x19, 0xf8, 0x3f, 0x00, 0x38, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x01, 0xe0, 0x3c, 0x00, 0x19, 0xf8, 0x1f, 0x80, 0x18, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x07, 0xf8, 0x1e, 0x00, 0x19, 0xf8, 0x1f, 0x80, 0x18, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x0f, 0xfe, 0x1e, 0x00, 0x19, 0xf8, 0x1f, 0x80, 0x18, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x1f, 0xff, 0x0e, 0x00, 0x19, 0xf8, 0x1f, 0x80, 0x18, 0x00, 0x00, 0x00, 0x1c,
0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x3f, 0xff, 0x8e, 0x00, 0x18, 0xfc, 0x3f, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x7f, 0xff, 0x8f, 0x00, 0x18, 0xfe, 0x7f, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x7f, 0xff, 0xc7, 0x00, 0x18, 0xff, 0xff, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x7e, 0x1f, 0xc7, 0x00, 0x0c, 0x7f, 0xfe, 0x00, 0x30, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xfc, 0x0f, 0xe7, 0x00, 0x0c, 0x3f, 0xfc, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xfc, 0x0f, 0xe3, 0x00, 0x0c, 0x1f, 0xfd, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xfc, 0x0f, 0xe3, 0x00, 0x0f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0e,
0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xfc, 0x0f, 0xe3, 0x80, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0e,
0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xfe, 0x0f, 0xc3, 0x80, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x7f, 0x3f, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x7f, 0xff, 0xdf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x3f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x81, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x07,
0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x80, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xcf, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f,
0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0e,
0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x3f, 0xff, 0xff, 0xfb, 0xff, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0e,
0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x3f, 0xfd, 0xff, 0xf9, 0xff, 0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0e,
0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1f, 0xf9, 0xff, 0xf8, 0xff, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x0f, 0xf0, 0xff, 0xf0, 0x7e, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0xe0, 0x7f, 0xe0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e,
0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1c,
0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x1c,
0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x3c,
0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x38,
0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x38,
0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x78,
0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x78,
0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x78,
0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x70,
0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xf0,
0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0xf0,
0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0,
0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xe0,
0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x01, 0xe0,
0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x01, 0xe0,
0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x01, 0xc0,
0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x03, 0xc0,
0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x03, 0xc0,
0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x03, 0x80,
0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x07, 0x80,
0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x07, 0x80,
0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x0f, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x0f, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x1e, 0x00, 0x00, 0x0e, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x1e, 0x00, 0x00, 0x1e, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xf1, 0xff, 0x9c, 0x00, 0x00, 0x3e, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x06, 0x07, 0xfb, 0xff, 0xbc, 0x00, 0x00, 0x3c, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x1f, 0x87, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x78, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3f, 0xcf, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x78, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7f, 0xef, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xf0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x38, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xf0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x01, 0xe0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x01, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xc0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x81, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x07, 0xc0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x83, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x07, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe3, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
