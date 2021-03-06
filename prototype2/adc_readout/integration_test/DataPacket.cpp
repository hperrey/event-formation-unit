/* Copyright (C) 2017-2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#include "DataPacket.h"
#include <cstring>

DataPacket::DataPacket(size_t MaxPacketSize)
    : Buffer(std::make_unique<std::uint8_t[]>(MaxPacketSize)),
      HeaderPtr(reinterpret_cast<PacketHeader *>(Buffer.get())),
      Size(sizeof(PacketHeader)), MaxSize(MaxPacketSize) {
  HeaderPtr->PacketType = 0x1111;
}

bool DataPacket::addSamplingRun(void *DataPtr, size_t Bytes) {
  if (Size + Bytes + 4 > MaxSize) {
    return false;
  }
  std::memcpy(Buffer.get() + Size, DataPtr, Bytes);
  Size += Bytes;
  return true;
}

std::pair<void *, size_t> DataPacket::getBuffer(std::uint16_t PacketCount,
                                                std::uint16_t ReadoutCount) {
  std::uint32_t *TrailerPtr =
      reinterpret_cast<std::uint32_t *>(Buffer.get() + Size);
  *TrailerPtr = htonl(0xFEEDF00Du);
  Size += 4;
  HeaderPtr->GlobalCount = htons(PacketCount);
  HeaderPtr->ReadoutCount = htons(ReadoutCount);
  HeaderPtr->ReadoutLength = htons(Size - 2);

  return std::make_pair(Buffer.get(), Size);
}

void DataPacket::resetPacket() { Size = sizeof(PacketHeader); }
