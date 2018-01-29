/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <common/Producer.h>
#include <cstring>
#include <dlfcn.h>
#include <librdkafka/rdkafkacpp.h>
#include <test/TestBase.h>

int fail = -1; // Dont fail

typedef RdKafka::Conf *(*pcreate)(RdKafka::Conf::ConfType);

RdKafka::Conf *RdKafka::Conf::create(ConfType type) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pcreate real_create = (pcreate)dlsym(
      RTLD_NEXT, "_ZN7RdKafka4Conf6createENS0_8ConfTypeE"); // nm -C
#pragma GCC diagnostic pop
  if (fail != -1 && type == fail) {
    printf("Forcing RdKafka::Conf::create() to fail\n");
    return nullptr;
  } else {
    return real_create(type);
  }
}

typedef RdKafka::Producer *(*pcreateprod)(RdKafka::Conf *, std::string &);
RdKafka::Producer *RdKafka::Producer::create(RdKafka::Conf *type,
                                             std::string &str) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pcreateprod real_create = (pcreateprod)dlsym(
      RTLD_NEXT,
      "_ZN7RdKafka8Producer6createEPNS_4ConfERSs"); // nm -C librdkafka.a

  printf("real_create OLD ABI: %p\n", (void*)real_create);

  if (real_create == NULL) {
    dlerror();
    real_create = (pcreateprod)dlsym(
        RTLD_NEXT,
      "_ZN7RdKafka8Producer6createEPNS_4ConfERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE");
    }

  if (real_create == NULL) {
    printf("Error stubbing kafka functions\n");
    exit(1);
  }
#pragma GCC diagnostic pop
  if (fail == 777) {
    printf("Forcing RdKafka::Producer::create() to fail\n");
    return nullptr;
  } else {
    return real_create(type, str);
  }
}

typedef RdKafka::Topic *(*pcreatetopic)(RdKafka::Handle *, std::string const &,
                                        RdKafka::Conf *, std::string &);
RdKafka::Topic *RdKafka::Topic::create(RdKafka::Handle *handle,
                                       std::string const &topic,
                                       RdKafka::Conf *conf, std::string &str) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pcreatetopic real_create = (pcreatetopic)dlsym(
      RTLD_NEXT,
      "_ZN7RdKafka5Topic6createEPNS_6HandleERKSsPNS_4ConfERSs"); // nm -C

  if (real_create == NULL) {                                                                // librdkafka.a
    real_create = (pcreatetopic)dlsym(
      RTLD_NEXT,
      "_ZN7RdKafka5Topic6createEPNS_6HandleERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPNS_4ConfERS8_");
  }

#pragma GCC diagnostic pop
  if (fail == 888) {
    printf("Forcing RdKafka::Topic::create() to fail\n");
    return nullptr;
  } else {
    return real_create(handle, topic, conf, str);
  }
}

//
// Google test code below
//

class ProducerTest : public TestBase {
  virtual void SetUp() { fail = -1; }
  virtual void TearDown() {}
};

TEST_F(ProducerTest, ConstructorOK) {
  Producer prod{"nobroker", "notopic"};
  int ret = prod.produce(0, 0);
  ASSERT_EQ(ret, RdKafka::ERR_NO_ERROR);
}

TEST_F(ProducerTest, CreateConfGlobalFail) {
  fail = RdKafka::Conf::CONF_GLOBAL;
  Producer prod{"nobroker", "notopic"};
  int ret = prod.produce(0, 0);
  ASSERT_EQ(ret, RdKafka::ERR_UNKNOWN);
}

TEST_F(ProducerTest, CreateConfTopicFail) {
  fail = RdKafka::Conf::CONF_TOPIC;
  Producer prod{"nobroker", "notopic"};
  int ret = prod.produce(0, 0);
  ASSERT_EQ(ret, RdKafka::ERR_UNKNOWN);
}

TEST_F(ProducerTest, CreateProducerFail) {
  fail = 777;
  Producer prod{"nobroker", "notopic"};
  int ret = prod.produce(0, 0);
  ASSERT_EQ(ret, RdKafka::ERR_UNKNOWN);
}

TEST_F(ProducerTest, CreateTopicFail) {
  fail = 888;
  Producer prod{"nobroker", "notopic"};
  int ret = prod.produce(0, 0);
  ASSERT_EQ(ret, RdKafka::ERR_UNKNOWN);
}

TEST_F(ProducerTest, ProducerFail) {
  Producer prod{"nobroker", "notopic"};
  int ret = prod.produce((char *)10000, 100000000);
  ASSERT_EQ(ret, RdKafka::ERR_MSG_SIZE_TOO_LARGE);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
