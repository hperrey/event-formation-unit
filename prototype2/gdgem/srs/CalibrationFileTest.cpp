/** Copyright (C) 2018 European Spallation Source ERIC */

#include <gdgem/srs/CalibrationFile.h>
#include <gdgem/srs/CalibrationFileTestData.h>
#include <prototype2/common/DataSave.h>
#include <test/TestBase.h>
#include <vector>

using namespace Gem;

class CalibrationFileTest : public TestBase {
protected:
  // virtual void SetUp() {  }
  // virtual void TearDown() {  }
};

/** Test cases below */
TEST_F(CalibrationFileTest, Constructor) {
  CalibrationFile cf;
  for (auto &types: CalibrationFile::CalibrationTypes) {
    for (size_t fec = 0; fec < CalibrationFile::MAX_FEC; fec++) {
      for (size_t vmm = 0; vmm < CalibrationFile::MAX_VMM; vmm++) {
        for (size_t ch = 0; ch < CalibrationFile::MAX_CH; ch++) {
          EXPECT_FLOAT_EQ(cf.getCalibration(types.first, fec, vmm, ch).slope, 1.0);
          EXPECT_FLOAT_EQ(cf.getCalibration(types.first, fec, vmm, ch).offset, 0.0);
        }
      }
    }
  }
}

TEST_F(CalibrationFileTest, AddCalibration) {
  CalibrationFile cf;
  int i = 0;
  for (auto &types: CalibrationFile::CalibrationTypes) {
    for (size_t fec = 0; fec < CalibrationFile::MAX_FEC; fec++) {
      for (size_t vmm = 0; vmm < CalibrationFile::MAX_VMM; vmm++) {
        for (size_t ch = 0; ch < CalibrationFile::MAX_CH; ch++) {
          cf.addCalibration(types.first, fec, vmm, ch, 3.14159 + i, 2.71828 - i);
          i++;
        }
      }
    }
  }

  i = 0;
  for (auto &types: CalibrationFile::CalibrationTypes) {
    for (size_t fec = 0; fec < CalibrationFile::MAX_FEC; fec++) {
      for (size_t vmm = 0; vmm < CalibrationFile::MAX_VMM; vmm++) {
        for (size_t ch = 0; ch < CalibrationFile::MAX_CH; ch++) {
          auto calib = cf.getCalibration(types.first, fec, vmm, ch);
          EXPECT_FLOAT_EQ(calib.offset, 3.14159 + i);
          EXPECT_FLOAT_EQ(calib.slope, 2.71828 - i);
          i++;
        }
      }
    }
  }
}

TEST_F(CalibrationFileTest, LoadSlowControlJsonString) {
  CalibrationFile cf;
  cf.loadCalibration(TestData_SlowControlJsonString);
  std::string type = "vmm_time_calibration";
  auto cal = cf.getCalibration(type, 3, 0, 0);
  EXPECT_FLOAT_EQ(cal.offset, 0.0);
  EXPECT_FLOAT_EQ(cal.slope, 1.0);

  type = "vmm_adc_calibration";
  cal = cf.getCalibration(type, 3, 1, 63);
  EXPECT_FLOAT_EQ(cal.offset, 0.0);
  EXPECT_FLOAT_EQ(cal.slope, 1.0);
}

TEST_F(CalibrationFileTest, LoadCalibrationInvalidJsonFile) {
  CalibrationFile cf;
  EXPECT_THROW(cf.loadCalibration(TestData_InvalidJson), std::runtime_error);
}

TEST_F(CalibrationFileTest, LoadCalibrationInvalidOffsetField) {
  CalibrationFile cf;
  EXPECT_THROW(cf.loadCalibration(TestData_InvalidJson), std::runtime_error);
}

TEST_F(CalibrationFileTest, LoadCalibration) {
  CalibrationFile cf;
  std::string type = "vmm_adc_calibration";
  cf.loadCalibration(TestData_DummyCal);
  auto cal = cf.getCalibration(type,1, 0, 0);
  EXPECT_FLOAT_EQ(cal.offset, 10.0);
  EXPECT_FLOAT_EQ(cal.slope, 1010.0);

  cal = cf.getCalibration(type,1, 0, 63);
  EXPECT_FLOAT_EQ(cal.offset, 10.7);
  EXPECT_FLOAT_EQ(cal.slope, 1010.7);

  cal = cf.getCalibration(type,1, 15, 0);
  EXPECT_FLOAT_EQ(cal.offset, 2.0);
  EXPECT_FLOAT_EQ(cal.slope, 3.0);

  cal = cf.getCalibration(type,1, 15, 63);
  EXPECT_FLOAT_EQ(cal.offset, 2.7);
  EXPECT_FLOAT_EQ(cal.slope, 3.7);
}

TEST_F(CalibrationFileTest, LoadCalibrationSizeMismatch) {
  CalibrationFile cf;
  EXPECT_THROW(cf.loadCalibration(TestData_ErrSizeMismatch), std::runtime_error);
}

// Saves a json string (DummyCal) as a valid calibration file. Then loads it
// and verifies that the values have been applied correctly
TEST_F(CalibrationFileTest, LoadCalibrationFile) {
 std::string filename = "deleteme.json";
 DataSave tempfile(filename, (void *)TestData_DummyCal.c_str(), TestData_DummyCal.size());
 CalibrationFile cf(filename);
 std::string type = "vmm_adc_calibration";
 auto cal = cf.getCalibration(type, 1, 0, 0);
 EXPECT_FLOAT_EQ(cal.offset, 10.0);
 EXPECT_FLOAT_EQ(cal.slope, 1010.0);

 cal = cf.getCalibration(type, 1, 0, 63);
 EXPECT_FLOAT_EQ(cal.offset, 10.7);
 EXPECT_FLOAT_EQ(cal.slope, 1010.7);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
