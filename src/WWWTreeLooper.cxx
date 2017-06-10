// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWTreeLooper.h"

#define MZ 91.1876

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

  // Load event list to check
  //LoopUtil::loadEventListToCheck();

  // Load event selections
  loadEventSelections();

}


//______________________________________________________________________________________
void loop()
{

  // Loop over file:ttree -> and loop over events in ttree
  // (usually I only have one file each.. so kinda redundant)
  while (LoopUtil::nextFileAndLoadTTreeWithName("t"))
  {

    initWWWTree();

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
}

//______________________________________________________________________________________
void loadWWWTreeEvent()
{
  mytree.GetEntry(LoopUtil::getCurrentTTreeEventIndex());
}

//______________________________________________________________________________________
void processWWWTreeEvent()
{
  // On following two occassions, we need to filter out some events based on truth info.
  //  1. W/Zjets has HT stitching.
  //  2. VHNonbb sample needs to be filtered to accept only WHWWW
  if (!passGenLevelEventFilter())
    return;

  // Get objects
  getObjects();

  // selectObjects for analysis
  selectObjects();

  // run analysis
  runSignalRegions();

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


//______________________________________________________________________________________
void runSignalRegions()
{
  ana_data.leptons = ana_data.lepcol["goodSSlep"];
  ana_data.jets    = ana_data.jetcol["goodSSjet"];
  if (passSMWWWSSmmAnalysis()) fillHistograms("SSmm");
  if (passSMWWWSSemAnalysis()) fillHistograms("SSem");
  if (passSMWWWSSeeAnalysis()) fillHistograms("SSee");

//	  ana_data.leptons = ana_data.lepcol["good3Llep"];
//	  ana_data.jets    = ana_data.jetcol["good3Ljet"];
//	  doSMWWW3L0SFOSAnalysis();
//	  doSMWWW3L1SFOSAnalysis();
//	  doSMWWW3L2SFOSAnalysis();
}


//`````````````````````````````````````````````````````````````````````````````````````
// Same sign selections
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

//______________________________________________________________________________________
bool passSMWWWSSmmAnalysis()
{
  return CutUtil::pass(ssmm_cuts);
}

//______________________________________________________________________________________
bool passSMWWWSSemAnalysis()
{
  return CutUtil::pass(ssem_cuts);
}

//______________________________________________________________________________________
bool passSMWWWSSeeAnalysis()
{
  return CutUtil::pass(ssee_cuts);
}







//eof

//	//	//______________________________________________________________________________________
//	//	bool passSSNLepCut(float& val, string& cutname, std::vector<int>&, int)
//	//	{
//	//	  cutname = "SSNLepCut";
//	//	  HistUtil::fillCutflow(prefix, ana_data, counter);
//	//	  if (!( ana_data.lepcol["goodSSlep"].size() == 2 ))
//	//	    return failed(__LINE__);
//	//	}
//	//	
//	//	//______________________________________________________________________________________
//	//	bool passSSChannel   (float&, string& cutname, std::vector<int>&, int)
//	//	{
//	//	  HistUtil::fillCutflow(prefix, ana_data, counter);
//	//	  if (!( ana_data.lepcol["goodSSlep"][0].pdgId * ana_data.lepcol["goodSSlep"][1].pdgId == pdgidprod  ))
//	//	    return failed(__LINE__);
//	//	}
//	//	
//	//	bool passSSLep0PtCut (float&, string& cutname, std::vector<int>&, float);
//	//	bool passSSLep1PtCut (float&, string& cutname, std::vector<int>&, float);
//	//	bool passSSNJetCut   (float&, string& cutname, std::vector<int>&, int);
//	//	bool passSSJet0PtCut (float&, string& cutname, std::vector<int>&, float);
//	//	bool passSS3rdLepVeto(float&, string& cutname, std::vector<int>&);
//	//	bool passSSLowMllCut (float&, string& cutname, std::vector<int>&, float);
//	//	bool passSSWMassCut  (float&, string& cutname, std::vector<int>&, float, float);
//	//	bool passSSMjjCut    (float&, string& cutname, std::vector<int>&, float);
//	//	bool passSSDEtajjCut (float&, string& cutname, std::vector<int>&, float);
//	//	bool passSSBVeto     (float&, string& cutname, std::vector<int>&, string);
//	
//	
//	
//	
//	
//	//`````````````````````````````````````````````````````````````````````````````````````
//	// 3 Lepton regions
//	//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//	
//	//______________________________________________________________________________________
//	bool passSMWWW3Lcommonselection(string prefix, int type, int& counter)
//	{
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["good3Llep"].size() == 3           )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nlep() == 3                                 )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( totalcharge() == 1                                 )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( getNumSFOSPairs() == type                          )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( fabs(VarUtil::DPhi(
//	                                                           ana_data.lepcol["good3Llep"][0].p4 +
//	                                                           ana_data.lepcol["good3Llep"][1].p4 +
//	                                                           ana_data.lepcol["good3Llep"][2].p4,
//	                                                           ana_data.met.p4)) > 2.5                            )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["good3Ljet"].size() <= 1           )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["lssbjet"].size() == 0             )) return failed(__LINE__);
//	  return true;
//	}
//	
//	//______________________________________________________________________________________
//	bool doSMWWW3L0SFOSAnalysis()
//	{
//	  /// Cutflow
//	  int counter = 0;
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 0, counter) )) return failed(__LINE__);
//	
//	  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
//	  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
//	  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];
//	
//	  float Mll = 0;
//	  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
//	  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);
//	
//	  float Mee = 0;
//	  if (abs(lep1.pdgId) == 11) Mee = Mll;
//	
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( Mll > 20.            )) return failed(__LINE__);
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mee - MZ) > 15. )) return failed(__LINE__);
//	
//	  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 3);
//	  /// Select object containers for plotting
//	  fillHistograms(__FUNCTION__);
//	  fillHistogramsTruthMatchingLeptons3L(__FUNCTION__);
//	  printEventList("3L0SFOS");
//	  return true;
//	}
//	
//	//______________________________________________________________________________________
//	bool doSMWWW3L1SFOSAnalysis()
//	{
//	  /// Cutflow
//	  int counter = 0;
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 1, counter) )) return failed(__LINE__);
//	
//	  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
//	  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
//	  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];
//	
//	  float Mll = 0;
//	  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
//	  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);
//	
//	  std::cout << __LINE__ << " " << Mll << std::endl;
//	
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( (MZ - Mll > 35.) || (Mll - MZ > 20.) )) return failed(__LINE__);
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 45.           )) return failed(__LINE__);
//	
//	  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 4);
//	  /// Select object containers for plotting
//	  fillHistograms(__FUNCTION__);
//	  printEventList("3L1SFOS");
//	  return true;
//	}
//	
//	//______________________________________________________________________________________
//	bool doSMWWW3L2SFOSAnalysis()
//	{
//	  /// Cutflow
//	  int counter = 0;
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( passSMWWW3Lcommonselection(__FUNCTION__, 2, counter) )) return failed(__LINE__);
//	
//	  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_pdgId<ObjUtil::Lepton>);
//	
//	  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
//	  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
//	  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];
//	
//	  float Mll0 = 0;
//	  float Mll1 = 0;
//	  if (lep1.pdgId > 0)
//	  {
//	    Mll0 = VarUtil::Mass(lep2, lep0);
//	    Mll1 = VarUtil::Mass(lep2, lep1);
//	  }
//	  else
//	  {
//	    Mll0 = VarUtil::Mass(lep0, lep1);
//	    Mll1 = VarUtil::Mass(lep0, lep2);
//	  }
//	
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mll0 - MZ) > 20.      )) return failed(__LINE__);
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( fabs(Mll1 - MZ) > 20.      )) return failed(__LINE__);
//	  HistUtil::fillCutflow(__FUNCTION__, ana_data, counter); if (!( ana_data.met.p4.Pt() > 55. )) return failed(__LINE__);
//	
//	  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, 5);
//	  /// Select object containers for plotting
//	  fillHistograms(__FUNCTION__);
//	  printEventList("3L2SFOS");
//	  return true;
//	}
//	
//	
//	
//	
//	
//	
//	
//	
//	
