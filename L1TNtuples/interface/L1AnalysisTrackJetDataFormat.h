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

      nTrk2Jets = 0;
      Trk2JetEt.clear();
      Trk2JetEta.clear();
      Trk2JetPhi.clear();
      Trk2JetHt = 0;
      
      nTrk3Jets = 0;
      Trk3JetEt.clear();
      Trk3JetEta.clear();
      Trk3JetPhi.clear();
      Trk3JetHt = 0;
      
      nTrk10Jets = 0;
      Trk10JetEt.clear();
      Trk10JetEta.clear();
      Trk10JetPhi.clear();
      Trk10JetHt = 0;
      
      nTrk100Jets = 0;
      Trk100JetEt.clear();
      Trk100JetEta.clear();
      Trk100JetPhi.clear();
      Trk100JetHt = 0;


      nGenJet = 0;     
      GenJetPt.clear();
      GenJetEta.clear();
      GenJetPhi.clear();
      GenJetM.clear();
      GenJetHt = 0;


    }

    unsigned short int nTrk2Jets;
    std::vector<double> Trk2JetEt;
    std::vector<double> Trk2JetEta;
    std::vector<double> Trk2JetPhi;
    double Trk2JetHt;
    
    unsigned short int nTrk3Jets;
    std::vector<double> Trk3JetEt;
    std::vector<double> Trk3JetEta;
    std::vector<double> Trk3JetPhi;
    double Trk3JetHt;

    unsigned short int nTrk10Jets;
    std::vector<double> Trk10JetEt;
    std::vector<double> Trk10JetEta;
    std::vector<double> Trk10JetPhi;
    double Trk10JetHt;

    unsigned short int nTrk100Jets;
    std::vector<double> Trk100JetEt;
    std::vector<double> Trk100JetEta;
    std::vector<double> Trk100JetPhi;
    double Trk100JetHt;

    int nGenJet;
    std::vector<float> GenJetPt;
    std::vector<float> GenJetEta;
    std::vector<float> GenJetPhi;
    std::vector<float> GenJetM;
    double GenJetHt;




  }; 
}
#endif
