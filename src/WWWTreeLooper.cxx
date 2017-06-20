// vim: tabstop=4:softtabstop=4:shiftwidth=4:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWTreeLooper.h"

#define MZ 91.1876

//______________________________________________________________________________________
int WWWTreeLooper( TChain* chain, TString output_name, int nevents )
{

    // Before the loop set up some configuration, histograms and etc.
    beforeLoop( chain, output_name, nevents );

    // During the event loop process each event one by one
    loop();

    // After the loop is done, save histograms and etc.
    afterLoop();

    return 0;
}

//______________________________________________________________________________________
void beforeLoop( TChain* chain, TString output_name_, int nevents )
{

    // Fun start ASCII art
    PrintUtil::start();

    // Initialize configurations for event looping
    LoopUtil::resetLoopCondition( chain, nevents );

    // Replace all ".root" added accidentally
    output_name_.ReplaceAll( ".root", "" );
    LoopUtil::output_name = output_name_;

    // Load event list to check
    CutUtil::loadEventListToCheck();
    CutUtil::loadEventListToVerboseCheck();

    // Load event selections
    loadEventSelections();

}


//______________________________________________________________________________________
void loop()
{

    // Loop over file:ttree -> and loop over events in ttree
    // (usually I only have one file each.. so kinda redundant)
    while ( LoopUtil::nextFileAndLoadTTreeWithName( "t" ) )
    {

        initWWWTree();

        // Loop over the TTree
        while ( LoopUtil::nextEvent() )
        {

            if ( LoopUtil::isAllEventsProcessed() ) break;

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
    mytree.Init( LoopUtil::getCurrentTTree() );
}

//______________________________________________________________________________________
void loadWWWTreeEvent()
{
    mytree.GetEntry( LoopUtil::getCurrentTTreeEventIndex() );
}

//______________________________________________________________________________________
void processWWWTreeEvent()
{
    // On following two occassions, we need to filter out some events based on truth info.
    //  1. W/Zjets has HT stitching.
    //  2. VHNonbb sample needs to be filtered to accept only WHWWW
    if ( !passGenLevelEventFilter() )
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
    if ( getBabyVersion() >= 6 )
    {
        if ( isSignalSample() )
        {
            PlotUtil::savePlots( ana_data_SS_true.hist_db, ( LoopUtil::output_name + "_SS_hist.root" ).Data() );
            PlotUtil::savePlots( ana_data_3L_true.hist_db, ( LoopUtil::output_name + "_3L_hist.root" ).Data() );
        }
        else
        {
            // Save based on bkg categories
            PlotUtil::savePlots ( ana_data_SS_true.hist_db,    ( LoopUtil::output_name + "_SS_true_"    + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_SS_chflip.hist_db,  ( LoopUtil::output_name + "_SS_chflip_"  + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_SS_lostlep.hist_db, ( LoopUtil::output_name + "_SS_lostlep_" + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_SS_onefake.hist_db, ( LoopUtil::output_name + "_SS_onefake_" + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_SS_twofake.hist_db, ( LoopUtil::output_name + "_SS_twofake_" + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_SS_others.hist_db,  ( LoopUtil::output_name + "_SS_others_"  + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_3L_true.hist_db,    ( LoopUtil::output_name + "_3L_true_"    + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_3L_chflip.hist_db,  ( LoopUtil::output_name + "_3L_chflip_"  + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_3L_lostlep.hist_db, ( LoopUtil::output_name + "_3L_lostlep_" + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_3L_onefake.hist_db, ( LoopUtil::output_name + "_3L_onefake_" + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_3L_twofake.hist_db, ( LoopUtil::output_name + "_3L_twofake_" + "_hist.root" ).Data ( ) );
            PlotUtil::savePlots ( ana_data_3L_others.hist_db,  ( LoopUtil::output_name + "_3L_others_"  + "_hist.root" ).Data ( ) );
        }
    }
    else
    {
        PlotUtil::savePlots( ana_data.hist_db, ( LoopUtil::output_name+"_hist.root" ).Data() );
    }

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
    if ( passSMWWWSSmmAnalysis() ) fillHistograms( "SSmm" );
    if ( passSMWWWSSemAnalysis() ) fillHistograms( "SSem" );
    if ( passSMWWWSSeeAnalysis() ) fillHistograms( "SSee" );

    if ( passSMWWWSSWZCRmmAnalysis() ) { fillHistograms( "SSWZCRmm" ); fillHistograms( "SSWZCR" ); }
    if ( passSMWWWSSWZCRemAnalysis() ) { fillHistograms( "SSWZCRem" ); fillHistograms( "SSWZCR" ); }
    if ( passSMWWWSSWZCReeAnalysis() ) { fillHistograms( "SSWZCRee" ); fillHistograms( "SSWZCR" ); }

    ana_data.leptons = ana_data.lepcol["good3Llep"];
    ana_data.jets    = ana_data.jetcol["good3Ljet"];
    if ( passSMWWW3L0SFOSAnalysis() ) fillHistograms( "ThreeLep0SFOS" );
    if ( passSMWWW3L1SFOSAnalysis() ) fillHistograms( "ThreeLep1SFOS" );
    if ( passSMWWW3L2SFOSAnalysis() ) fillHistograms( "ThreeLep2SFOS" );
}


//`````````````````````````````````````````````````````````````````````````````````````
// Same sign selections
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

//______________________________________________________________________________________
bool passSMWWWSSmmAnalysis()
{
    return CutUtil::pass( ssmm_cuts, "SSmm", ( *getAnalysisData( "SSmm" ) ) );
}

//______________________________________________________________________________________
bool passSMWWWSSemAnalysis()
{
    return CutUtil::pass( ssem_cuts, "SSem", ( *getAnalysisData( "SSem" ) ) );
}

//______________________________________________________________________________________
bool passSMWWWSSeeAnalysis()
{
    return CutUtil::pass( ssee_cuts, "SSee", ( *getAnalysisData( "SSee" ) ) );
}

//`````````````````````````````````````````````````````````````````````````````````````
// tri-lepton region
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

//______________________________________________________________________________________
bool passSMWWW3L0SFOSAnalysis()
{
    return CutUtil::pass( threelep0_cuts, "ThreeLep0SFOS", ( *getAnalysisData( "ThreeLep0SFOS" ) ) );
}

//______________________________________________________________________________________
bool passSMWWW3L1SFOSAnalysis()
{
    return CutUtil::pass( threelep1_cuts, "ThreeLep1SFOS", ( *getAnalysisData( "ThreeLep1SFOS" ) ) );
}

//______________________________________________________________________________________
bool passSMWWW3L2SFOSAnalysis()
{
    return CutUtil::pass( threelep2_cuts, "ThreeLep2SFOS", ( *getAnalysisData( "ThreeLep2SFOS" ) ) );
}

//`````````````````````````````````````````````````````````````````````````````````````
// SS WZ CR region (really a three lepton region with 2 jets)
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

//______________________________________________________________________________________
bool passSMWWWSSWZCRmmAnalysis()
{
    return CutUtil::pass( sswzcrmm_cuts, "SSWZCRmm", ( *getAnalysisData( "SSWZCRmm" ) ) );
}

//______________________________________________________________________________________
bool passSMWWWSSWZCRemAnalysis()
{
    return CutUtil::pass( sswzcrem_cuts, "SSWZCRem", ( *getAnalysisData( "SSWZCRem" ) ) );
}

//______________________________________________________________________________________
bool passSMWWWSSWZCReeAnalysis()
{
    return CutUtil::pass( sswzcree_cuts, "SSWZCRee", ( *getAnalysisData( "SSWZCRee" ) ) );
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
