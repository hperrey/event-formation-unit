/** Copyright (C) 2016-2018 European Spallation Source ERIC */
//===----------------------------------------------------------------------===//
///
/// \file
/// Class for handling configuration via command line options
///
//===----------------------------------------------------------------------===//

#include <common/DetectorModuleRegister.h>
#include <common/EFUArgs.h>
#include <common/Version.h>
#include <common/Log.h>
#include <cstdio>
#include <fstream>
#include <regex>
#include <string>
#include <algorithm>

using namespace std::literals::string_literals;

EFUArgs::EFUArgs() {
  // clang-format off
  CLIParser.set_help_flag(); // Removes the default help flag
  CLIParser.allow_extras(true);
  CLIParser.get_formatter()->column_width(30);

  HelpOption = CLIParser.add_flag("-h,--help", "Print this help message and exit")
      ->group("EFU Options")->configurable(false);

  CLIParser.add_flag("--version", PrintVersion, "Print version and exit")
      ->group("EFU Options")->configurable(false);

  CLIParser.add_option("-a,--logip", GraylogConfig.address, "Graylog server IP address")
      ->group("EFU Options")->default_str("127.0.0.1");

  CLIParser.add_option("-b,--broker_addr", EFUSettings.KafkaBroker, "Kafka broker address")
      ->group("EFU Options")->default_str("localhost");

  CLIParser.add_option("-t,--broker_topic", EFUSettings.KafkaTopic, "Kafka broker topic")
      ->group("EFU Options")->default_str("Detector_data");

  CLIParser.add_option("-c,--core_affinity", [this](std::vector<std::string> Input) {
                    return parseAffinityStrings(Input);
                  }, "Thread to core affinity. Ex: \"-c input_t:4\"")
      ->group("EFU Options");

  CLIParser.add_option("-l,--log_level", [this](std::vector<std::string> Input) {
    return parseLogLevel(Input);
  }, "Set log message level. Set to 1 - 7 or one of \n                              `Critical`, `Error`, `Warning`, `Notice`, `Info`,\n                              or `Debug`. Ex: \"-l Notice\"")
  ->group("EFU Options")->default_str("Info");

  CLIParser.add_option("--log_file", LogFileName, "Write log messages to file.")
  ->group("EFU Options");

  CLIParser.add_flag("--nohwcheck", EFUSettings.NoHwCheck, "Perform HW check or not")
      ->group("EFU Options");

  CLIParser.add_flag("--udder", EFUSettings.TestImage, "Generate a test image")
      ->group("EFU Options");

  std::string DetectorDescription{"Detector name"};
  std::map<std::string, DetectorModuleSetup> StaticDetModules =
      DetectorModuleRegistration::getFactories();
  if (not StaticDetModules.empty()) {
    auto GetModuleName = [](std::string &A, auto const &B) {
      return std::move(A) + " " + B.first;
    };
    auto TempString = std::accumulate(StaticDetModules.begin(), StaticDetModules.end(), " (Known modules:"s, GetModuleName);
    DetectorDescription += TempString + ")";
  }
  DetectorOption = CLIParser.add_option("-d,--det", DetectorName, DetectorDescription)
      ->group("EFU Options")->required();

  CLIParser.add_option("-i,--dip", EFUSettings.DetectorAddress,
                       "IP address of receive interface")
      ->group("EFU Options")->default_str("0.0.0.0");

  CLIParser.add_option("-p,--port", EFUSettings.DetectorPort, "TCP/UDP receive port")
      ->group("EFU Options")->default_str("9000");

  CLIParser.add_option("-m,--cmdport", EFUSettings.CommandServerPort,
                       "Command parser tcp port")
      ->group("EFU Options")->default_str("8888");

  CLIParser.add_option("-g,--graphite", EFUSettings.GraphiteAddress,
                       "IP address of graphite metrics server")
      ->group("EFU Options")->default_str("127.0.0.1");

  CLIParser.add_option("-r,--region", EFUSettings.GraphiteRegion,
                       "name of detector region covered by this pipeline")
      ->group("EFU Options")->default_str("region1");

  CLIParser.add_option("-o,--gport", EFUSettings.GraphitePort, "Graphite tcp port")
      ->group("EFU Options")->default_str("2003");

  CLIParser.add_option("-s,--stopafter", EFUSettings.StopAfterSec,
                       "Terminate after timeout seconds")
      ->group("EFU Options")->default_str("4294967295"); // 0xffffffffU

  WriteConfigOption = CLIParser
      .add_option("--write_config", ConfigFileName,
                  "Write CLI options with default values to config file.")
      ->group("EFU Options")->configurable(false);

  ReadConfigOption =   CLIParser
      .set_config("--read_config", "", "Read CLI options from config file.", false)
      ->group("EFU Options")->excludes(WriteConfigOption);

  CLIParser.add_option("--updateinterval", EFUSettings.UpdateIntervalSec,
                       "Stats and event data update interval (seconds).")
      ->group("EFU Options")->default_str("1");

  CLIParser.add_option("--rxbuffer", EFUSettings.DetectorRxBufferSize,
                       "Receive from detector buffer size.")
      ->group("EFU Options")->default_str("2000000");

  CLIParser.add_option("--txbuffer", EFUSettings.DetectorTxBufferSize,
                  "Transmit to detector buffer size.")
      ->group("EFU Options")->default_str("9216");
  // clang-format on
}

bool EFUArgs::parseAffinityStrings(
    std::vector<std::string> ThreadAffinityStrings) {
  bool CoreIntegerCorrect = false;
  int CoreNumber = 0;
  try {
    CoreNumber = std::stoi(ThreadAffinityStrings.at(0));
    CoreIntegerCorrect = true;
  } catch (std::invalid_argument &e) {
    // No nothing
  }
  if (ThreadAffinityStrings.size() == 1 and CoreIntegerCorrect) {
    ThreadAffinity.emplace_back(ThreadCoreAffinitySetting{
        "implicit_affinity", static_cast<std::uint16_t>(CoreNumber)});
  } else {
    std::string REPattern = "([^:]+):(\\d{1,2})";
    std::regex AffinityRE(REPattern);
    std::smatch AffinityRERes;
    for (auto &AffinityStr : ThreadAffinityStrings) {
      if (not std::regex_match(AffinityStr, AffinityRERes, AffinityRE)) {
        return false;
      }
      ThreadAffinity.emplace_back(ThreadCoreAffinitySetting{
          AffinityRERes[1],
          static_cast<std::uint16_t>(std::stoi(AffinityRERes[2]))});
    }
  }
  return true;
}

bool EFUArgs::parseLogLevel(std::vector<std::string> LogLevelString) {
  std::map<std::string, int> LevelMap{{"Critical", 2}, {"Error", 3}, {"Warning", 4}, {"Notice", 5}, {"Info", 6}, {"Debug", 7}};
  if (LogLevelString.size() != 1) {
    return false;
  }
  try {
    LogMessageLevel = LevelMap.at(LogLevelString.at(0));
    return true;
  } catch (std::out_of_range &e) {
    // Do nothing
  }
  try {
    int TempLogMessageLevel = std::stoi(LogLevelString.at(0));
    if (TempLogMessageLevel < 1 or TempLogMessageLevel > 7) {
      return false;
    }
    LogMessageLevel = TempLogMessageLevel;
  } catch (std::invalid_argument &e) {
    return false;
  }
  return true;
}

void EFUArgs::printSettings() {
  // clang-format off
  LOG(INIT, Sev::Info, "Starting event processing pipeline2 with main properties:");
  LOG(INIT, Sev::Info, "  Detector:                 {}",    DetectorName);
  LOG(INIT, Sev::Info, "  Rx UDP Socket:            {}:{}",
         EFUSettings.DetectorAddress, EFUSettings.DetectorPort);
  LOG(INIT, Sev::Info, "  Perform HW checks         {}", !EFUSettings.NoHwCheck);
  LOG(INIT, Sev::Info, "  Kafka broker:             {}", EFUSettings.KafkaBroker);
  LOG(INIT, Sev::Info, "  Log IP:                   {}", GraylogConfig.address);
  LOG(INIT, Sev::Info, "  Graphite TCP socket:      {}:{}",
        EFUSettings.GraphiteAddress, EFUSettings.GraphitePort);
  LOG(INIT, Sev::Info, "  CLI TCP Socket:           localhost:{}", EFUSettings.CommandServerPort);

  if (EFUSettings.StopAfterSec == 0xffffffffU) {
    LOG(INIT, Sev::Info, "  Stopafter:                never");
  } else {
    LOG(INIT, Sev::Info, "  Stopafter:                {}s", EFUSettings.StopAfterSec);
  }

  LOG(INIT, Sev::Info, "<<< NOT ALL CONFIGURABLE SETTINGS MAY BE DISPLAYED >>>");
  // clang-format on
}

void EFUArgs::printHelp() { std::cout << CLIParser.help(); }

void EFUArgs::printVersion() { std::cout << efu_version() << '\n'; }

EFUArgs::Status EFUArgs::parseFirstPass(const int argc, char *argv[]) {
  try {
    CLIParser.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    // Do nothing, as we only care about the version flag in this pass.
  }
  if (PrintVersion) {
    printVersion();
    return Status::EXIT;
  }

  try {
    CLIParser.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    CLIParser.exit(e);
  }
  if ((*HelpOption and not*DetectorOption) or
      (not*HelpOption and not*DetectorOption)) {
    printHelp();
    return Status::EXIT;
  }
  CLIParser.clear();
  CLIParser.allow_extras(false);
  return Status::CONTINUE;
}

EFUArgs::Status EFUArgs::parseSecondPass(const int argc, char *argv[]) {
  try {
    CLIParser.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    CLIParser.exit(e);
    return Status::EXIT;
  }
  if (*HelpOption and *DetectorOption) {
    printHelp();
    return Status::EXIT;
  }
  if (*WriteConfigOption) {
    std::ofstream ConfigFile(ConfigFileName, std::ios::binary);
    if (not ConfigFile.is_open()) {
      LOG(INIT, Sev::Error, "Failed to open config file for writing.");
      return Status::EXIT;
    }
    ConfigFile << CLIParser.config_to_str(true, true);
    ConfigFile.close();
    LOG(INIT, Sev::Info, "Config file created, now exiting.");
    return Status::EXIT;
  }
  return Status::CONTINUE;
}
