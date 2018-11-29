#ifndef antiNumuCCMultiPiSelection_h
#define antiNumuCCMultiPiSelection_h

#include "SelectionBase.hxx"
#include "antiNumuCCSelection.hxx"
#include "numuCCMultiPiSelection.hxx"



class antiNumuCC1piXsecSelection: public SelectionBase{
public:
  antiNumuCC1piXsecSelection(bool forceBreak=true);
  virtual ~antiNumuCC1piXsecSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  
  ToyBoxB* MakeToyBox(); 
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1AntiNuMuCC0Pi;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
   
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  
  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;    
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
 
  void SetDetectorFV(SubDetId_h det, Int_t ibranch=-1){
    SelectionBase::SetDetectorFV(det, ibranch);
    // Set to the selection that are used
    _antiNumuCCSelection.SetDetectorFV(det, ibranch); 
    _numuCCMultiPiSelection.SetDetectorFV(det, ibranch); 
  }



protected:
  antiNumuCCSelection _antiNumuCCSelection;
  numuCCMultiPiSelection _numuCCMultiPiSelection;
  
  Int_t _MuonPIDCutIndex;
  Int_t _FindPionsStepIndex; 
  bool  _useECalPiZeroInfo;
};

class ToyBoxAntiCCMultiPi: public ToyBoxCCMultiPi{

public:
  ToyBoxAntiCCMultiPi(){}

  virtual ~ToyBoxAntiCCMultiPi(){}

};

inline ToyBoxB* antiNumuCC1piXsecSelection::MakeToyBox(){return new ToyBoxAntiCCMultiPi();}



class FillSummaryAction_antinumuCC1piXsec: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_antinumuCC1piXsec();}
};

class FindPionsAction_antinumuCC1piXsec: public StepBase{
public:
  using StepBase::Apply;
  FindPionsAction_antinumuCC1piXsec(){
    pionSelParams.useTPCPions                 = (bool)ND::params().GetParameterI("psycheSelections.antinumuCCMultiPi.UseTPCPions");
    pionSelParams.useME                       = (bool)ND::params().GetParameterI("psycheSelections.antinumuCCMultiPi.UseME");
    pionSelParams.useFGDPions                 = (bool)ND::params().GetParameterI("psycheSelections.antinumuCCMultiPi.UseFGDPions");
    pionSelParams.useOldSecondaryPID          = (bool)ND::params().GetParameterI("psycheSelections.antinumuCCMultiPi.OldSecondaryPID");
    pionSelParams.useECalPiZeroInfo           = (bool)ND::params().GetParameterI("psycheSelections.antinumuCCMultiPi.UseECalPiZeroInfo");
    
    pionSelParams.ECalMostUpstreamLayerHitCut = (Int_t)ND::params().GetParameterI("psycheSelections.antinumuCCMultiPi.MostUpstreamLayerHitCut");
    // Default
    pionSelParams.ECalEMEnergyCut = 30.;
    pionSelParams.ECalPIDMipEmCut = 0.;
  } 
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FindPionsAction_antinumuCC1piXsec();}

protected:
  mutable multipi::PionSelectionParams pionSelParams;
};


#endif
