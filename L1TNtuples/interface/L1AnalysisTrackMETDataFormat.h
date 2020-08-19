#ifndef __L1Analysis_L1AnalysisTrackMETDataFormat_H__
#define __L1Analysis_L1AnalysisTrackMETDataFormat_H__

//-------------------------------------------------------------------------------
// Created 20/04/2010 - E. Conte, A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------




namespace L1Analysis
{
  struct L1AnalysisTrackMETDataFormat
  {
    L1AnalysisTrackMETDataFormat(){Reset();};
    ~L1AnalysisTrackMETDataFormat(){};

    void Reset()
    {
      GenMet =0;
      GenMetPhi = 0;
      GenSumEt = 0;
      TrkMet = 0;
      TrkMetPhi = 0;
      TrkSumEt = 0;
      CutTrkMet = 0;
      CutTrkMetPhi = 0;
      CutTrkSumEt = 0;
      MVATrkMet = 0;
      MVATrkMetPhi = 0;
      MVATrkSumEt = 0;
      SimTrkMet = 0;
      SimTrkMetPhi = 0;
      SimTrkSumEt = 0;
    }

    double GenMet;
    double GenMetPhi;
    double GenSumEt;
    double TrkSumEt;
    double TrkMet;
    double TrkMetPhi;
    
    double CutTrkMet;
    double CutTrkMetPhi;
    double CutTrkSumEt;
    double MVATrkMet;
    double MVATrkMetPhi;
    double MVATrkSumEt;
    double SimTrkMet;
    double SimTrkMetPhi;
    double SimTrkSumEt;

  }; 
}
#endif

