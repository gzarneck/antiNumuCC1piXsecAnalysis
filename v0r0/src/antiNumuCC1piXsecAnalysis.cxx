#include "antiNumuCC1piXsecAnalysis.hxx"
#include "antiNumuCC1piXsecSelection.hxx"
//#include "antiNumuCCMultiPiFGD2Selection.hxx"
//#include "antiNumuCCMultiTrackSelection.hxx"
//#include "antiNumuCCMultiTrackFGD2Selection.hxx"

//constructor
//********************************************************************
antiNumuCC1piXsecAnalysis::antiNumuCC1piXsecAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("antiNumuCC1piXsecAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ANTINUMUCC1PIXSECANALYSISROOT")));

  // Create a antiNumuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _antiNumuCCAnalysis = new antiNumuCCAnalysis(this);
  
  // Create a numuCCMultiPiAnalysis passing this analysis to the constructor. In that way the same managers are used
  _numuCCMultiPiAnalysis = new numuCCMultiPiAnalysis(this);

  // Use the antiNumuCCAnalysis (in practice that means that the same box and event will be used for the antiNumuCCAnalysis as for this analysis)
  UseAnalysis(_antiNumuCCAnalysis);
  UseAnalysis(_numuCCMultiPiAnalysis);
}

//********************************************************************
bool antiNumuCC1piXsecAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis instead of numuCCAnalysis
  // since we do not want to call and overload numu categories
  if(!baseTrackerAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("antiNumuCC1piXsecAnalysis.MinAccumLevelToSave"));

  // which analysis: FGD1, FGD2 or FGDs
  _whichFGD = ND::params().GetParameterI("antiNumuCC1piXsecAnalysis.Selections.whichFGD");
  if (_whichFGD == 3) {
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "WARNING: only for events with accum_level > 6 the vars in the output microtree will surely refer to the muon candidate in that FGD" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
  }
  
  
  

  // CC-multi-pi or CC-multi-track
   _runMultiTrack = ND::params().GetParameterI("antiNumuCC1piXsecAnalysis.Selections.RunMultiTrack");
  
  // Define antinu categories (different legend from numuCC)
  // for FGD2 same categories with prefix "fgd2", i,e, "fgd2reaction" etc.)
  anaUtils::AddStandardAntiNumuCategories();
  //anaUtils::AddStandardAntiNumuCategories("fgd2");
  //TODO: add categories
  antiNumuCC1piXsecAnalysis::AddCategories();
  _numuCCMultiPiAnalysis->SetStoreAllTruePrimaryPions((bool)ND::params().GetParameterI("antiNumuCC1piXsecAnalysis.MicroTrees.StoreAllTruePrimaryPions"));
  
  return true;
}

//********************************************************************
void antiNumuCC1piXsecAnalysis::DefineSelections(){
//********************************************************************
  
  sel().AddSelection("kTrackerAntiNumuCC1piXsecAnalysis",    "antiNumu FGD1 CC single Pion selection",  new antiNumuCC1piXsecSelection(false));
/*
  // FGD1 antineutrino analysis:
  if(_whichFGD==1 || _whichFGD==3){
    // ---- Inclusive CC ----
    if(!_runMultiTrack)
      sel().AddSelection("kTrackerAntiNumuCCMultiPi",    "antiNumu FGD1 CC Multiple Pion selection",  new antiNumuCCMultiPiSelection(false));
    // ---- CC Multi Pion Samples ----
    else
      sel().AddSelection("kTrackerAntiNumuCCMultiTrack", "antiNumu FGD1 CC Multiple Track selection", new antiNumuCCMultiTrackSelection(false));
  }
  // FGD2 antineutrino analysis:
  if (_whichFGD==2 || _whichFGD==3){
    // ---- CC Inclusive ----
    if(!_runMultiTrack)
      sel().AddSelection("kTrackerAntiNumuCCMultiPiFGD2",  "antiNumu FGD2 CC Multiple Pion selection",  new antiNumuCCMultiPiFGD2Selection(false));
    // ---- CC Multi Track Samples ----
    else
      sel().AddSelection("kTrackerAntiNumuMultiTrackFGD2", "antiNumu FGD2 CC Multiple Track selection", new antiNumuCCMultiTrackFGD2Selection(false));
  }
*/
  
}

//********************************************************************
void antiNumuCC1piXsecAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------

  if(_runMultiTrack){
    _antiNumuCCAnalysis->DefineMicroTrees(addBase);
  }
  else{
    _numuCCMultiPiAnalysis->DefineMicroTrees(addBase); 
  }
  
  
}

//********************************************************************
void antiNumuCC1piXsecAnalysis::DefineTruthTree(){
//********************************************************************

  if(_runMultiTrack){
    _antiNumuCCAnalysis->DefineTruthTree();
  }
  else{
    _numuCCMultiPiAnalysis->DefineTruthTree();
  }
}

//********************************************************************
void antiNumuCC1piXsecAnalysis::FillMicroTrees(bool addBase){
//********************************************************************
  
  if(_runMultiTrack){
    _antiNumuCCAnalysis->FillMicroTrees(addBase);
  }
  else{
    _numuCCMultiPiAnalysis->FillMicroTrees(addBase); 
  }
  
}

//********************************************************************
void antiNumuCC1piXsecAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************
 
  if(_runMultiTrack){
    _antiNumuCCAnalysis->FillToyVarsInMicroTrees(addBase);
  }
  else{
    _numuCCMultiPiAnalysis->FillToyVarsInMicroTrees(addBase); 
  }
  
}

//********************************************************************
bool antiNumuCC1piXsecAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  if (_whichFGD == 3) fgdID = SubDetId::kFGD;

  bool IsAntinu = true;

  // GetReactionCC already takes into account the outFV and also
  // the NuWro reaction code for 2p2h in prod5 (that is 70)
  bool antiNumuCCinFV = (anaUtils::GetReactionCC(vtx, fgdID, IsAntinu) == 1);

  // Since our topology definition doesn't consider the reaction code
  // and since a muon pair can be created in the FSI (likely in DIS)
  // in principle we might have a non-CC vertex categorized as CCother
  // (nevertheless I didn't find any)
  int topo = anaUtils::GetTopology(vtx, fgdID, IsAntinu);
  bool topoCCinFV = (topo == 0 || topo == 1 || topo == 2);

  return (antiNumuCCinFV || topoCCinFV);  
}

//********************************************************************
void antiNumuCC1piXsecAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  _antiNumuCCAnalysis->FillTruthTree(vtx);
  
  if (!_runMultiTrack){
    _numuCCMultiPiAnalysis->FillTruePionInfo(vtx); 
  }
}

//********************************************************************
void antiNumuCC1piXsecAnalysis::FillCategories(){
//********************************************************************
AnaTrackB *track = static_cast<AnaTrack*>(box().MainTrack);
std::string prefix = "";
  _antiNumuCCAnalysis->FillCategories();


anaUtils::_categ->SetCode(prefix + "mycateg", GetMyCateg(track));

}

//********************************************************************
int antiNumuCC1piXsecAnalysis::GetMyCateg(AnaTrackB* track){
//************************
if (!track) return -1;
if (!track->GetTrueParticle()) return -1;

  /* Classify reaction types
  -1 = no true vertex
   0 = CC 0pi      = mu + X nucleons (+ 0 mesons)
   1 = CC 1pi+(-)  = mu + X nucleons + 1 pion +(-)
   2 = CC other
   3 = BKG: not (anti-)numu, NC
                               7 = out of FV
                                  */
                            
    AnaTrueVertex* trueVertex = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
    if (!trueVertex) return -1; //no truth
    
    // out of FGD1 FV
    if ( ! anaUtils::InFiducialVolume(SubDetId::kFGD1,trueVertex->Position)) return -2;
                                        
                                        
    Int_t reac   = trueVertex->ReacCode;
    Int_t nutype = trueVertex->NuPDG;
                        
    if (abs(reac) > 30 && abs (reac) < 70) return 4; //NC is flavor blind
    if (abs(nutype) == 12) return 6; //nu-e CC interactions
    if (nutype == 14) return 5; //NuMu background
                                        
    if (nutype == -14) {
      /// primary particles from the true vertex
     // Int_t Nmuon =     trueVertex->NPrimaryParticles[ParticleId::kMuon];
     // Int_t Nantimuon = trueVertex->NPrimaryParticles[ParticleId::kAntiMuon];
      Int_t Nmeson =    trueVertex->NPrimaryParticles[ParticleId::kMesons];
     // Int_t Npipos =    trueVertex->NPrimaryParticles[ParticleId::kPiPos];
      Int_t Npineg =    trueVertex->NPrimaryParticles[ParticleId::kPiNeg];
                                                      
      // non numu CC, i.e. BKG
 //     if ( ! IsAntinu && Nmuon <= 0) return BKG; // BKG
 //     if (IsAntinu && Nantimuon <= 0) return BKG; // BKG
  
      // CC 0pi, i.e. mu & 0 mesons
      if (Nmeson == 0) return 0;

      // CC 1pi-, i.e. mu & 1 pi & no other mesons
      if (Nmeson == 1) {
      //if ( ! IsAntinu && Npipos == 1) return CC_1pi_0meson;
      if(Npineg == 1) return 1;
    }
                                                                  
                                               
    // CC other
    return 2; 
  }
 
  return 7; //any other background
            //it's empty


}


void antiNumuCC1piXsecAnalysis::AddCategories(){
const int NTYPES = 7;
std::string part_types[NTYPES] = {"CC-0#pi", "CC-1#pi^{-}", "CC-other", "BKG NC (flavour blind)", "BKG CC #nu_{#mu}", "BKG CC #nu_{e} + #bar{#nu}_{e}", "OOFV"};//, "no truth"};
int part_codes[NTYPES] = {0                , 1            , 2         , 4                       , 5                 , 6                               , -2    };//, -1};
int part_colors[NTYPES] = {2               , 4            , 7         , 6                       , 3                 , 5                               , 1     };//, 8 };
anaUtils::_categ->AddCategory("mycateg", NTYPES, part_types, part_codes, part_colors);
}

