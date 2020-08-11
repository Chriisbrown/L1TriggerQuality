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

      nCutTrackerJets = 0;
      CuttrackerJetEt.clear();
      CuttrackerJetEta.clear();
      CuttrackerJetPhi.clear();
      CuttrackerJetHt = 0;

      nMVATrackerJets = 0;
      MVAtrackerJetEt.clear();
      MVAtrackerJetEta.clear();
      MVAtrackerJetPhi.clear();
      MVAtrackerJetHt = 0;

      nGenJet = 0;     
      genJetPt.clear();
      genJetEta.clear();
      genJetPhi.clear();
      genJetM.clear();
      genJetHt = 0;


    }

    unsigned short int nTrkJets;
    std::vector<double> TrkJetEt;
    std::vector<double> TrkJetEta;
    std::vector<double> TrkJetPhi;
    double TrkJetHt;

    unsigned short int nCutTrkJets;
    std::vector<double> CutTrkJetEt;
    std::vector<double> CutTrkJetEta;
    std::vector<double> CutTrkJetPhi;
    double CutTrkJetHt;

    unsigned short int nMVATrkJets;
    std::vector<double> MVATrkJetEt;
    std::vector<double> MVATrkJetEta;
    std::vector<double> MVATrkJetPhi;
    double MVATrkJetHt;

  
    int nGenJet;
    std::vector<float> GenJetPt;
    std::vector<float> GenJetEta;
    std::vector<float> GenJetPhi;
    std::vector<float> GenJetM;
    double GenJetHt;




  }; 
}
#endif