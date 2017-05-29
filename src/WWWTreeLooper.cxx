// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWTreeLooper.h"

#define MZ 91.1876

//WWWTree mytree;
ObjUtil::AnalysisData ana_data;
TString output_name;

//______________________________________________________________________________________
int WWWTreeLooper(TChain* chain, TString output_name, int nevents)
{

  // Before the loop set up some configuration, histograms and etc.
  beforeLoop(chain, output_name, nevents);

  // During the event loop process each event one by one
  loop();

  // After the loop is done, save histograms and etc.
  afterLoop();

  return 0;
}

//______________________________________________________________________________________
void beforeLoop(TChain* chain, TString output_name_, int nevents)
{

  // Fun start ASCII art
  PrintUtil::start();

  // Initialize configurations for event looping
  LoopUtil::resetLoopCondition(chain, nevents);

  // Replace all ".root" added accidentally
  output_name_.ReplaceAll(".root", "");
  LoopUtil::output_name = output_name_;

  // If output_name does not contain "_skimtree" then skim the event
  //if (!output_name.Contains("_skimtree"))
  //  LoopUtil::setDoSkim();

  // If output_name contains "_skimtree" remove it.
  LoopUtil::output_name.ReplaceAll("_skimtree", "");

}


//______________________________________________________________________________________
void loop()
{

  // Loop over file:ttree -> and loop over events in ttree
  // (usually I only have one file each.. so kinda redundant)
  while (LoopUtil::nextFileAndLoadTTreeWithName("t"))
  {

    initWWWTree();

    // I am assuming I only run one file at a time
    // NOTE: Later I should probably update this to be more general
    if (LoopUtil::doskim) TreeUtil::createSkimTree((LoopUtil::output_name+"_skimtree.root").Data());

    // Loop over the TTree
    while (LoopUtil::nextEvent())
    {

      if (LoopUtil::isAllEventsProcessed()) break;

      loadWWWTreeEvent();

      //=================================================================
      // <3 of the code
      // Do whatever you want to in the following function for each event
      //=================================================================
      processWWWTreeEvent();

    } // End TTree loop

  } // End Loop over files
}

//______________________________________________________________________________________
void initWWWTree()
{
  // Init the Class
  mytree.Init(LoopUtil::getCurrentTTree());
  //if (LoopUtil::doskim) mytree.LoadAllBranches();
}

//______________________________________________________________________________________
void loadWWWTreeEvent()
{
  //mytree.LoadTree(LoopUtil::getCurrentTTreeEventIndex());
  mytree.GetEntry(LoopUtil::getCurrentTTreeEventIndex());
}

//______________________________________________________________________________________
void processWWWTreeEvent()
{

  // HT filter stitching for DY
  if (LoopUtil::output_name.Contains("dy_m50") && !LoopUtil::output_name.Contains("_ht"))
    if (mytree.gen_ht() > 100.)
      return;

  // HT filter stitching for wjets
  if (LoopUtil::output_name.Contains("wjets_incl"))
    if (mytree.gen_ht() > 100.)
      return;

  /// Set objects
  getObjects();

  /// selectObjects for analysis
  selectObjects();

  /// run analysis
  ana_data.leptons = ana_data.lepcol["goodSSlep"];
  doSMWWWSSmmAnalysis();
  doSMWWWSSemAnalysis();
  doSMWWWSSeeAnalysis();

  ana_data.leptons = ana_data.lepcol["good3Llep"];
  doSMWWW3L0SFOSAnalysis();
  doSMWWW3L1SFOSAnalysis();
  doSMWWW3L2SFOSAnalysis();

}

//______________________________________________________________________________________
void getObjects()
{
  /// Get objects
  ana_data.lepcol["goodSSlep"] = getLeptons();
  ana_data.lepcol["good3Llep"] = getLeptons();
  ana_data.jetcol["goodSSjet"] = getJets();
  ana_data.jetcol["good3Ljet"] = getJets();
  ana_data.jetcol["medbjet"] = getJets();
  ana_data.jetcol["lssbjet"] = getJets();
  ana_data.met = getMET();
  ana_data.wgt = mytree.evt_scale1fb();
}

//______________________________________________________________________________________
void afterLoop()
{
  // Save histograms
  PlotUtil::savePlots(ana_data.hist_db, (LoopUtil::output_name+"_hist.root").Data());

  // Save skimmmed tree
  if (LoopUtil::doskim) TreeUtil::saveSkimTree();

  // Fun exit
  PrintUtil::exit();
}

//______________________________________________________________________________________
void fillHistograms(string prefix)
{
  HistUtil::fillMET(prefix, ana_data);
  HistUtil::fillLepMTs(prefix, ana_data);
  HistUtil::fillLepSumPt(prefix, ana_data);
  HistUtil::fillLepRelIso03EA(prefix, ana_data);
  HistUtil::fillLepAbsIso03EA(prefix, ana_data);
}

//=====================================================================================
//=====================================================================================
//=====================================================================================
// Event selections
//=====================================================================================
//=====================================================================================
//=====================================================================================

//______________________________________________________________________________________
void doSMWWWSSmmAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWWSScommonselection(__FUNCTION__, 169, counter) )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 0);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  fillHistograms("SS");
  HistUtil::fillCounter("SMWWWAnalysis_SS", ana_data, 0);
  printEventList("SSmm");
}

//______________________________________________________________________________________
void doSMWWWSSeeAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWWSScommonselection(__FUNCTION__, 121, counter)                                )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) > 100.
                                                              || VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) <  80.)) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 55.                                                            )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 1);
  fillHistograms(__FUNCTION__);
  fillHistograms("SS");
  HistUtil::fillCounter("SMWWWAnalysis_SS", ana_data, 0);
  printEventList("SSee");
}

//______________________________________________________________________________________
void doSMWWWSSemAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWWSScommonselection(__FUNCTION__, 143, counter) )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 55.                             )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 2);
  fillHistograms(__FUNCTION__);
  fillHistograms("SS");
  HistUtil::fillCounter("SMWWWAnalysis_SS", ana_data, 0);
  printEventList("SSem");
}

//______________________________________________________________________________________
bool passSMWWWSScommonselection(string prefix, int pdgidprod, int& counter)
{
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"].size() == 2                                                    )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"][0].pdgId * ana_data.lepcol["goodSSlep"][1].pdgId == pdgidprod  )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"][0].p4.Pt() > 30.                                               )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"][1].p4.Pt() > 30.                                               )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["goodSSjet"].size() >= 2                                                    )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nlep() == 2                                                                          )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nisoTrack_mt2() == 0                                                                 )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) >  40.      )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.jetcol["goodSSjet"][0], ana_data.jetcol["goodSSjet"][1]) < 105.      )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.jetcol["goodSSjet"][0], ana_data.jetcol["goodSSjet"][1]) >  65.      )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::DEta(ana_data.jetcol["goodSSjet"][0], ana_data.jetcol["goodSSjet"][1]) < 1.5       )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["goodSSjet"][0].p4.Pt() > 30.                                               )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["lssbjet"].size() == 0                                                      )) return false;
  return true;
}

//______________________________________________________________________________________
void doSMWWW3L0SFOSAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 0, counter) )) return;

  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];

  float Mll = 0;
  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);

  float Mee = 0;
  if (abs(lep1.pdgId) == 11) Mee = Mll;

  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( Mll > 20.            )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mee - MZ) > 15. )) return;

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 3);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  printEventList("3L0SFOS");
}

//______________________________________________________________________________________
void doSMWWW3L1SFOSAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 1, counter) )) return;

  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];

  float Mll = 0;
  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);

  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( (MZ - Mll > 35.) || (Mll - MZ > 20.) )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 45.           )) return;

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 4);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  printEventList("3L1SFOS");
}

//______________________________________________________________________________________
void doSMWWW3L2SFOSAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 2, counter) )) return;

  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_pdgId<ObjUtil::Lepton>);

  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];

  float Mll0 = 0;
  float Mll1 = 0;
  if (lep1.pdgId > 0)
  {
    Mll0 = VarUtil::Mass(lep2, lep0);
    Mll1 = VarUtil::Mass(lep2, lep1);
  }
  else
  {
    Mll0 = VarUtil::Mass(lep0, lep1);
    Mll1 = VarUtil::Mass(lep0, lep2);
  }

  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mll0 - MZ) > 20.      )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mll1 - MZ) > 20.      )) return;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 55. )) return;

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 5);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  printEventList("3L2SFOS");
}

//______________________________________________________________________________________
bool passSMWWW3Lcommonselection(string prefix, int type, int& counter)
{
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["good3Llep"].size() == 3           )) return false;
  if      (type == 2) {                             if (!( is2SFOSEvent()                                     )) return false; }
  else if (type == 1) {                             if (!( is1SFOSEvent()                                     )) return false; }
  else if (type == 0) {                             if (!( is0SFOSEvent()                                     )) return false; }
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( fabs(VarUtil::DPhi(
                                                           ana_data.lepcol["good3Llep"][0].p4 +
                                                           ana_data.lepcol["good3Llep"][1].p4 +
                                                           ana_data.lepcol["good3Llep"][2].p4,
                                                           ana_data.met.p4)) > 2.5                            )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["good3Ljet"].size() <= 1           )) return false;
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["lssbjet"].size() == 0             )) return false;
  return true;
}

//=====================================================================================
//=====================================================================================
//=====================================================================================
// Object selections
//=====================================================================================
//=====================================================================================
//=====================================================================================

//______________________________________________________________________________________
void selectObjects()
{
  Analyses::selectObjs<ObjUtil::Lepton>(ana_data.lepcol["goodSSlep"] , isGoodSSLepton);
  Analyses::selectObjs<ObjUtil::Lepton>(ana_data.lepcol["good3Llep"] , isGood3LLepton);
  Analyses::selectObjs<ObjUtil::Jet>   (ana_data.jetcol["goodSSjet"] , isGoodSSJet);
  Analyses::selectObjs<ObjUtil::Jet>   (ana_data.jetcol["good3Ljet"] , isGood3LJet);
  Analyses::selectObjs<ObjUtil::Jet>   (ana_data.jetcol["medbjet"]   , isGoodWWWMediumBJet);
  Analyses::selectObjs<ObjUtil::Jet>   (ana_data.jetcol["lssbjet"]   , isGoodWWWLooseBJet);

  /// Sort by abs(pdgId) descending order
  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_abspdgId<ObjUtil::Lepton>);
}

//______________________________________________________________________________________
bool isGoodSSLepton(ObjUtil::Lepton& lepton)
{
  return isGoodSSElectron(lepton) || isGoodSSMuon(lepton);
}

//______________________________________________________________________________________
bool isGoodSSElectron(ObjUtil::Lepton& lepton)
{
  if (!( abs(lepton.pdgId) == 11      )) return false;
  if (!( lepton.p4.Pt() > 30.         )) return false;
  if (!( fabs(lepton.p4.Eta()) < 2.4  )) return false;
  if (!( lepton.relIso03EA < 0.1      )) return false;
  if (!( fabs(lepton.ip3d) < 0.015    )) return false;
  if (!( lepton.tightcharge == 2      )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGoodSSMuon(ObjUtil::Lepton& lepton)
{
  if (!( abs(lepton.pdgId) == 13      )) return false;
  if (!( lepton.p4.Pt() > 30.         )) return false;
  if (!( fabs(lepton.p4.Eta()) < 2.4  )) return false;
  if (!( lepton.relIso03EA < 0.1      )) return false;
  if (!( fabs(lepton.ip3d) < 0.015    )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGood3LLepton(ObjUtil::Lepton& lepton)
{
  return isGood3LElectron(lepton) || isGood3LMuon(lepton);
}

//______________________________________________________________________________________
bool isGood3LElectron(ObjUtil::Lepton& lepton)
{
  if (!( abs(lepton.pdgId) == 11      )) return false;
  if (!( lepton.p4.Pt() > 20.         )) return false;
  if (!( fabs(lepton.p4.Eta()) < 2.4  )) return false;
  if (!( lepton.relIso03EA < 0.1      )) return false;
  if (!( fabs(lepton.ip3d) < 0.015    )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGood3LMuon(ObjUtil::Lepton& lepton)
{
  if (!( abs(lepton.pdgId) == 13      )) return false;
  if (!( lepton.p4.Pt() > 20.         )) return false;
  if (!( fabs(lepton.p4.Eta()) < 2.4  )) return false;
  if (!( lepton.relIso03EA < 0.1      )) return false;
  if (!( fabs(lepton.ip3d) < 0.015    )) return false;
  return true;
}

//______________________________________________________________________________________
bool isVetoLepton(ObjUtil::Lepton& lepton)
{
  return isVetoElectron(lepton) || isVetoMuon(lepton);
}

//______________________________________________________________________________________
bool isVetoElectron(ObjUtil::Lepton& lepton)
{
  if (!( lepton.p4.Pt() > 10.        )) return false;
  if (!( fabs(lepton.p4.Eta()) < 2.4 )) return false;
  return true;
}

//______________________________________________________________________________________
bool isVetoMuon(ObjUtil::Lepton& lepton)
{
  if (!( lepton.p4.Pt() > 10.        )) return false;
  if (!( fabs(lepton.p4.Eta()) < 2.4 )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGoodSSJet(ObjUtil::Jet& jet)
{
  if (!( jet.p4.Pt() > 20.        )) return false;
  if (!( fabs(jet.p4.Eta()) < 2.5 )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGood3LJet(ObjUtil::Jet& jet)
{
  if (!( jet.p4.Pt() > 25.        )) return false;
  if (!( fabs(jet.p4.Eta()) < 4.5 )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGoodWWWMediumBJet(ObjUtil::Jet& jet)
{
  if (!( jet.p4.Pt() > 25.           )) return false;
  if (!( Analyses::isMediumBJet(jet) )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGoodWWWLooseBJet(ObjUtil::Jet& jet)
{
  if (!( jet.p4.Pt() > 20.          )) return false;
  if (!( Analyses::isLooseBJet(jet) )) return false;
  return true;
}

//______________________________________________________________________________________
bool is2SFOSEvent()
{
  if (!( (abs(ana_data.lepcol["good3Llep"][0].pdgId) == 11  &&
          abs(ana_data.lepcol["good3Llep"][1].pdgId) == 11  &&
          abs(ana_data.lepcol["good3Llep"][2].pdgId) == 11) ||
         (abs(ana_data.lepcol["good3Llep"][0].pdgId) == 13  &&
          abs(ana_data.lepcol["good3Llep"][1].pdgId) == 13  &&
          abs(ana_data.lepcol["good3Llep"][2].pdgId) == 13)    )) return false;
  return true;
}

//______________________________________________________________________________________
bool is1SFOSEvent()
{
  if (!( !is2SFOSEvent()                                       )) return false;
  if (!( (abs(ana_data.lepcol["good3Llep"][0].pdgId +
              ana_data.lepcol["good3Llep"][1].pdgId +
              ana_data.lepcol["good3Llep"][2].pdgId) == 11) ||
         (abs(ana_data.lepcol["good3Llep"][0].pdgId +
              ana_data.lepcol["good3Llep"][1].pdgId +
              ana_data.lepcol["good3Llep"][2].pdgId) == 13)    )) return false;
  return true;
}

//______________________________________________________________________________________
bool is0SFOSEvent()
{
  if (!( !is2SFOSEvent()                                       )) return false;
  if (!( !is1SFOSEvent()                                       )) return false;
  if (!( (abs(ana_data.lepcol["good3Llep"][0].pdgId +
              ana_data.lepcol["good3Llep"][1].pdgId +
              ana_data.lepcol["good3Llep"][2].pdgId) != 11) &&
         (abs(ana_data.lepcol["good3Llep"][0].pdgId +
              ana_data.lepcol["good3Llep"][1].pdgId +
              ana_data.lepcol["good3Llep"][2].pdgId) != 13)    )) return false;
  return true;
}

//______________________________________________________________________________________
void printEventList(string prefix)
{
  std::cout << prefix.c_str() << ": make_tuple(" << mytree.evt() << "," <<  mytree.run() << "," << mytree.lumi() << ")," << std::endl;
}

//eof