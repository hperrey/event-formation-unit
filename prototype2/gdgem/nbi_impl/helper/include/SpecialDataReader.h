//
// Created by soegaard on 11/6/17.
//

#ifndef PROJECT_SPECIALDATAREADER_H
#define PROJECT_SPECIALDATAREADER_H

#include <fstream>
#include <string>
#include <vector>
#include <array>

#include "../../clusterer/include/NMXClustererDefinitions.h"


typedef std::array<uint32_t, 4> line_data;
typedef std::vector<nmx::DataPoint> plane;
typedef std::array<plane, 2> event;

class SpecialDataReader {

public:

    SpecialDataReader();

    nmx::FullCluster ReadNextEvent();

private:

    std::ifstream m_ifile;

    int m_verboseLevel = 0;

    bool isEventHeader(const std::string &line, uint &ievent);
    line_data readDataPoint(const std::string &line);

};

#endif //PROJECT_SPECIALDATAREADER_H
