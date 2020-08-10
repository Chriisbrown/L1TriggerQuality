#ifndef __L1Analysis_L1AnalysisTrackJetDataFormat_H__
#define __L1Analysis_L1AnalysisTrackJetDataFormat_H__

//-------------------------------------------------------------------------------
// Created 20/04/2010 - E. Conte, A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------


#include <vector>

namespace L1Analysis
{
  struct L1AnalysisTrackJetDataFormat
  {
    L1AnalysisTrackJetDataFormat(){Reset();};
    ~L1AnalysisTrackJetDataFormat(){};

    void Reset()
    {

      nTrackerJets = 0;
      trackerJetEt.clear();
      trackerJetEta.clear();
      trackerJetPhi.clear();
      trackerJetHt = 0;

      nGenJet = 0;     
      genJetPt.clear();
      genJetEta.clear();
      genJetPhi.clear();
      genJetM.clear();
      genJetHt = 0;


    }

    unsigned short int nTrackerJets;
    std::vector<double> trackerJetEt;
    std::vector<double> trackerJetEta;
    std::vector<double> trackerJetPhi;
    double trackerJetHt;

  
    int nGenJet;
    std::vector<float> genJetPt;
    std::vector<float> genJetEta;
    std::vector<float> genJetPhi;
    std::vector<float> genJetM;
    double genJetHt;




  }; 
}
#endif