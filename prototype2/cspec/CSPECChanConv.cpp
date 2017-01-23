/** Copyright (C) 2016 European Spallation Source ERIC */

#include <common/Trace.h>
#include <cspec/CSPECChanConv.h>
#include <cstring>

CSPECChanConv::CSPECChanConv() {
  static_assert(sizeof(wirecal) == adcsize * 2, "wirecal mismatch");
  static_assert(sizeof(gridcal) == adcsize * 2, "gridcal mismatch");

  for (int i = 0; i < adcsize; i++) {
    wirecal[i] = 0;
    gridcal[i] = 0;
  }
}

CSPECChanConv::CSPECChanConv(uint16_t initval) {
  static_assert(sizeof(wirecal) == adcsize * 2, "wirecal mismatch");
  static_assert(sizeof(gridcal) == adcsize * 2, "gridcal mismatch");

  for (int i = 0; i < adcsize; i++) {
    wirecal[i] = initval;
    gridcal[i] = initval;
  }
}

int CSPECChanConv::makecal(uint16_t *array, unsigned int min, unsigned int max,
                           unsigned int nb_channels) {
  if ((min >= max) || (min >= CSPECChanConv::adcsize) ||
      (max >= CSPECChanConv::adcsize))
    return -1;
  if (nb_channels > (max - min))
    return -1;

  for (unsigned int adc = 0; adc < CSPECChanConv::adcsize; adc++) {
    if (adc < min) {
      array[adc] = CSPECChanConv::adcsize - 1;
    } else if (adc > max) {
      array[adc] = CSPECChanConv::adcsize - 1;
    } else {
      array[adc] = ((adc - min) * nb_channels) / (max - min);
    }
  }
  return 0;
}

void CSPECChanConv::load_calibration(uint16_t *wirecal_new,
                                     uint16_t *gridcal_new) {
  if ((wirecal_new == NULL) || (gridcal_new == NULL)) {
    XTRACE(PROCESS, ERR, "Invalid calibration data\n");
    return;
  }
  XTRACE(PROCESS, INF, "Loading calibration data\n");
  std::memcpy(wirecal, wirecal_new, sizeof(wirecal));
  std::memcpy(gridcal, gridcal_new, sizeof(gridcal));
}