/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <common/EV42Serializer.h>
#include <common/Producer.h>
#include <cstring>
#include <test/TestBase.h>
#include "ev42_events_generated.h"

//#define ARRAYLENGTH 125000
#define ARRAYLENGTH 10

struct MockProducer {

  template<typename T>
  inline void produce(const Buffer<T>&)
  {
    NumberOfCalls++;
  }


  size_t NumberOfCalls {0};
};

class EV42SerializerTest : public TestBase {
  virtual void SetUp() {
    for (int i = 0; i < 200000; i++) {
      time[i] = i;
      pixel[i] = 200000 - i;
    }
  }

  virtual void TearDown() {}

protected:
  char flatbuffer[1024 * 1024];
  uint32_t time[200000];
  uint32_t pixel[200000];
  EV42Serializer fb{ARRAYLENGTH, "nameless"};
};

TEST_F(EV42SerializerTest, Serialize) {
  for (size_t i=0; i < ARRAYLENGTH; i++)
    fb.addEvent(i,i);
  auto buffer = fb.serialize();
  EXPECT_GE(buffer.size, ARRAYLENGTH * 8);
  EXPECT_LE(buffer.size, ARRAYLENGTH * 8 + 2048);
  ASSERT_TRUE(buffer);

  EXPECT_EQ(std::string(reinterpret_cast<const char*>(&buffer[4]), 4), "ev42");
}

TEST_F(EV42SerializerTest, SerDeserialize) {
  for (size_t i=0; i < ARRAYLENGTH-1; i++)
    fb.addEvent(i,i);
  auto buffer = fb.serialize();

  memset(flatbuffer, 0, sizeof(flatbuffer));

  auto events = GetEventMessage(flatbuffer);

  ASSERT_NE(events->message_id(), 1);

  memcpy(flatbuffer, buffer.address, buffer.size);
  EXPECT_EQ(std::string(&flatbuffer[4], 4), "ev42");
  events = GetEventMessage(flatbuffer);
  EXPECT_EQ(events->source_name()->str(), "nameless");
  ASSERT_EQ(events->message_id(), 1);
}

TEST_F(EV42SerializerTest, SerPulseTime) {
  fb.pulseTime(12345);
  ASSERT_EQ(fb.pulseTime(), 12345);
  auto buffer = fb.serialize();

  memset(flatbuffer, 0, sizeof(flatbuffer));

  auto events = GetEventMessage(flatbuffer);
  ASSERT_NE(events->pulse_time(), 12345);

  memcpy(flatbuffer, buffer.address, buffer.size);
  EXPECT_EQ(std::string(&flatbuffer[4], 4), "ev42");
  events = GetEventMessage(flatbuffer);
  EXPECT_EQ(events->source_name()->str(), "nameless");
  ASSERT_EQ(events->pulse_time(), 12345);
}

TEST_F(EV42SerializerTest, DeserializeCheckData) {
  for (int i = 0; i < ARRAYLENGTH - 1; i++) {
    auto len = fb.addEvent(time[i], pixel[i]);
    ASSERT_EQ(len, 0);
    ASSERT_EQ(fb.eventCount(), i+1);
  }

  auto buffer = fb.serialize();
  ASSERT_TRUE(buffer);

  memcpy(flatbuffer, buffer.address, buffer.size);
  EXPECT_EQ(std::string(&flatbuffer[4], 4), "ev42");

  auto veri = flatbuffers::Verifier((uint8_t *)flatbuffer, buffer.size);
  ASSERT_TRUE(VerifyEventMessageBuffer(veri));
  auto events = GetEventMessage(flatbuffer);
  EXPECT_EQ(events->source_name()->str(), "nameless");

  auto detvec = events->detector_id();
  EXPECT_EQ(detvec->size(), ARRAYLENGTH - 1);

  auto timevec = events->time_of_flight();
  EXPECT_EQ(timevec->size(), ARRAYLENGTH - 1);

  for (int i = 0; i < ARRAYLENGTH - 1; i++) {
    EXPECT_EQ((*timevec)[i], i);
    EXPECT_EQ((*detvec)[i], 200000 - i);
  }

  EXPECT_EQ(events->source_name()->str(), "nameless");
}

TEST_F(EV42SerializerTest, AutoDeserialize) {
  MockProducer mp;
  fb.setProducerCallback(std::bind(&MockProducer::produce<uint8_t>, &mp, std::placeholders::_1));

  for (int i = 0; i < ARRAYLENGTH - 1; i++) {
    auto len = fb.addEvent(time[i], pixel[i]);
    ASSERT_EQ(len, 0);
    ASSERT_EQ(fb.eventCount(), i + 1);
  }
  EXPECT_EQ(mp.NumberOfCalls, 0);

  auto len = fb.addEvent(time[ARRAYLENGTH - 1], pixel[ARRAYLENGTH - 1]);
  EXPECT_GT(len, 0);
  EXPECT_EQ(mp.NumberOfCalls, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
