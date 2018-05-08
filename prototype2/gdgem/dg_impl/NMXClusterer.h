#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <future>   // async

#include <gdgem/vmm2srs/SRSMappings.h>
#include <gdgem/vmm2srs/SRSTime.h>
#include <gdgem/dg_impl/NMXClusterMatcher.h>

class HitsQueue {
public:
  HitsQueue(SRSTime Time, double deltaTimeHits);
  void store(uint16_t strip, uint16_t adc, double chipTime);
  void sort_and_correct();
  void CorrectTriggerData();
  void subsequentTrigger(bool);

  const HitContainer& hits() const;

private:
  HitContainer hitsOld;
  HitContainer hitsNew;
  HitContainer hitsOut;

  SRSTime pTime;
  double pDeltaTimeHits {200};
  bool m_subsequentTrigger {false};
};

class NMXClusterMatcher {
public:
  NMXClusterMatcher(double dPlane);
  void match(std::vector<ClusterNMX>& x, std::vector<ClusterNMX>& y);

  size_t stats_cluster_count {0};
  std::vector<ClusterNMX> matched_clusters;

private:
  double pdPlane {0};
};

class NMXClusterer {
public:
  NMXClusterer(SRSTime time,
               SRSMappings chips,
               uint16_t adcThreshold, size_t minClusterSize,
               double deltaTimeHits, uint16_t deltaStripHits, double deltaTimeSpan);

  ~NMXClusterer();

  // Analyzing and storing the hits
  bool AnalyzeHits(int triggerTimestamp, unsigned int frameCounter, int fecID,
                   int vmmID, int chNo, int bcid, int tdc, int adc,
                   int overThresholdFlag);

  // Analyzing and storing the clusters
  void AnalyzeClusters();

  void ClusterByTime(const HitContainer &oldHits, double dTime, int dStrip,
                    double dSpan, std::vector<ClusterNMX>& clusters);
  void ClusterByStrip(HitContainer &cluster, int dStrip, double dSpan,
                      std::vector<ClusterNMX>& clusters, double maxDeltaTime);

  void StoreClusters(std::vector<ClusterNMX>& clusters, double clusterPosition,
                     short clusterSize, int clusterADC, double clusterTime,
                     double maxDeltaTime, int maxDeltaStrip, double deltaSpan);

  bool ready() const;

  // Statistics counters
  size_t stats_fc_error{0};
  size_t stats_bcid_tdc_error{0};
  size_t stats_triggertime_wraps{0};
  size_t stats_clusterX_count{0};
  size_t stats_clusterY_count{0};

  const uint8_t planeID_X{0};
  const uint8_t planeID_Y{1};

  std::vector<ClusterNMX> m_tempClusterX;
  std::vector<ClusterNMX> m_tempClusterY;

private:
  SRSTime pTime;
  SRSMappings pChips;

  uint16_t pADCThreshold;
  size_t pMinClusterSize;
  double pDeltaTimeHits;
  uint16_t pDeltaStripHits;
  double pDeltaTimeSpan;

  // These are in play for triggering the actual clustering
  double m_oldTriggerTimestamp_ns = 0;
  unsigned int m_oldFrameCounter = 0;

  // For debug output only
  double m_timeStamp_ms = 0;
  int m_oldVmmID = 0;
  int m_eventNr = 0;

  // For all 0s correction
  int m_oldBcidX = 0;
  int m_oldTdcX = 0;
  int m_oldBcidY = 0;
  int m_oldTdcY = 0;

  HitsQueue hitsX;
  HitsQueue hitsY;
};
