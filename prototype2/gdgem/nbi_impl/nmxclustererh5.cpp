//
// Created by soegaard on 6/13/18.
//

#include <chrono>
#include "../nmx/Readout.h"
#include "../nmx/ReadoutFile.h"

#include "clusterer/include/NMXClustererDefinitions.h"
#include "clusterer/include/NMXClusterer.h"

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char *argv[]) {

    std::string h5file = "../prototype2/gdgem/clustering/test_data/run16full.h5";

    if (argc == 2) {
      h5file = argv[1];
    }

    NMXClusterer c;

    int readN = 1000000;

    int chunckSize = 1000000;

    int read = 0;
    ReadoutFile file;
    try {
      file = ReadoutFile::open(h5file);
    } catch (...) {
      printf("\nUnable to open file %s\n", h5file.c_str());
      exit(1);
    }

    if (readN < 0)
        readN = file.count();

    unsigned int npoints = 0;

    uint32_t oldSRStime = 0;
    uint32_t counter = 0;

    std::cout << "Processing " << readN << " points !" << std::endl;

    auto t1 = Clock::now();

    while (true) {

        int readpoints = chunckSize;

        if (read + readpoints >= readN)
            readpoints = readN - read;

        file.read_at(read, readpoints);

        std::cout << "Processing chunk of size " << readpoints << std::endl;

        for (auto data : file.data) {

            if (npoints % 10000 == 0)
                std::cout << ". " << std::flush;

            uint32_t SRStime = data.srs_timestamp;

            if (SRStime != oldSRStime) {
                counter++;
                oldSRStime = SRStime;
            }

            double calctime = (counter * 65536 - (data.bcid >= 2000 ? 65536 : 0)) * 3.125;

            double chiptime = data.bcid * 50 + data.tdc * 60. / 255.;

            double timestamp_ns = calctime + chiptime;
            uint32_t timestamp = static_cast<uint32_t >(timestamp_ns);


            unsigned int plane = (data.chip_id > 7 ? 1 : 0);
            uint32_t strip = (data.chip_id - (plane ? 8 : 0)) * 64 + data.channel;

            nmx::DataPoint point = {strip, data.adc, timestamp};
            c.addDatapoint(plane, point);
            npoints++;
        }

        std::cout << std::endl;

        read += readpoints;
        std::cout << "Now processed " << read << " points!" << std::endl;

        if (read == readN)
            break;
    }

    auto t2 = Clock::now();

    long time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    unsigned int w1 = 40;
    unsigned int w2 = 10;

    std::cout << std::endl;
    std::cout.width(w1); std::cout << std::left << "Processing time :" << std::right
                                   << std::setw(w2) << time << " us" << std::endl;
    std::cout << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of inserted data-points [X]:" << std::right
                                   << std::setw(w2) << c.getNumberofInsertedDataPointsX() << " points" << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of inserted data-points [Y]:" << std::right
                                   << std::setw(w2) << c.getNumberofInsertedDataPointsY() << " points" << std::endl;
    std::cout.width(w1); std::cout << std::left << "Data-point processing time :" << std::right
                                    << std::setw(w2) << 1.* static_cast<double>(time)/
                                                            static_cast<double>(npoints) << " us" << std::endl;
     std::cout.width(w1); std::cout << std::left << "Data-point processing rate :"  << std::right
                                    << std::setw(w2)
                                    << 1./(static_cast<double>(time)/ static_cast<double>(npoints)/1000000.) << " Hz"
                                    << std::endl;
    std::cout << std::endl;
    uint64_t nClustersX = c.getNumberOfProducedClustersX();
    uint64_t nClustersY = c.getNumberOfProducedClustersY();
    uint64_t nPaired    = c.getNumberOfPaired();
    std::cout.width(w1); std::cout << std::left << "Number of produced clusters X :" << std::right
                                   << std::setw(w2) << nClustersX << " clusters" << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of produced clusters Y :" << std::right
                                   << std::setw(w2) << nClustersY << " clusters" << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of paired :" << std::right
                                   << std::setw(w2) << nPaired << " clusters" << std::endl;
    std::cout.width(w1); std::cout << std::left << "Ratio - paired/produced clusters [X]:" << std::right
                                   << std::setw(w2) << static_cast<double>(nPaired)/static_cast<double>(nClustersX)
                                   << std::endl;
    std::cout.width(w1); std::cout << std::left << "Ratio - paired/produced clusters [Y]:" << std::right
                                   << std::setw(w2) << static_cast<double>(nPaired)/static_cast<double>(nClustersY)
                                   << std::endl;
    std::cout << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of old points X :" << std::right
                                   << std::setw(w2) << c.getNumberOfOldPointsX() << " points" << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of old points Y :" << std::right
                                   << std::setw(w2) << c.getNumberOfOldPointsY() << " points" << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of failed Cluster-requests [X]: " << std::right
                                   << std::setw(w2) << c.getFailedClusterRequests()[0] << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of failed Cluster-requests [Y]: " << std::right
                                   << std::setw(w2) << c.getFailedClusterRequests()[1] << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of late clusters [X]: " << std::right
                                   << std::setw(w2) << c.getNumberOfLateClusters()[0] << std::endl;
    std::cout.width(w1); std::cout << std::left << "Number of late clusters [Y]: " << std::right
                                   << std::setw(w2) << c.getNumberOfLateClusters()[1] << std::endl;

    c.endRun();
    c.terminate();
}
