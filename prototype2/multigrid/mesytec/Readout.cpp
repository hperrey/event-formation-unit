/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <multigrid/mesytec/Readout.h>
#include <sstream>

namespace Multigrid {

std::string Readout::debug() const {
  // \todo use fmt
  std::stringstream ss;
  ss << " trigger_count=" << trigger_count;
  ss << " total_time=" << total_time;
  ss << " external_trigger=" << (external_trigger ? "true" : "false");

  if (!external_trigger) {
    ss << " bus=" << static_cast<uint16_t>(bus);
    ss << " channel=" << channel;
    ss << " adc=" << adc;
  }

  // \todo don't use this
  ss << " high_time=" << high_time;
  ss << " low_time=" << low_time;
  ss << " time_diff=" << time_diff;

  // \todo use this
  //ss << " module=" << static_cast<uint16_t>(module);

  return ss.str();
}

}