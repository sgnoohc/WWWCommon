// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

// ======================================================================================
//
// This file holds the event selection cut functions.
// The event selection cut functions will then be pushed into a std::vector.
// Then for each event, it will be looped over and each function will be executed.
// Then if all pass, it all passes, etc.
//
// The event selection function must have the following format.
// CutUtil::CutData is a struct in DittoCutUtil.h.
//
// void MyCutFunctionNLepEqTwoCutExample(CutUtil::CutData& cutdata)
// {
//   cutdata.cutname = "NLepEqTwoCut"; // string to hold the name of the cut
//   cutdata.val = ana_data.lepcol["goodSSlep"].size(); // value of the cut being checked
//   cutdata.pass = (cutdata.val == 2); // Whether the cut has passed or not.
//
//   // Set the cutdata.eventid which is std::vector<int> with run,lumi,evt.
//   // See WWWTreeLooperUtil for getEventID.
//   getEventID(cutdata.eventid);
//
// }
//
// Then, the CutUtil::Cuts (which is a std::vector) holds the CutUtil::Cut.
// The CutUtil::Cut is a std::function<void(CutData&)>.
//
// This CutUtil::Cuts will be looped over for each event at the WWWTreeLooper.
// If all passes, "CutUtil::pass(CutUtil::Cuts&)" return true.
// One can also obtain the "CutUtil::passbit(CutUtil::Cuts&)" to make N-1 cuts or so.
//
// ======================================================================================


#include "WWWTreeLooperEventSelections.h"

#define MZ 91.1876

CutUtil::Cuts ss_common_cuts;
CutUtil::Cuts ssmm_cuts;
CutUtil::Cuts ssem_cuts;
CutUtil::Cuts ssee_cuts;

CutUtil::Cuts threelep_common_cuts;
CutUtil::Cuts threelep0_cuts;
CutUtil::Cuts threelep1_cuts;
CutUtil::Cuts threelep2_cuts;

CutUtil::Cuts sswzcr_common_cuts;
CutUtil::Cuts sswzcrmm_cuts;
CutUtil::Cuts sswzcrem_cuts;
CutUtil::Cuts sswzcree_cuts;

CutUtil::Cuts ssar_common_cuts;
CutUtil::Cuts ssarmm_cuts;
CutUtil::Cuts ssarem_cuts;
CutUtil::Cuts ssaree_cuts;


using namespace std::placeholders;

//______________________________________________________________________________________
void loadEventSelections()
{
  loadSSSelections();
  load3LSelections();
  loadSSWZCRSelections();
  loadSSARSelections();
}

//______________________________________________________________________________________
void loadSSSelections()
{
  loadSSCommonSelections();
  loadSSemSelections();
  loadSSmmSelections();
  loadSSeeSelections();
}

//______________________________________________________________________________________
void loadSSCommonSelections()
{

  // Fill the common cuts
  ss_common_cuts.clear();
  ss_common_cuts.push_back(SS_NLepEqTwoCut);
  ss_common_cuts.push_back(SS_Lep0PtCut);
  ss_common_cuts.push_back(SS_Lep1PtCut);
  ss_common_cuts.push_back(SS_NJetCut);
  ss_common_cuts.push_back(SS_Jet0PtCut);
  ss_common_cuts.push_back(SS_ThirdLepVeto);
  ss_common_cuts.push_back(SS_IsoTrackVeto);
  ss_common_cuts.push_back(SS_WmassCut);
  ss_common_cuts.push_back(SS_MjjCut);
  ss_common_cuts.push_back(SS_DEtajjCut);
  ss_common_cuts.push_back(SS_BVeto);

}

//______________________________________________________________________________________
void loadSSmmSelections()
{
  ssmm_cuts.push_back(std::bind(SS_FlavorChan, 169, _1));
  ssmm_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  ssmm_cuts.insert(ssmm_cuts.end(), ss_common_cuts.begin(), ss_common_cuts.end());
}

//______________________________________________________________________________________
void loadSSemSelections()
{
  ssem_cuts.push_back(std::bind(SS_FlavorChan, 143, _1));
  if (getBabyVersion() == 5)
  {
    ssem_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  }
  else
  {
    ssem_cuts.push_back(std::bind(SS_MllCut, 30, _1));
    ssem_cuts.push_back(SS_MTmaxCut);
  }
  ssem_cuts.push_back(std::bind(SS_METCut, 40, _1));
  ssem_cuts.insert(ssem_cuts.end(), ss_common_cuts.begin(), ss_common_cuts.end());
}

//______________________________________________________________________________________
void loadSSeeSelections()
{
  ssee_cuts.push_back(std::bind(SS_FlavorChan, 121, _1));
  ssee_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  ssee_cuts.push_back(std::bind(SS_METCut, 40, _1));
  ssee_cuts.push_back(SS_ZVeto);
  ssee_cuts.insert(ssee_cuts.end(), ss_common_cuts.begin(), ss_common_cuts.end());
}

//______________________________________________________________________________________
void SS_NLepEqTwoCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.lepcol["goodSSlep"].size();
  cutdata.pass = (cutdata.val == 2);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_FlavorChan(int pdgidprod, CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["goodSSlep"].size() >= 2 ?
      (ana_data.lepcol["goodSSlep"][0].pdgId * ana_data.lepcol["goodSSlep"][1].pdgId)
      :
      -999;
  cutdata.pass = (cutdata.val == pdgidprod);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_Lep0PtCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["goodSSlep"].size() >= 1?
    ana_data.lepcol["goodSSlep"][0].p4.Pt()
    :
    -999;
  cutdata.pass = (cutdata.val > 30.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_Lep1PtCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["goodSSlep"].size() >= 2?
    ana_data.lepcol["goodSSlep"][1].p4.Pt()
    :
    -999;
  cutdata.pass = (cutdata.val > 30.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_NJetCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.jetcol["goodSSjet"].size();
  cutdata.pass = (cutdata.val >= 2);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_Jet0PtCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.jetcol["goodSSjet"].size() >= 1 ?
    ana_data.jetcol["goodSSjet"][0].p4.Pt()
    :
    -999;
  cutdata.pass = (cutdata.val > 30);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_ThirdLepVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  if (getBabyVersion() == 6)
//	    cutdata.val = mytree.nlep_VVV_cutbased_tight();
    cutdata.val = ana_data.lepcol["vetolep"].size();
  else if (getBabyVersion() >= 9)
    cutdata.val = mytree.nlep_VVV_cutbased_veto();
  else
    cutdata.val = mytree.nlep();
  cutdata.pass = (cutdata.val == 2);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_IsoTrackVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  if (getBabyVersion() == 6)
    cutdata.val = mytree.nisoTrack_PFHad10_woverlaps() + mytree.nisoTrack_PFLep5_woverlaps() - 2;
  else if (getBabyVersion() >= 9)
    cutdata.val = mytree.nisoTrack_mt2_cleaned_VVV_cutbased_veto();
  else
    cutdata.val = mytree.nisoTrack_mt2();
  cutdata.pass = (cutdata.val == 0);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_WmassCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  if (ana_data.jetcol["goodSSjet"].size() >= 2)
  {
    selectWtaggedJets();
    cutdata.val =
      VarUtil::Mass(ana_data.jetcol["WbosonSSjet"][0],
          ana_data.jetcol["WbosonSSjet"][1]);
  }
  else
  {
    cutdata.val = -999;
  }
  cutdata.pass = (cutdata.val < 100. && cutdata.val > 60.);
  getEventID(cutdata.eventid);
  if (CutUtil::ifEventIdInVerboseCheckList(cutdata.eventid))
  {
    std::cout << " good jets" << std::endl;
    for (auto& jet : ana_data.jetcol["goodSSjet"])
    {
      jet.p4.Print();
    }
    std::cout << " W boson tagged jets" << std::endl;
    ana_data.jetcol["WbosonSSjet"][0].p4.Print();
    ana_data.jetcol["WbosonSSjet"][1].p4.Print();
  }
}

//______________________________________________________________________________________
void SS_MjjCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.jetcol["goodSSjet"].size() >= 2 ?
    VarUtil::Mass(ana_data.jetcol["goodSSjet"][0],
        ana_data.jetcol["goodSSjet"][1])
    :
    -999;
  cutdata.pass = (cutdata.val < 400.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_DEtajjCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.jetcol["goodSSjet"].size() >= 2 ?
    VarUtil::DEta(ana_data.jetcol["goodSSjet"][0],
        ana_data.jetcol["goodSSjet"][1])
    :
    -999;
  cutdata.pass = (cutdata.val < 1.5);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_BVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.jetcol["lssbjet"].size();
  cutdata.pass = (cutdata.val == 0);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_MllCut(float cutval, CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["goodSSlep"].size() >= 2 ?
    VarUtil::Mass(ana_data.lepcol["goodSSlep"][0],
        ana_data.lepcol["goodSSlep"][1])
    :
    -999;
  cutdata.pass = (cutdata.val > cutval);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_METCut(float cutval, CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.met.p4.Pt();
  cutdata.pass = (cutdata.val > cutval);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_ZVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["goodSSlep"].size() >= 2 ?
    VarUtil::Mass(ana_data.lepcol["goodSSlep"][0],
        ana_data.lepcol["goodSSlep"][1])
    :
    -999;
  cutdata.pass = (cutdata.val > 100. || cutdata.val < 80.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_MTmaxCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["goodSSlep"].size() >= 2 ?
    getMTmax()
    :
    -999;
  cutdata.pass = (cutdata.val > 90.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void load3LSelections()
{
  load3LCommonSelections();
  load3L0SFOSSelections();
  load3L1SFOSSelections();
  load3L2SFOSSelections();
}

//______________________________________________________________________________________
void load3LCommonSelections()
{
  // Fill the common cuts
  threelep_common_cuts.clear();
  threelep_common_cuts.push_back(ThreeLep_NLepEqThreeCut);
  threelep_common_cuts.push_back(ThreeLep_TotalChargeCut);
  threelep_common_cuts.push_back(ThreeLep_NJetCut);
//	  if (getBabyVersion() >= 6)
    threelep_common_cuts.push_back(ThreeLep_PtlllCut);
  threelep_common_cuts.push_back(ThreeLep_BVeto);
  threelep_common_cuts.push_back(ThreeLep_FourthLepVeto);
//	  threelep_common_cuts.push_back(ThreeLep_IsoTrackVeto);
}

//______________________________________________________________________________________
void load3L0SFOSSelections()
{
  threelep0_cuts.push_back(std::bind(ThreeLep_NSFOSPairCut, 0, _1));
  threelep0_cuts.push_back(ThreeLep_0SFOS_MllSFCut);
  threelep0_cuts.push_back(ThreeLep_0SFOS_MeeZVetoCut);
//	  if (getBabyVersion() == 5)
//	    threelep0_cuts.push_back(std::bind(ThreeLep_DPhilllCut, 2.5, _1));
//	  if (getBabyVersion() >= 6)
    threelep0_cuts.push_back(std::bind(ThreeLep_DPhilllCut, 2.7, _1));
  threelep0_cuts.insert(threelep0_cuts.end(), threelep_common_cuts.begin(), threelep_common_cuts.end());
}

//______________________________________________________________________________________
void load3L1SFOSSelections()
{
  threelep1_cuts.push_back(std::bind(ThreeLep_NSFOSPairCut, 1, _1));
  threelep1_cuts.push_back(std::bind(ThreeLep_METCut, 45, _1));
  threelep1_cuts.push_back(ThreeLep_1SFOS_MllSFOSCut);
  threelep1_cuts.push_back(std::bind(ThreeLep_DPhilllCut, 2.5, _1));
  threelep1_cuts.insert(threelep1_cuts.end(), threelep_common_cuts.begin(), threelep_common_cuts.end());
}

//______________________________________________________________________________________
void load3L2SFOSSelections()
{
  threelep2_cuts.push_back(std::bind(ThreeLep_NSFOSPairCut, 2, _1));
  threelep2_cuts.push_back(std::bind(ThreeLep_METCut, 55, _1));
  threelep2_cuts.push_back(ThreeLep_2SFOS_Mll0SFOSCut);
  threelep2_cuts.push_back(ThreeLep_2SFOS_Mll1SFOSCut);
  threelep2_cuts.push_back(std::bind(ThreeLep_DPhilllCut, 2.5, _1));
  threelep2_cuts.insert(threelep2_cuts.end(), threelep_common_cuts.begin(), threelep_common_cuts.end());
}

//______________________________________________________________________________________
void ThreeLep_NSFOSPairCut(int nsfos, CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = getNumSFOSPairs();
  cutdata.pass = (((int)cutdata.val) == nsfos);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_NLepEqThreeCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.lepcol["good3Llep"].size();
  cutdata.pass = (cutdata.val == 3);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_TotalChargeCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = totalcharge();
  cutdata.pass = (cutdata.val == 1);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_NJetCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.jetcol["good3Ljet"].size();
  cutdata.pass = (cutdata.val <= 1);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_DPhilllCut(float cutval, CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["good3Llep"].size() == 3 ?
    fabs(
        VarUtil::DPhi(
          ana_data.lepcol["good3Llep"][0].p4 +
          ana_data.lepcol["good3Llep"][1].p4 +
          ana_data.lepcol["good3Llep"][2].p4,
          ana_data.met.p4
          )
        )
    :
    -999;
  cutdata.pass = (cutdata.val > cutval);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_PtlllCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.lepcol["good3Llep"].size() >= 3 ?
    (ana_data.lepcol["good3Llep"][0].p4 +
     ana_data.lepcol["good3Llep"][1].p4 +
     ana_data.lepcol["good3Llep"][2].p4
     ).Pt()
    :
    -999;
  cutdata.pass = (cutdata.val > 60.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_BVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.jetcol["lssbjet"].size();
  cutdata.pass = (cutdata.val == 0);
  getEventID(cutdata.eventid);
}


//______________________________________________________________________________________
void ThreeLep_FourthLepVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  if (getBabyVersion() == 6)
//	    cutdata.val = mytree.nlep_VVV_cutbased_tight();
    cutdata.val = ana_data.lepcol["vetolep"].size();
  else if (getBabyVersion() >= 9)
    cutdata.val = mytree.nlep_VVV_cutbased_veto();
  else
    cutdata.val = mytree.nlep();
  cutdata.pass = (cutdata.val == 3);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_IsoTrackVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  if (getBabyVersion() >= 6)
    cutdata.val = mytree.nisoTrack_PFHad10_woverlaps() + mytree.nisoTrack_PFLep5_woverlaps() - 3;
  else if (getBabyVersion() >= 9)
    cutdata.val = mytree.nisoTrack_mt2_cleaned_VVV_cutbased_veto();
  else
    cutdata.val = mytree.nisoTrack_mt2();
  cutdata.pass = (cutdata.val == 0);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_METCut(float cutval, CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.met.p4.Pt();
  cutdata.pass = (cutdata.val > cutval);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_0SFOS_MllSFCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = get0SFOSMll();
  cutdata.pass = (cutdata.val > 20.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_0SFOS_MeeZVetoCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = get0SFOSMee();
  cutdata.pass = (fabs(cutdata.val - MZ) > 15.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_1SFOS_MllSFOSCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = get1SFOSMll();
  cutdata.pass = (cutdata.val < 55. || cutdata.val > 110.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_2SFOS_Mll0SFOSCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = get2SFOSMll0();
  cutdata.pass = (fabs(cutdata.val - MZ) > 20.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void ThreeLep_2SFOS_Mll1SFOSCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = get2SFOSMll1();
  cutdata.pass = (fabs(cutdata.val - MZ) > 20.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void loadSSWZCRSelections()
{
  loadSSWZCRCommonSelections();
  loadSSWZCReeSelections();
  loadSSWZCRemSelections();
  loadSSWZCRmmSelections();
}

//______________________________________________________________________________________
void loadSSWZCRCommonSelections()
{

  // Fill the common cuts
  sswzcr_common_cuts.clear();
  sswzcr_common_cuts.push_back(SSWZCR_NLepEqThreeCut);
  sswzcr_common_cuts.push_back(SSWZCR_AtLeastOneMZSFOS);
  sswzcr_common_cuts.push_back(SS_Lep0PtCut);
  sswzcr_common_cuts.push_back(SS_Lep1PtCut);
  sswzcr_common_cuts.push_back(SS_NJetCut);
  sswzcr_common_cuts.push_back(SS_Jet0PtCut);
//	  sswzcr_common_cuts.push_back(SS_ThirdLepVeto);
//	  sswzcr_common_cuts.push_back(SS_IsoTrackVeto);
//	  sswzcr_common_cuts.push_back(SS_WmasswzcrCut);
  sswzcr_common_cuts.push_back(SS_MjjCut);
  sswzcr_common_cuts.push_back(SS_DEtajjCut);
  sswzcr_common_cuts.push_back(SS_BVeto);

}

//______________________________________________________________________________________
void loadSSWZCRmmSelections()
{
  sswzcrmm_cuts.push_back(std::bind(SS_FlavorChan, 169, _1));
  sswzcrmm_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  sswzcrmm_cuts.insert(sswzcrmm_cuts.end(), sswzcr_common_cuts.begin(), sswzcr_common_cuts.end());
}

//______________________________________________________________________________________
void loadSSWZCRemSelections()
{
  sswzcrem_cuts.push_back(std::bind(SS_FlavorChan, 143, _1));
  if (getBabyVersion() == 5)
  {
    sswzcrem_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  }
  else
  {
    sswzcrem_cuts.push_back(std::bind(SS_MllCut, 30, _1));
    sswzcrem_cuts.push_back(SS_MTmaxCut);
  }
  sswzcrem_cuts.push_back(std::bind(SS_METCut, 40, _1));
  sswzcrem_cuts.insert(sswzcrem_cuts.end(), sswzcr_common_cuts.begin(), sswzcr_common_cuts.end());
}

//______________________________________________________________________________________
void loadSSWZCReeSelections()
{
  sswzcree_cuts.push_back(std::bind(SS_FlavorChan, 121, _1));
  sswzcree_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  sswzcree_cuts.push_back(std::bind(SS_METCut, 40, _1));
  sswzcree_cuts.push_back(SS_ZVeto);
  sswzcree_cuts.insert(sswzcree_cuts.end(), sswzcr_common_cuts.begin(), sswzcr_common_cuts.end());
}

//______________________________________________________________________________________
void SSWZCR_NLepEqThreeCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.lepcol["goodSSlep"].size();
  cutdata.pass = (cutdata.val == 3);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SSWZCR_AtLeastOneMZSFOS(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = getSSWZCRSFOSMll();
  cutdata.pass = (fabs(cutdata.val - MZ) < 10.);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void loadSSARSelections()
{
  loadSSARCommonSelections();
  loadSSARemSelections();
  loadSSARmmSelections();
  loadSSAReeSelections();
}

//______________________________________________________________________________________
void loadSSARCommonSelections()
{

  // Fill the common cuts
  ssar_common_cuts.clear();
  ssar_common_cuts.push_back(SS_NLepEqTwoCut);
  ssar_common_cuts.push_back(SS_Lep0PtCut);
  ssar_common_cuts.push_back(SS_Lep1PtCut);
  ssar_common_cuts.push_back(SS_NJetCut);
  ssar_common_cuts.push_back(SS_Jet0PtCut);
  ssar_common_cuts.push_back(SS_ThirdLepVeto);
  ssar_common_cuts.push_back(SS_IsoTrackVeto);
  ssar_common_cuts.push_back(SS_WmassCut);
  ssar_common_cuts.push_back(SS_MjjCut);
  ssar_common_cuts.push_back(SS_DEtajjCut);
  ssar_common_cuts.push_back(SS_BVeto);

}

//______________________________________________________________________________________
void loadSSARmmSelections()
{
  ssarmm_cuts.push_back(std::bind(SS_FlavorChan, 169, _1));
  ssarmm_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  ssarmm_cuts.insert(ssarmm_cuts.end(), ssar_common_cuts.begin(), ssar_common_cuts.end());
}

//______________________________________________________________________________________
void loadSSARemSelections()
{
  ssarem_cuts.push_back(std::bind(SS_FlavorChan, 143, _1));
  if (getBabyVersion() == 5)
  {
    ssarem_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  }
  else
  {
    ssarem_cuts.push_back(std::bind(SS_MllCut, 30, _1));
    ssarem_cuts.push_back(SS_MTmaxCut);
  }
  ssarem_cuts.push_back(std::bind(SS_METCut, 40, _1));
  ssarem_cuts.insert(ssarem_cuts.end(), ssar_common_cuts.begin(), ssar_common_cuts.end());
}

//______________________________________________________________________________________
void loadSSAReeSelections()
{
  ssaree_cuts.push_back(std::bind(SS_FlavorChan, 121, _1));
  ssaree_cuts.push_back(std::bind(SS_MllCut, 40, _1));
  ssaree_cuts.push_back(std::bind(SS_METCut, 40, _1));
  ssaree_cuts.push_back(SS_ZVeto);
  ssaree_cuts.insert(ssaree_cuts.end(), ssar_common_cuts.begin(), ssar_common_cuts.end());
}

// eof
