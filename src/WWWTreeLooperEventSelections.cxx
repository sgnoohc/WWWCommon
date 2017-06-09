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

void loadEventSelections()
{
  using namespace std::placeholders;
  ss_common_cuts.clear();
  ss_common_cuts.push_back(SS_NLepEqTwoCut);
  ss_common_cuts.push_back(SS_Lep0PtCut);
  ss_common_cuts.push_back(SS_Lep1PtCut);
  ss_common_cuts.push_back(SS_NJetCut);
  ss_common_cuts.push_back(SS_Jet0PtCut);
}

void SS_NLepEqTwoCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.lepcol["goodSSlep"].size();
  cutdata.pass = (cutdata.val == 2);
  getEventID(cutdata.eventid);
}

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

void SS_NJetCut(CutUtil::CutData& cutdata)
{
  cutdata.cutname = __FUNCTION__;
  cutdata.val = ana_data.jetcol["goodSSjet"].size();
  cutdata.pass = (cutdata.val >= 2);
  getEventID(cutdata.eventid);
}

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

//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"][0].pdgId * ana_data.lepcol["goodSSlep"][1].pdgId == pdgidprod  )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.lepcol["goodSSlep"][0].pdgId + ana_data.lepcol["goodSSlep"][1].pdgId > 0           )) return failed(__LINE__);

//	  selectWtaggedJets();
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nlep() == 2                                                                          )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( mytree.nisoTrack_mt2() == 0                                                                 )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.lepcol["goodSSlep"][0], ana_data.lepcol["goodSSlep"][1]) >  40.      )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.jetcol["WbosonSSjet"][0], ana_data.jetcol["WbosonSSjet"][1]) < 100.  )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.jetcol["WbosonSSjet"][0], ana_data.jetcol["WbosonSSjet"][1]) >  60.  )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::Mass(ana_data.jetcol["goodSSjet"][0], ana_data.jetcol["goodSSjet"][1]) < 400.      )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( VarUtil::DEta(ana_data.jetcol["goodSSjet"][0], ana_data.jetcol["goodSSjet"][1]) < 1.5       )) return failed(__LINE__);
//	  HistUtil::fillCutflow(prefix, ana_data, counter); if (!( ana_data.jetcol["lssbjet"].size() == 0                                                      )) return failed(__LINE__);
