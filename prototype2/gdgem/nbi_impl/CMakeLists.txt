cmake_minimum_required(VERSION 3.5)

project(NMXClusterer CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-error=unused-function")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(NMXCLUSTERER_INC
        clusterer/include/NMXClustererSettings.h
        clusterer/include/NMXClustererDefinitions.h
        clusterer/include/NMXClusterer.h
        clusterer/include/NMXPlaneClusterer.h
        clusterer/include/NMXBoxAdministration.h
        clusterer/include/NMXClusterManager.h
        clusterer/include/NMXClusterPairing.h
        clusterer/include/NMXQmatrix.h
        clusterer/include/NMXLocationFinder.h)

set(NMXCLUSTERER_SRC
        clusterer/src/NMXClusterer.cpp
        clusterer/src/NMXPlaneClusterer.cpp
        clusterer/src/NMXBoxAdministration.cpp
        clusterer/src/NMXClusterManager.cpp
        clusterer/src/NMXClusterPairing.cpp
        clusterer/src/NMXLocationFinder.cpp)

add_library(nmxbase STATIC ${NMXCLUSTERER_INC} ${NMXCLUSTERER_SRC})

set(NMXHELPER_INC
        helper/include/NMXClustererHelper.h
        helper/include/ClusterReader.h
        helper/include/NMXClustererVerification.h
        helper/include/SpecialDataReader.h
        helper/include/VerifyClusters.h
        helper/include/WriteVerificationToDisk.h)

set(NMXHELPER_SRC
        helper/src/ClusterReader.cpp
        helper/src/NMXClustererVerification.cpp
        helper/src/SpecialDataReader.cpp
        helper/src/VerifyClusters.cpp
        helper/src/WriteVerificationToDisk.cpp)

add_library(nmxhelper STATIC ${NMXHELPER_INC} ${NMXHELPER_SRC})

add_executable(testnmxclustererfile testNMXClustererFile.cpp nmxclustererh5.cpp)
target_link_libraries(testnmxclustererfile nmxbase nmxhelper Threads::Threads)

find_package(Boost COMPONENTS system filesystem thread program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(nmxh5read_INC ../nmx/Readout.h ../nmx/ReadoutFile.h)
set(nmxh5read_SRC misc/readnmx5htest.cpp ../nmx/ReadoutFile.cpp)

create_executable(nmxh5read)

find_package(HDF5 REQUIRED COMPONENTS C)
include_directories(${HDF5_INCLUDE_DIRS})

add_library(nmxreadout STATIC ../nmx/Readout.h ../nmx/ReadoutFile.h ../nmx/ReadoutFile.cpp)

set(nmxclustererh5_INC
        ${NMXCLUSTERER_INC}
        ../nmx/Readout.h
        ../nmx/ReadoutFile.h)
set(nmxclustererh5_SRC nmxclustererh5.cpp
        ${NMXCLUSTERER_SRC}
        ../nmx/ReadoutFile.cpp )

create_executable(nmxclustererh5 -latomic)

#add_executable(nmxclustererh5 nmxclustererh5.cpp)
#target_link_libraries(nmxclustererh5 nmxbase nmxreadout Threads::Threads ${EFU_COMMONLIBS})


#add_library(nmxh5read STATIC ../nmx/Readout.h ../nmx/ReadoutFile.h ../nmx/ReadoutFile.cpp)

#add_executable(readnmxh5test misc/readnmx5htest.cpp)
#target_link_libraries(readnmxh5test ${Boost_LIBRARYDIR} nmxh5read)

#add_executable(verifyclusterer verifyClusterer.cpp)
#target_link_libraries(verifyclusterer nmxhelper)

# Tests

#set(NMXBoxAdministrationTest_INC ${NMXCLUSTERER_INC} )
#set(NMXBoxAdministrationTest_SRC
#        tests/testBoxAdministration.cpp
#        clusterer/src/NMXBoxAdministration.cpp
#        )
#create_test_executable(NMXBoxAdministrationTest)