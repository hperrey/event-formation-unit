set(AdcSimulator_SRC
  AdcSimulator.cpp
  ContinousSamplingTimer.cpp
  DataPacket.cpp
  FPGASim.cpp
  PoissonDelay.cpp
  SampleRunGenerator.cpp
  SamplingTimer.cpp
)

set(AdcSimulator_INC
  AmpEventDelay.h
  ContinousSamplingTimer.h
  DataPacket.h
  FPGASim.h
  PoissonDelay.h
  SampleRunGenerator.h
  SamplingTimer.h
)

find_package(Threads REQUIRED)

add_executable(AdcSimulator EXCLUDE_FROM_ALL ${AdcSimulator_SRC} ${AdcSimulator_INC})
set_target_properties(AdcSimulator PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_include_directories(AdcSimulator PRIVATE ${ASIO_INCLUDE_DIR} "../" "../../common/")

target_link_libraries(AdcSimulator ${CMAKE_THREAD_LIBS_INIT})

get_filename_component(ADC_CONFIG_FILE "AdcIntegrationTestConfig.ini" ABSOLUTE)
add_custom_command(
        TARGET AdcSimulator POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${ADC_CONFIG_FILE}
                "$<TARGET_FILE_DIR:efu>/")
