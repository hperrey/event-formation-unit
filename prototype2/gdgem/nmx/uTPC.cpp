/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <gdgem/nmx/uTPC.h>
#include <cmath>
#include <set>
#include <sstream>
#include <algorithm>

#include <common/Trace.h>
//#undef TRC_LEVEL
//#define TRC_LEVEL TRC_L_DEB

#include <common/Log.h>
#undef TRC_MASK
#define TRC_MASK 0

namespace Gem {

uint32_t utpcResultsPlane::utpc_center_rounded() const {
  return static_cast<uint32_t>(std::round(utpc_center));
}

std::string utpcResultsPlane::debug() const {
  return fmt::format("{}(lu={},uu={})", utpc_center, uncert_lower, uncert_upper);
}

std::string utpcResults::debug() const {
  return fmt::format("x={}, y={}, t={}", x.debug(), y.debug(), time);
}


utpcAnalyzer::utpcAnalyzer(bool weighted, uint16_t max_timebins, uint16_t max_timedif)
: weighted_(weighted)
, max_timebins_(max_timebins)
, max_timedif_(max_timedif)
{}

utpcResultsPlane utpcAnalyzer::analyze(Cluster& cluster) const {
  utpcResultsPlane ret;

  if (cluster.hits.empty()) {
    return ret;
  }

  std::sort(cluster.hits.begin(), cluster.hits.end(),
      [](const Hit &c1, const Hit &c2) {
    return c1.time < c2.time;
  });

  double center_sum{0};
  double center_count{0};
  int16_t lspan_min = std::numeric_limits<int16_t>::max();
  int16_t lspan_max = std::numeric_limits<int16_t>::min();
  int16_t uspan_min = std::numeric_limits<int16_t>::max();
  int16_t uspan_max = std::numeric_limits<int16_t>::min();
  uint64_t earliest = std::min(cluster.time_start(), cluster.time_end()
  - static_cast<uint64_t>(max_timedif_));
  std::set<uint64_t> timebins;
  for (auto it = cluster.hits.rbegin(); it != cluster.hits.rend(); ++it) {
    auto e = *it;
    if (e.time == cluster.time_end()) {
      if (weighted_) {
        center_sum += (e.coordinate * e.weight);
        center_count += e.weight;
      } else {
        center_sum += e.coordinate;
        center_count++;
      }
      lspan_min = std::min(lspan_min, static_cast<int16_t>(e.coordinate));
      lspan_max = std::max(lspan_max, static_cast<int16_t>(e.coordinate));
    }
    if ((e.time >= earliest) && ((max_timebins_ > timebins.size()) || (timebins.count(e.time)))) {
      timebins.insert(e.time);
      uspan_min = std::min(uspan_min, static_cast<int16_t>(e.coordinate));
      uspan_max = std::max(uspan_max, static_cast<int16_t>(e.coordinate));
    } else {
      break;
    }
  }

  LOG(PROCESS, Sev::Debug, "uTPC center_sum={} center_count={}",
      center_sum,
      center_count);

  ret.utpc_center = center_sum / center_count;
  ret.uncert_lower = lspan_max - lspan_min + int16_t(1);
  ret.uncert_upper = uspan_max - uspan_min + int16_t(1);
  return ret;
}

utpcResults utpcAnalyzer::analyze(Event& event) const {
  utpcResults ret;
  ret.x = analyze(event.c1);
  ret.y = analyze(event.c2);
  ret.good = std::isfinite(ret.x.utpc_center) && std::isfinite(ret.y.utpc_center);
  ret.time = utpc_time(event);
  return ret;
}

uint64_t utpcAnalyzer::utpc_time(const Event& e) {
  // \todo is this what we want?
  return std::max(e.c1.time_end(), e.c2.time_end());
}

bool utpcAnalyzer::meets_lower_criterion(const utpcResultsPlane& x, const utpcResultsPlane& y,
                                  int16_t max_lu) {
  return (x.uncert_lower < max_lu) && (y.uncert_lower < max_lu);
}



}
