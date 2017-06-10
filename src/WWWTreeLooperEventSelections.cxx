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

CutUtil::Cuts ss_common_cuts;
CutUtil::Cuts ssmm_cuts;
CutUtil::Cuts ssem_cuts;
CutUtil::Cuts ssee_cuts;

using namespace std::placeholders;

//______________________________________________________________________________________
void loadEventSelections()
{
  loadSSSelections();
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
  ssem_cuts.push_back(std::bind(SS_MllCut, 30, _1));
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
  cutdata.val = mytree.nlep();
  cutdata.pass = (cutdata.val == 2);
  getEventID(cutdata.eventid);
}

//______________________________________________________________________________________
void SS_IsoTrackVeto(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
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
void SS_MjjCut(float cutval, CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val =
    ana_data.jetcol["goodSSlep"].size() >= 2 ?
    VarUtil::Mass(ana_data.jetcol["goodSSlep"][0],
        ana_data.jetcol["goodSSlep"][1])
    :
    -999;
  cutdata.pass = (cutdata.val > cutval);
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


//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) >  40.      )) return failed(__LINE__);
