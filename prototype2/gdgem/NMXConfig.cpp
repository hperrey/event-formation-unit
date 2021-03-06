/** Copyright (C) 2017 European Spallation Source ERIC */

#include <gdgem/NMXConfig.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include <common/Trace.h>
//#undef TRC_LEVEL
//#define TRC_LEVEL TRC_L_DEB

namespace Gem {

NMXConfig::NMXConfig(std::string configfile, std::string calibrationfile) {

  calfile = std::make_shared<CalibrationFile>(calibrationfile);

  nlohmann::json root;

  std::ifstream t(configfile);
  std::string jsonstring((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());

  if (!t.good()) {
    XTRACE(INIT, ERR, fmt::format("Invalid Json file: {}", configfile).c_str());
    throw std::runtime_error("NMXConfig error - requested file unavailable.");
  }

  try {
    root = nlohmann::json::parse(jsonstring);
  }
  catch (...) {
    XTRACE(INIT, WAR, "Invalid Json file: %s", configfile.c_str());
    return;
  }

  builder_type = root["builder_type"].get<std::string>();

  /**< \todo get from slow control? */
  if (builder_type == "VMM3") {
    auto tc = root["time_config"];
    time_config.tac_slope_ns(tc["tac_slope"].get<int>());
    time_config.bc_clock_MHz(tc["bc_clock"].get<int>());
    time_config.trigger_resolution_ns(tc["trigger_resolution"].get<double>());
    time_config.acquisition_window(tc["acquisition_window"].get<unsigned int>());
  }

  if (builder_type != "Hits") {
    auto sm = root["srs_mappings"];
    for (unsigned int index = 0; index < sm.size(); index++) {
      auto fecID = sm[index]["fecID"].get<int>();
      auto vmmID = sm[index]["vmmID"].get<int>();
      auto planeID = sm[index]["planeID"].get<int>();
      auto strip_offset = sm[index]["strip_offset"].get<int>();
      srs_mappings.set_mapping(fecID, vmmID, planeID, strip_offset);
    }

    adc_threshold = root["adc_threshold"].get<unsigned int>();
  }

  hit_histograms = root["hit_histograms"].get<bool>();

  perform_clustering = root["perform_clustering"].get<bool>();

  send_raw_hits = root["send_raw_hits"].get<bool>();

  if (perform_clustering) {
    auto cx = root["clusterer x"];
    clusterer_x.max_strip_gap = cx["max_strip_gap"].get<unsigned int>();
    clusterer_x.max_time_gap = cx["max_time_gap"].get<double>();

    auto cy = root["clusterer y"];
    clusterer_y.max_strip_gap = cy["max_strip_gap"].get<unsigned int>();
    clusterer_y.max_time_gap = cy["max_time_gap"].get<double>();

    matcher_max_delta_time = root["matcher_max_delta_time"].get<double>();

    analyze_weighted = root["analyze_weighted"].get<bool>();
    analyze_max_timebins = root["analyze_max_timebins"].get<unsigned int>();
    analyze_max_timedif = root["analyze_max_timedif"].get<unsigned int>();

    auto f = root["filters"];
    filter.enforce_lower_uncertainty_limit =
            f["enforce_lower_uncertainty_limit"].get<bool>();
    filter.lower_uncertainty_limit = f["lower_uncertainty_limit"].get<unsigned int>();
    filter.enforce_minimum_hits = f["enforce_minimum_hits"].get<bool>();
    filter.minimum_hits = f["minimum_hits"].get<unsigned int>();

    track_sample_minhits = root["track_sample_minhits"].get<unsigned int>();
    cluster_adc_downshift = root["cluster_adc_downshift"].get<unsigned int>();
    send_tracks = root["send_tracks"].get<bool>();

    // \todo deduce geometry from SRS mappings eventually
    //       this is better for now, while prototyping
    geometry.nx(root["geometry_x"].get<unsigned int>());
    geometry.ny(root["geometry_y"].get<unsigned int>());
    geometry.nz(1);
    geometry.np(1);
  }
}

std::string NMXConfig::debug() const {
  std::string ret;
  ret += fmt::format("{:=^50}\n", "");
  ret += fmt::format("{:=^50}\n", fmt::format("{:^12}", builder_type));
  ret += fmt::format("{:=^50}\n", "");

  if (builder_type == "VMM3") {
    if (calfile) {
      ret += "VMM Calibrations:" + calfile->debug() + "\n";
    }
  }
  if (builder_type != "Hits") {
    ret += "Time config:\n" + time_config.debug();
    ret += "Digital geometry:\n" + srs_mappings.debug();
    ret += fmt::format("adc_threshold = {}\n", adc_threshold);
  }

  ret += "\n";

  ret += fmt::format("Send raw hits = {}\n", (send_raw_hits ? "YES" : "no"));

  ret += "\n";

  ret += fmt::format("Histogram hits = {}\n", (hit_histograms ? "YES" : "no"));
  if (hit_histograms && perform_clustering) {
    ret += fmt::format("  cluster_adc_downshift = {} (bits)\n",
                       cluster_adc_downshift);
  }

  ret += "\n";

  ret += fmt::format("Perform clustering = {}\n", (perform_clustering ? "YES" : "no"));

  if (perform_clustering) {
    ret += "  Clusterer-X:\n";
    ret += fmt::format("    max_time_gap = {}\n", clusterer_x.max_time_gap);
    ret += fmt::format("    max_strip_gap = {}\n", clusterer_x.max_strip_gap);

    ret += "  Clusterer-Y:\n";
    ret += fmt::format("    max_time_gap = {}\n", clusterer_y.max_time_gap);
    ret += fmt::format("    max_strip_gap = {}\n", clusterer_y.max_strip_gap);

    ret += fmt::format("  Matcher\n    max_delta_time = {}\n",
        matcher_max_delta_time);
    ret += fmt::format("  Send tracks = {}\n", (send_tracks ? "YES" : "no"));
    if (send_tracks) {
      ret += fmt::format("    sample_minhits = {}\n", track_sample_minhits);
    }

    ret += "\n";

    ret += "Event analysis\n";
    ret += fmt::format("  weighted = {}\n", (analyze_weighted ? "YES" : "no"));
    ret += fmt::format("  max_timebins = {}\n", analyze_max_timebins);
    ret += fmt::format("  max_timedif = {}\n", analyze_max_timedif);

    ret += "  Filters:\n";
    ret += fmt::format("    enforce_lower_uncertainty_limit = {}\n",
        (filter.enforce_lower_uncertainty_limit ? "YES" : "no"));
    if (filter.enforce_lower_uncertainty_limit) {
      ret += fmt::format("    lower_uncertainty_limit = {}\n",
          filter.lower_uncertainty_limit);
    }
    ret += fmt::format("    enforce_minimum_hits = {}\n",
        (filter.enforce_minimum_hits ? "YES" : "no"));
    if (filter.enforce_minimum_hits) {
      ret += fmt::format("    minimum_hits = {}\n",
          filter.minimum_hits);
    }

    ret += fmt::format("  geometry_x = {}\n", geometry.nx());
    ret += fmt::format("  geometry_y = {}\n", geometry.ny());
  }

  return ret;
}

}
