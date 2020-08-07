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
      TrkMe = 0;
      CutTrkMet = 0;
      MVATrkMet = 0;
    }

    double GenMet;
    double TrkMet;
    double CutTrkMet;
    double MVATrkMet;

  }; 
}
#endif

