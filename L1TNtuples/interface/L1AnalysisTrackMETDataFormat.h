#ifndef __L1Analysis_L1AnalysisTrackMETDataFormat_H__
#define __L1Analysis_L1AnalysisTrackMETDataFormat_H__

//-------------------------------------------------------------------------------
// Created 20/04/2010 - E. Conte, A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------


#include <vector>

namespace L1Analysis
{
  struct L1AnalysisTrackMETDataFormat
  {
    L1AnalysisTrackMETDataFormat(){Reset();};
    ~L1AnalysisTrackMETDataFormat(){};

    void Reset()
    {
      GensumEt.clear();
      TksumEt.clear();
      CutsumEt.clear();
      MVAsumEt.clear();
    }

    std::vector<double> GensumEt;
    std::vector<double> TksumEt;
    std::vector<double> CutsumEt;
    std::vector<double> MVAsumEt;

  }; 
}
#endif

