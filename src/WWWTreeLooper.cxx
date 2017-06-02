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

  // load event list to check
  //LoopUtil::loadEventListToCheck();

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

  // Set objects
  getObjects();

  // Mia sample reweighting
  if (LoopUtil::output_name.Contains("www_2l_ext1_mia_skim_1")) ana_data.wgt *= 0.066805*164800./(91900.+164800.);
  if (LoopUtil::output_name.Contains("www_2l_mia_skim_1"))      ana_data.wgt *= 0.066805*91900./(91900.+164800.);

  /// selectObjects for analysis
  selectObjects();

  /// run analysis
  ana_data.leptons = ana_data.lepcol["goodSSlep"];
  ana_data.jets = ana_data.jetcol["goodSSjet"];
  doSMWWWSSmmAnalysis();
  doSMWWWSSemAnalysis();
  doSMWWWSSeeAnalysis();

  ana_data.leptons = ana_data.lepcol["good3Llep"];
  ana_data.jets = ana_data.jetcol["good3Ljet"];
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
  ana_data.jetcol["rmvdjet"] = getRemovedJets();
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

//=====================================================================================
//=====================================================================================
//=====================================================================================
// Event selections
//=====================================================================================
//=====================================================================================
//=====================================================================================


//`````````````````````````````````````````````````````````````````````````````````````
// Same sign selections
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

//______________________________________________________________________________________
bool passSMWWWSScommonselection(string prefix, int pdgidprod, int& counter)
{
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"].size() == 2                                                    )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"][0].pdgId * ana_data.lepcol["goodSSlep"][1].pdgId == pdgidprod  )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["goodSSjet"].size() >= 2                                                    )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["goodSSjet"][0].p4.Pt() > 30.                                               )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nlep() == 2                                                                          )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nisoTrack_mt2() == 0                                                                 )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) >  40.      )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::MjjClosest(ana_data) < 100.                                                        )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::MjjClosest(ana_data) >  60.                                                        )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.jetcol["goodSSjet"][0], ana_data.jetcol["goodSSjet"][1]) < 400.      )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::DEta(ana_data.jetcol["goodSSjet"][0], ana_data.jetcol["goodSSjet"][1]) < 1.5       )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["lssbjet"].size() == 0                                                      )) return failed(__LINE__);
  return true;
}

//______________________________________________________________________________________
bool doSMWWWSSmmAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWWSScommonselection(__FUNCTION__, 169, counter) )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 0);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  fillHistograms("SS");
  HistUtil::fillCounter("SMWWWAnalysis_SS", ana_data, 0);
  printEventList("SSmm");
  return true;
}

//______________________________________________________________________________________
bool doSMWWWSSemAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWWSScommonselection(__FUNCTION__, 143, counter) )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 40.                             )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 1);
  fillHistograms(__FUNCTION__);
  fillHistograms("SS");
  HistUtil::fillCounter("SMWWWAnalysis_SS", ana_data, 0);
  printEventList("SSem");
  return true;
}

//______________________________________________________________________________________
bool doSMWWWSSeeAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWWSScommonselection(__FUNCTION__, 121, counter)                                )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) > 100.
                                                              || VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) <  80.)) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 40.                                                            )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 2);
  fillHistograms(__FUNCTION__);
  fillHistograms("SS");
  HistUtil::fillCounter("SMWWWAnalysis_SS", ana_data, 0);
  printEventList("SSee");
  return true;
}



//`````````````````````````````````````````````````````````````````````````````````````
// Same sign selections
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

//______________________________________________________________________________________
bool passSMWWW3Lcommonselection(string prefix, int type, int& counter)
{
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["good3Llep"].size() == 3           )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nlep() == 3                                 )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( totalcharge() == 1                                 )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( getNumSFOSPairs() == type                          )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( fabs(VarUtil::DPhi(
                                                           ana_data.lepcol["good3Llep"][0].p4 +
                                                           ana_data.lepcol["good3Llep"][1].p4 +
                                                           ana_data.lepcol["good3Llep"][2].p4,
                                                           ana_data.met.p4)) > 2.5                            )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["good3Ljet"].size() <= 1           )) return failed(__LINE__);
  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["lssbjet"].size() == 0             )) return failed(__LINE__);
  return true;
}

//______________________________________________________________________________________
bool doSMWWW3L0SFOSAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 0, counter) )) return failed(__LINE__);

  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];

  float Mll = 0;
  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);

  float Mee = 0;
  if (abs(lep1.pdgId) == 11) Mee = Mll;

  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( Mll > 20.            )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mee - MZ) > 15. )) return failed(__LINE__);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 3);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  fillHistogramsTruthMatchingLeptons3L(__FUNCTION__);
  printEventList("3L0SFOS");
  return true;
}

//______________________________________________________________________________________
bool doSMWWW3L1SFOSAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 1, counter) )) return failed(__LINE__);

  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];

  float Mll = 0;
  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);

  std::cout << __LINE__ << " " << Mll << std::endl;

  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( (MZ - Mll > 35.) || (Mll - MZ > 20.) )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 45.           )) return failed(__LINE__);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 4);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  printEventList("3L1SFOS");
  return true;
}

//______________________________________________________________________________________
bool doSMWWW3L2SFOSAnalysis()
{
  /// Cutflow
  int counter = 0;
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 2, counter) )) return failed(__LINE__);

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

  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mll0 - MZ) > 20.      )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mll1 - MZ) > 20.      )) return failed(__LINE__);
  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 55. )) return failed(__LINE__);

  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 5);
  /// Select object containers for plotting
  fillHistograms(__FUNCTION__);
  printEventList("3L2SFOS");
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
  if (!( abs(lepton.pdgId) == 11      )) return failed(__LINE__);
  if (!( lepton.p4.Pt() > 30.         )) return failed(__LINE__);
  if (!( fabs(lepton.p4.Eta()) < 2.4  )) return failed(__LINE__);
  if (!( lepton.relIso03EA < 0.06     )) return failed(__LINE__);
  if (!( fabs(lepton.ip3d) < 0.015    )) return failed(__LINE__);
  if (!( lepton.tightcharge != 0      )) return failed(__LINE__);
  return true;
}

//______________________________________________________________________________________
bool isGoodSSMuon(ObjUtil::Lepton& lepton)
{
  if (!( abs(lepton.pdgId) == 13      )) return failed(__LINE__);
  if (!( lepton.p4.Pt() > 30.         )) return failed(__LINE__);
  if (!( fabs(lepton.p4.Eta()) < 2.4  )) return failed(__LINE__);
  if (!( lepton.relIso03EA < 0.06     )) return failed(__LINE__);
  if (!( fabs(lepton.ip3d) < 0.015    )) return failed(__LINE__);
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
  if (!( jet.p4.Pt() > 30.        )) return false;
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

//=====================================================================================
//=====================================================================================
//=====================================================================================
// Utility functions
//=====================================================================================
//=====================================================================================
//=====================================================================================

//______________________________________________________________________________________
int totalcharge()
{
  return abs(mytree.lep_pdgId().at(0)/abs(mytree.lep_pdgId().at(0)) +
             mytree.lep_pdgId().at(1)/abs(mytree.lep_pdgId().at(1)) +
             mytree.lep_pdgId().at(2)/abs(mytree.lep_pdgId().at(2)));
}

//______________________________________________________________________________________
int getNumSFOSPairs(){
  /*Loops through pairs of entries in the lep_pdgId vector and counts how many have opposite value*/
  int num_SFOS = 0;
  for (int i = 0; i < (int) mytree.lep_pdgId().size(); i++){
    for (int j = i+1; j < (int) mytree.lep_pdgId().size(); j++){
      if (mytree.lep_pdgId().at(i) == -mytree.lep_pdgId().at(j)) num_SFOS++;
    }
  }
  return num_SFOS;
}

//______________________________________________________________________________________
void printEventList(string prefix)
{
  std::cout << prefix.c_str() << ": make_tuple(" << mytree.evt() << "," <<  mytree.run() << "," << mytree.lumi() << ")," << std::endl;
  std::cout << prefix.c_str() << "HJ: " << mytree.run() << ":" <<  mytree.lumi() << ":" << mytree.evt() << std::endl;
}

//______________________________________________________________________________________
bool failed(float cutid)
{
  std::vector<int> eventid;
  eventid.push_back(mytree.run());
  eventid.push_back(mytree.lumi());
  eventid.push_back(mytree.evt());
  return LoopUtil::failed(eventid, cutid);
}

//=====================================================================================
//=====================================================================================
//=====================================================================================
// Histogram functions
//=====================================================================================
//=====================================================================================
//=====================================================================================

//______________________________________________________________________________________
void fillHistograms(string prefix)
{
  HistUtil::fillMET(prefix, ana_data);
  HistUtil::fillMjj(prefix, ana_data);
  HistUtil::fillMjjWithMaxDEtajj(prefix, ana_data);
  HistUtil::fillLepMTs(prefix, ana_data);
  HistUtil::fillLepMlvjs(prefix, ana_data);
  HistUtil::fillLepSumPt(prefix, ana_data);
  HistUtil::fillLepRelIso03EA(prefix, ana_data);
  HistUtil::fillLepAbsIso03EA(prefix, ana_data);
  HistUtil::fillLepRelIso04EA(prefix, ana_data);
  HistUtil::fillLepAbsIso04EA(prefix, ana_data);
  HistUtil::fillLepIP(prefix, ana_data);
  HistUtil::fillLepTightCharge(prefix, ana_data);
  HistUtil::fillLepNeutrinoNSol(prefix, ana_data);
}

//______________________________________________________________________________________
void fillHistogramsTruthMatchingLeptons3L(string prefix)
{
  /// Lepton truth matching category information
  /// Using isFromX fill the category

  // If we don't have two leptons exit
  if (ana_data.lepcol["goodSSlep"].size() < 3)
    return;

  // Read the three leptons
  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];

  // Fill histogram based on truth matching
  HistUtil::fillLepIsFromX(prefix, ana_data);

  // Counting number of prompt vs. fake
  int prompt = 0;
  if (lep0.isFromX == (1 << 0) || lep0.isFromX == (1 << 1)) prompt++;
  if (lep1.isFromX == (1 << 0) || lep1.isFromX == (1 << 1)) prompt++;
  if (lep2.isFromX == (1 << 0) || lep2.isFromX == (1 << 1)) prompt++;

  PlotUtil::plot1D("truth3L_nlep_isFromW", prompt, ana_data.wgt, ana_data.hist_db, "", 4, 0., 4., prefix);
}




//======================================================================================
//======================================================================================
//======================================================================================
//======================================================================================
//======================================================================================
//======================================================================================



//______________________________________________________________________________________
void fillHistogramsTruthMatchingLeptons(string prefix)
{
  /// Lepton truth matching category information
  /// Using isFromX fill the category

  // If we don't have two leptons exit
  if (ana_data.lepcol["goodSSlep"].size() != 2) return;

  ObjUtil::Lepton lep0 = ana_data.lepcol["goodSSlep"][0];
  ObjUtil::Lepton lep1 = ana_data.lepcol["goodSSlep"][1];

  // Leading lepton
  /* 1 : isFromW */        if (lep0.isFromX == 1                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 1, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 2 : isFromZ */        if (lep0.isFromX == 2                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 2, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 3 : isFromW/Z */      if (lep0.isFromX == 1 || lep0.isFromX == 2) PlotUtil::plot1D("leptruthcategorySS_lep0", 3, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 4 : isFromB/C/L/LF */ if (lep0.isFromX >= 4                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 4, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 5 : isFromB/C */      if (lep0.isFromX == 4 || lep0.isFromX == 8) PlotUtil::plot1D("leptruthcategorySS_lep0", 5, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 0 : not matched */    if (lep0.isFromX == 0                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 0, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);

  // Sub-leading lepton
  /* 1 : isFromW */        if (lep1.isFromX == 1                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 1, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 2 : isFromZ */        if (lep1.isFromX == 2                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 2, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 3 : isFromW/Z */      if (lep1.isFromX == 1 || lep1.isFromX == 2) PlotUtil::plot1D("leptruthcategorySS_lep1", 3, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 4 : isFromB/C/L/LF */ if (lep1.isFromX >= 4                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 4, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 5 : isFromB/C */      if (lep1.isFromX == 4 || lep1.isFromX == 8) PlotUtil::plot1D("leptruthcategorySS_lep1", 5, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
  /* 0 : not matched */    if (lep1.isFromX == 0                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 0, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);

  // event category
  /* 1 : both are from W */if ( lep0.isFromX == 1 && lep1.isFromX == 1)  PlotUtil::plot1D("leptruthcategorySS"   , 0, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
  /* 2 : one is from W   */if ((lep0.isFromX == 1 && lep1.isFromX != 1)||
                               (lep1.isFromX == 1 && lep0.isFromX != 1)) PlotUtil::plot1D("leptruthcategorySS"   , 1, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
  /* 3 : none are from W */if ( lep0.isFromX != 1 && lep1.isFromX != 1)  PlotUtil::plot1D("leptruthcategorySS"   , 2, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);

  if ((lep0.isFromX == 1 && lep1.isFromX != 1) || (lep1.isFromX == 1 && lep0.isFromX != 1))
  {
    if (lep0.isFromX == 1)
    {
      if (lep1.isFromX >= 4)
      {
        PlotUtil::plot1D("leptruthcategorySS_oneW", 0, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
        ObjUtil::Jets removedjets = getRemovedJets();
        float mindr = -999;
        float csv = -999;
        for (auto& jet : removedjets)
        {
          float tmpdr = VarUtil::DR(lep1, jet);
          if (mindr < 0 || mindr > tmpdr)
          {
            mindr = tmpdr;
            csv = jet.btagCSV;
          }
        }
        PlotUtil::plot1D("leptruthcategorySS_oneW_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso03EA" , lep1.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso04EA" , lep1.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_ip3d"       , lep1.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_sip3d"      , lep1.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);

        mindr = -999;
        csv = -999;
        for (auto& jet : removedjets)
        {
          float tmpdr = VarUtil::DR(lep0, jet);
          if (mindr < 0 || mindr > tmpdr)
          {
            mindr = tmpdr;
            csv = jet.btagCSV;
          }
        }
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso03EA" , lep0.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso03EA" , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso04EA" , lep0.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_ip3d"       , lep0.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_sip3d"      , lep0.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);

      }
      else if (lep1.isFromX == 2)
      {
        PlotUtil::plot1D("leptruthcategorySS_oneW", 1, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
      }
      else
      {
        PlotUtil::plot1D("leptruthcategorySS_oneW", 2, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
        PlotUtil::plot1D("ngentau", mytree.ngenTau(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
        PlotUtil::plot1D("nTaus20", mytree.nTaus20(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_reliso03EA" , lep1.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_reliso04EA" , lep1.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_ip3d"       , lep1.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_sip3d"      , lep1.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_pt"         , lep1.p4.Pt()                 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_eta"        , lep1.p4.Eta()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
        PlotUtil::plot1D("misidlep1isfromnothing_phi"        , lep1.p4.Phi()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_reliso03EA" , lep1.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_reliso04EA" , lep1.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_ip3d"       , lep1.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("misidlepisfromnothing_sip3d"      , lep1.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_pt"         , lep1.p4.Pt()                 , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_eta"        , lep1.p4.Eta()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_phi"        , lep1.p4.Phi()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
      }
    }
    else if (lep1.isFromX == 1)
    {
      if (lep0.isFromX >= 4)
      {
        PlotUtil::plot1D("leptruthcategorySS_oneW", 0, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
        ObjUtil::Jets removedjets = getRemovedJets();
        float mindr = -999;
        float csv = -999;
        for (auto& jet : removedjets)
        {
          float tmpdr = VarUtil::DR(lep1, jet);
          if (mindr < 0 || mindr > tmpdr)
          {
            mindr = tmpdr;
            csv = jet.btagCSV;
          }
        }
        PlotUtil::plot1D("leptruthcategorySS_oneW_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso03EA" , lep0.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso03EA" , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso04EA" , lep0.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_ip3d"       , lep0.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_sip3d"      , lep0.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);

        mindr = -999;
        csv = -999;
        for (auto& jet : removedjets)
        {
          float tmpdr = VarUtil::DR(lep1, jet);
          if (mindr < 0 || mindr > tmpdr)
          {
            mindr = tmpdr;
            csv = jet.btagCSV;
          }
        }
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso03EA" , lep1.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso04EA" , lep1.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_ip3d"       , lep1.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_sip3d"      , lep1.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
      }
      else if (lep0.isFromX == 2)
      {
        PlotUtil::plot1D("leptruthcategorySS_oneW", 1, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
      }
      else
      {
        PlotUtil::plot1D("leptruthcategorySS_oneW", 2, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
        PlotUtil::plot1D("ngentau", mytree.ngenTau(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
        PlotUtil::plot1D("nTaus20", mytree.nTaus20(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_reliso03EA" , lep0.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_absiso03EA" , lep0.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_reliso04EA" , lep0.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_ip3d"       , lep0.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_sip3d"      , lep0.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_pt"         , lep0.p4.Pt()                 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_eta"        , lep0.p4.Eta()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
        PlotUtil::plot1D("misidlep0isfromnothing_phi"        , lep0.p4.Phi()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_reliso03EA" , lep0.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_absiso03EA" , lep0.relIso03EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_reliso04EA" , lep0.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_ip3d"       , lep0.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
        PlotUtil::plot1D("misidlepisfromnothing_sip3d"      , lep0.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_pt"         , lep0.p4.Pt()                 , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_eta"        , lep0.p4.Eta()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
        PlotUtil::plot1D("misidlepisfromnothing_phi"        , lep0.p4.Phi()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
      }
    }
  }
  else if (lep0.isFromX != 1 && lep1.isFromX != 1)
  {
    if (lep0.isFromX == (1<<1)) PlotUtil::plot1D("lep0misid_mcmatch"    , 0 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<2)) PlotUtil::plot1D("lep0misid_mcmatch"    , 1 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<3)) PlotUtil::plot1D("lep0misid_mcmatch"    , 2 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<4)) PlotUtil::plot1D("lep0misid_mcmatch"    , 3 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<5)) PlotUtil::plot1D("lep0misid_mcmatch"    , 4 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<1)) PlotUtil::plot1D("lep1misid_mcmatch"    , 0 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<2)) PlotUtil::plot1D("lep1misid_mcmatch"    , 1 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<3)) PlotUtil::plot1D("lep1misid_mcmatch"    , 2 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<4)) PlotUtil::plot1D("lep1misid_mcmatch"    , 3 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<5)) PlotUtil::plot1D("lep1misid_mcmatch"    , 4 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<1)) PlotUtil::plot1D("lepmisid_mcmatch"    , 0 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<2)) PlotUtil::plot1D("lepmisid_mcmatch"    , 1 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<3)) PlotUtil::plot1D("lepmisid_mcmatch"    , 2 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<4)) PlotUtil::plot1D("lepmisid_mcmatch"    , 3 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep0.isFromX == (1<<5)) PlotUtil::plot1D("lepmisid_mcmatch"    , 4 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<1)) PlotUtil::plot1D("lepmisid_mcmatch"    , 0 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<2)) PlotUtil::plot1D("lepmisid_mcmatch"    , 1 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<3)) PlotUtil::plot1D("lepmisid_mcmatch"    , 2 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<4)) PlotUtil::plot1D("lepmisid_mcmatch"    , 3 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
    if (lep1.isFromX == (1<<5)) PlotUtil::plot1D("lepmisid_mcmatch"    , 4 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);

    PlotUtil::plot1D("lep0misid_reliso03EA" , lep0.relIso03EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lep0misid_absiso03EA" , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lep0misid_reliso04EA" , lep0.relIso04EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lep0misid_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lep0misid_ip3d"       , lep0.ip3d                    , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
    PlotUtil::plot1D("lep0misid_sip3d"      , lep0.sip3d                   , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
    PlotUtil::plot1D("lep1misid_reliso03EA" , lep1.relIso03EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lep1misid_absiso03EA" , lep1.relIso03EA*lep0.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lep1misid_reliso04EA" , lep1.relIso04EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lep1misid_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lep1misid_ip3d"       , lep1.ip3d                    , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
    PlotUtil::plot1D("lep1misid_sip3d"      , lep1.sip3d                   , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
    PlotUtil::plot1D("lepmisid_reliso03EA"  , lep0.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lepmisid_absiso03EA"  , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lepmisid_reliso04EA"  , lep0.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lepmisid_absiso04EA"  , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lepmisid_ip3d"        , lep0.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
    PlotUtil::plot1D("lepmisid_sip3d"       , lep0.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
    PlotUtil::plot1D("lepmisid_reliso03EA"  , lep1.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lepmisid_absiso03EA"  , lep1.relIso03EA*lep0.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lepmisid_reliso04EA"  , lep1.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
    PlotUtil::plot1D("lepmisid_absiso04EA"  , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
    PlotUtil::plot1D("lepmisid_ip3d"        , lep1.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
    PlotUtil::plot1D("lepmisid_sip3d"       , lep1.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
  }
  else
  {
    PlotUtil::plot1D("leptruthcategorySS_oneW_csv"                , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.    , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_dr"                 , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.    , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW"                    , -1   , ana_data.wgt , ana_data.hist_db , "" , 3     , 0.  , 3.    , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_dr"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.    , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_csv"        , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.    , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso03EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso03EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_reliso03EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_absiso03EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_reliso04EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_absiso04EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_ip3d"               , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015 , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_sip3d"              , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.    , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso04EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso04EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_ip3d"       , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015 , prefix);
    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_sip3d"      , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.    , prefix);
  }
}

//eof
