
option(GOOGLE_BENCHMARK "Enable google benchmark for unit tests" OFF)

set(benchmark_targets "" CACHE INTERNAL "All targets")

#
# Generate benchmark targets
#
function(create_benchmark_executable exec_name link_libraries)
  if(GOOGLE_BENCHMARK)
    add_executable(${exec_name} EXCLUDE_FROM_ALL
      ${${exec_name}_SRC}
      ${${exec_name}_INC})
    target_include_directories(${exec_name}
      PRIVATE ${GTEST_INCLUDE_DIRS})
    set_target_properties(${exec_name} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/benchmarks")

    target_link_libraries(${exec_name}
      ${link_libraries}
      ${GTEST_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
      -lbenchmark)
    set(benchmark_targets
      ${exec_name}
      ${benchmark_targets}
      CACHE INTERNAL "All targets")
  else()
    message(STATUS "skipping benchmark for ${exec_name} (can be enabled by cmake -DGOOGLE_BENCHMARK=YES)")
  endif()
endfunction(create_benchmark_executable)
