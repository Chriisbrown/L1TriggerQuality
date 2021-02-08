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
      Trk2Met = 0;
      Trk2MetPhi = 0;
      Trk2SumEt = 0;
      Trk3Met = 0;
      Trk3MetPhi = 0;
      Trk3SumEt = 0;
      Trk10Met = 0;
      Trk10MetPhi = 0;
      Trk10SumEt = 0;
      Trk100Met = 0;
      Trk100MetPhi = 0;
      Trk100SumEt = 0;
    }

    double GenMet;
    double GenMetPhi;
    double GenSumEt;
    
    double Trk2SumEt;
    double Trk2Met;
    double Trk2MetPhi;
    
    double Trk3SumEt;
    double Trk3Met;
    double Trk3MetPhi;
    
    double Trk10SumEt;
    double Trk10Met;
    double Trk10MetPhi;
    
    double Trk100SumEt;
    double Trk100Met;
    double Trk100MetPhi;
    
    
    

  }; 
}
#endif
