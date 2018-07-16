//
// Created by soegaard on 6/13/18.
//

#include <iomanip>
#include "../../nmx/Readout.h"
#include "../../nmx/ReadoutFile.h"

int main() {

    int readN = 1000;

    auto file = ReadoutFile::open("run16full.h5");

    file.read_at(0, readN);

    int w = 15;

    std::cout << std::setw(w) << "FEC" << std::setw(w) << "Chip-ID" << std::setw(w)
              << "SRS-time" << std::setw(w) << "Channel" << std::setw(w) << "BC-ID" << std::setw(w) << "Tdc"
              << std::setw(w) << "ADC" << std::setw(w) << "Over threshold" << std::setw(w) << "Chip-time"
              << std::setw(w) << "SRS-time" << std::setw(w) << "Time-stamp" << std::setw(w) << "Plane"
              << std::setw(w) << "Strip" << std::endl;

    for (int i = 0; i < readN; i++) {

        Readout data = file.data.at(i);

        double chiptime = data.bcid*50 + data.tdc*60./255.;
        double SRStime = (data.srs_timestamp - (data.bcid > 2000 ? 65536 : 0)) * 3.125;
        double timestamp_ns = SRStime + chiptime;
        uint64_t timestamp = static_cast<uint64_t >(timestamp_ns);

        int plane = (data.chip_id > 7 ? 1 : 0);
        int strip = (data.chip_id - (plane ? 7 : 0))*64 + data.channel;

        std::cout << std::setw(w) << (int)data.fec << std::setw(w) << (int)data.chip_id
                  << std::setw(w) << data.srs_timestamp << std::setw(w) << data.channel << std::setw(w) << data.bcid
                  << std::setw(w) << data.tdc << std::setw(w) << data.adc << std::setw(w)
                  << (data.over_threshold ? 1 : 0) << std::setw(w) << chiptime << std::setw(w) << SRStime
                  << std::setw(w) << timestamp << std::setw(w) << plane << std::setw(w) << strip << std::endl;
    }
}
