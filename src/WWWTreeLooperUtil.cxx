// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWTreeLooperUtil.h"

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
  if (ana_data.lepcol["good3Llep"].size() != 3)
    return -999;
  return abs(ana_data.lepcol["good3Llep"][0].pdgId/abs(ana_data.lepcol["good3Llep"][0].pdgId) +
             ana_data.lepcol["good3Llep"][1].pdgId/abs(ana_data.lepcol["good3Llep"][1].pdgId) +
             ana_data.lepcol["good3Llep"][2].pdgId/abs(ana_data.lepcol["good3Llep"][2].pdgId));
}

//______________________________________________________________________________________
int getNumSFOSPairs(){
  if (ana_data.lepcol["good3Llep"].size() != 3)
    return -999;
  /*Loops through pairs of entries in the lep_pdgId vector and counts how many have opposite value*/
  int num_SFOS = 0;
  for (int i = 0; i < (int) ana_data.lepcol["good3Llep"].size(); i++){
    for (int j = i+1; j < (int) ana_data.lepcol["good3Llep"].size(); j++){
      if (ana_data.lepcol["good3Llep"][i].pdgId == -(ana_data.lepcol["good3Llep"][j].pdgId)) num_SFOS++;
    }
  }
  return num_SFOS;
}

//______________________________________________________________________________________
float get0SFOSMll()
{
  if (ana_data.lepcol["good3Llep"].size() != 3)
    return -999;

  // Sort by pdgId to count them easily
  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_abspdgId<ObjUtil::Lepton>);

  ObjUtil::Lepton& lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton& lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton& lep2 = ana_data.lepcol["good3Llep"][2];

  float Mll = 0;
  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);
  return Mll;
}

//______________________________________________________________________________________
float get0SFOSMee()
{
  // The no good value is not 90 for 0SFOS, because there may not be a Mee pair
  float Mll = -999;
  if (ana_data.lepcol["good3Llep"].size() != 3)
    return Mll;

  // Sort by pdgId to count them easily
  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_abspdgId<ObjUtil::Lepton>);

//	  ObjUtil::Lepton& lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton& lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton& lep2 = ana_data.lepcol["good3Llep"][2];

  if (abs(lep1.pdgId) == 11)
    Mll = VarUtil::Mass(lep1, lep2);

  return Mll;
}

//______________________________________________________________________________________
float get1SFOSMll()
{
  // the "no good" value is MZ=90, since we want to Z-veto.
  float Mll = 90;
  if (ana_data.lepcol["good3Llep"].size() != 3)
    return Mll;

  // Sort by pdgId to count them easily
  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_abspdgId<ObjUtil::Lepton>);

  ObjUtil::Lepton& lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton& lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton& lep2 = ana_data.lepcol["good3Llep"][2];

  if (abs(lep1.pdgId) == 11) Mll = VarUtil::Mass(lep1, lep2);
  if (abs(lep1.pdgId) == 13) Mll = VarUtil::Mass(lep0, lep1);
  return Mll;
}

//______________________________________________________________________________________
float get2SFOSMll0()
{
  // the "no good" value is MZ=90, since we want to Z-veto.
  if (ana_data.lepcol["good3Llep"].size() != 3)
    return 90.;

  // Sort by pdgId to count them easily
  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_pdgId<ObjUtil::Lepton>);

  ObjUtil::Lepton& lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton& lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton& lep2 = ana_data.lepcol["good3Llep"][2];

  if (lep1.pdgId > 0)
    return VarUtil::Mass(lep2, lep0);
  else
    return VarUtil::Mass(lep0, lep1);

}

//______________________________________________________________________________________
float get2SFOSMll1()
{
  // the "no good" value is MZ=90, since we want to Z-veto.
  if (ana_data.lepcol["good3Llep"].size() != 3)
    return 90.;

  // Sort by pdgId to count them easily
  std::sort(ana_data.lepcol["good3Llep"].begin(), ana_data.lepcol["good3Llep"].end(), comparator_pdgId<ObjUtil::Lepton>);

  ObjUtil::Lepton& lep0 = ana_data.lepcol["good3Llep"][0];
  ObjUtil::Lepton& lep1 = ana_data.lepcol["good3Llep"][1];
  ObjUtil::Lepton& lep2 = ana_data.lepcol["good3Llep"][2];

  if (lep1.pdgId > 0)
    return VarUtil::Mass(lep2, lep1);
  else
    return VarUtil::Mass(lep0, lep2);
}

//______________________________________________________________________________________
float getSSWZCRSFOSMll()
{
  // the "no good" value is MZ=90, since we want to Z-veto.
  if (ana_data.lepcol["goodSSlep"].size() != 3)
    return -999;

  if (ana_data.lepcol["goodSSlep"].size() != 3)
    return -999;
  /*Loops through pairs of entries in the lep_pdgId vector and counts how many have opposite value*/
  float MllClosestToZ = -999;
  for (int i = 0; i < (int) ana_data.lepcol["goodSSlep"].size(); i++){
    for (int j = i+1; j < (int) ana_data.lepcol["goodSSlep"].size(); j++){
      if (ana_data.lepcol["goodSSlep"][i].pdgId == -(ana_data.lepcol["goodSSlep"][j].pdgId))
      {
        float tmpmll = VarUtil::Mass(ana_data.lepcol["goodSSlep"][i], ana_data.lepcol["goodSSlep"][j]);
        if (fabs(tmpmll-90) < fabs(MllClosestToZ-90))
          MllClosestToZ = tmpmll;
      }
    }
  }
  return MllClosestToZ;
}

//______________________________________________________________________________________
float getMTmax()
{
  if (ana_data.lepcol["goodSSlep"].size() < 2)
    return -999;
  float MT0;
  float MT1;
  MT0 = VarUtil::MT(ana_data.lepcol["goodSSlep"][0], ana_data.met);
  MT1 = VarUtil::MT(ana_data.lepcol["goodSSlep"][1], ana_data.met);
  return MT0 > MT1 ? MT0 : MT1;
}

//______________________________________________________________________________________
void printEventList(string prefix)
{
//	  std::cout << prefix.c_str() << ": make_tuple(" << mytree.evt() << "," <<  mytree.run() << "," << mytree.lumi() << ")," << std::endl;
//	  std::cout << prefix.c_str() << "HJ: " << mytree.run() << ":" <<  mytree.lumi() << ":" << mytree.evt() << std::endl;
  std::vector<int> eventid;
  getEventID(eventid);
  std::cout << prefix.c_str() << " " << eventid[0] << " " << eventid[1] << " " << eventid[2] << std::endl;
}

//______________________________________________________________________________________
bool passGenLevelEventFilter()
{
  if (!passGenLevelWHWWW()) return false;
  if (!passGenLevelWjetsHTStitch()) return false;
  if (!passGenLevelZjetsHTStitch()) return false;
  return true;
}

//______________________________________________________________________________________
bool passGenLevelWHWWW()
{
  // VH Non-bb sample counting
  int nW = 0;
  int nZ = 0;
  int nWfromH = 0;
  int nZfromH = 0;
  if (LoopUtil::output_name.Contains("vh_non"))
  {
    for (unsigned int igen = 0; igen < mytree.genPart_pdgId().size(); ++igen)
    {
      if (abs(mytree.genPart_pdgId().at(igen)) == 24 && mytree.genPart_status().at(igen) == 22 && mytree.genPart_motherId().at(igen) != 25) nW++;
      if (abs(mytree.genPart_pdgId().at(igen)) == 23 && mytree.genPart_status().at(igen) == 22 && mytree.genPart_motherId().at(igen) != 25) nZ++;
      if (abs(mytree.genPart_pdgId().at(igen)) == 24 && mytree.genPart_status().at(igen) == 22 && mytree.genPart_motherId().at(igen) == 25) nWfromH++;
      if (abs(mytree.genPart_pdgId().at(igen)) == 23 && mytree.genPart_status().at(igen) == 22 && mytree.genPart_motherId().at(igen) == 25) nZfromH++;
    }

    // WH
    if (nW == 1)
    {
      HistUtil::fillCounter("Hprod", ana_data, 1);
      if (nWfromH == 2)
      {
        HistUtil::fillCounter("Hdecay", ana_data, 0);
      }
      else
      {
        HistUtil::fillCounter("TotalWs", ana_data, nW+nWfromH);
        HistUtil::fillCounter("Hdecay", ana_data, 1);
        return false;
      }
    }
    // ZH
    else if (nZ == 1)
    {
      HistUtil::fillCounter("Hprod", ana_data, 2);
      return false;
    }
    // unknown
    else
    {
      HistUtil::fillCounter("Hprod", ana_data, 0);
      return false;
    }
  }
  return true;
}

//______________________________________________________________________________________
bool passGenLevelWjetsHTStitch()
{
  // HT filter stitching for wjets
  if (LoopUtil::output_name.Contains("wjets_incl"))
    if (mytree.gen_ht() > 100.)
      return false;
  return true;
}

//______________________________________________________________________________________
bool passGenLevelZjetsHTStitch()
{
  // HT filter stitching for DY
  if (LoopUtil::output_name.Contains("dy_m50") && !LoopUtil::output_name.Contains("_ht"))
    if (mytree.gen_ht() > 100.)
      return false;
  return true;
}

//______________________________________________________________________________________
void reweightWWW2lepFilteredSample()
{
  if (LoopUtil::output_name.Contains("www_2l_ext1_mia_skim_1")) ana_data.wgt *= 0.066805*164800./(91900.+164800.);
  if (LoopUtil::output_name.Contains("www_2l_mia_skim_1"))      ana_data.wgt *= 0.066805*91900./(91900.+164800.);
}

//______________________________________________________________________________________
void selectWtaggedJets()
{
  ObjUtil::Jet jet0;
  ObjUtil::Jet jet1;
  CombUtil::MinDRPair(ana_data.jetcol["goodSSjet"], jet0, jet1);
  ObjUtil::Jets wbosonjets;
  wbosonjets.push_back(jet0);
  wbosonjets.push_back(jet1);
  ana_data.jetcol["WbosonSSjet"] = wbosonjets;
}

//______________________________________________________________________________________
void selectWtaggedJetsViaLeadJets()
{
  ObjUtil::Jets wbosonjets;
  wbosonjets.push_back(ana_data.jetcol["goodSSjet"][0]);
  wbosonjets.push_back(ana_data.jetcol["goodSSjet"][1]);
  ana_data.jetcol["WbosonSSjet"] = wbosonjets;
}

//______________________________________________________________________________________
void getEventID(std::vector<int>& eventid)
{
  eventid.push_back(mytree.run());
  eventid.push_back(mytree.lumi());
  eventid.push_back(mytree.evt());
}

//______________________________________________________________________________________
bool isSignalSample()
{
  if (LoopUtil::output_name.Contains("www_") || LoopUtil::output_name.Contains("vh_nonbb"))
    return true;
  else
    return false;
}

//______________________________________________________________________________________
int getBabyVersion()
{
  if (LoopUtil::last_tfile_name.Contains("v0.1.5"))
    return 5;
  if (LoopUtil::last_tfile_name.Contains("v0.1.6"))
    return 6;
  return 0;
}

//______________________________________________________________________________________
unsigned int getNLepFromX(ObjUtil::Leptons& leptons, unsigned int pidx)
{
  unsigned int n = 0;
  for (auto& lepton: leptons)
    if ((lepton.isFromX & (1 << pidx)) != 0) n++;
  return n;
}

//______________________________________________________________________________________
unsigned int getNLepFromW(ObjUtil::Leptons& leptons)
{
  return getNLepFromX(leptons, 0);
}

//______________________________________________________________________________________
unsigned int getNLepFromZ(ObjUtil::Leptons& leptons)
{
  return getNLepFromX(leptons, 1);
}

//______________________________________________________________________________________
BkgType getBkgTypeSS()
{

  // If signal sample don't do any of this stuff
  if (isSignalSample())
    return kTrue;

  // Initialize as not belonging to any type
  BkgType sn = kOthers;

  // Leptons passing 2l selection
  if (ana_data.lepcol["goodSSlep"].size() >= 2)
  {
    int nW = getNLepFromW(ana_data.lepcol["goodSSlep"]);
    int nZ = getNLepFromW(ana_data.lepcol["goodSSlep"]);

    ObjUtil::Lepton& lep0 = ana_data.lepcol["goodSSlep"][0];
    ObjUtil::Lepton& lep1 = ana_data.lepcol["goodSSlep"][1];

    int mc_id_product = lep0.mc_Id * lep1.mc_Id;

    // True SS category (e.g. W+W+)
    if (nW == 2 &&  mc_id_product > 0)
      sn = kTrue;
    // W+W(else) charg flip
    else if (nW == 2)
      sn = kChFlip;
    else if (nZ == 2 && mc_id_product <= 0)
      sn = kChFlip;
    // ZZ both with a lost lepton
    else if (nZ == 2)
      sn = kLostLep;
    // WZ
    else if(nW == 1 && nZ == 1)
      sn = kLostLep;
    else if ((nW+nZ)==1)
      sn = kOneFake;
    else if ((nW+nZ)==0)
      sn = kTwoFake;
    else
      sn = kOthers;
  }

  return sn;
}

//______________________________________________________________________________________
BkgType getBkgType3L()
{

  // If signal sample don't do any of this stuff
  if (isSignalSample())
    return kTrue;

  // Initialize with nothing
  BkgType sn2 = kOthers;

  // Leptons passing 3l lepton selection
  if (ana_data.lepcol["good3Llep"].size() >= 3)
  {
    int nW = getNLepFromW(ana_data.lepcol["good3Llep"]);
    int nZ = getNLepFromW(ana_data.lepcol["good3Llep"]);

    ObjUtil::Lepton& lep0 = ana_data.lepcol["good3Llep"][0];
    ObjUtil::Lepton& lep1 = ana_data.lepcol["good3Llep"][1];
    ObjUtil::Lepton& lep2 = ana_data.lepcol["good3Llep"][2];

    // Charge flip W+W+W+
    if (nW == 3 && (lep0.mc_Id > 0 && lep1.mc_Id > 0 && lep2.mc_Id > 0))
      sn2 = kChFlip;
    // Charge flip W-W-W-
    else if (nW == 3 && (lep0.mc_Id > 0 && lep1.mc_Id > 0 && lep2.mc_Id > 0))
      sn2 = kChFlip;
    else if (nW == 3)
      sn2 = kTrue;
    // Lost lepton for ttZ
    else if (nW == 2 && nZ == 1)
      sn2 = kLostLep;
    // Neglect WZZ as LL
    else if (nW == 1 && nZ == 2)
      sn2 = kTrue;
    // ZZ
    else if (nZ == 3)
      sn2 = kLostLep;
    else if ((nW+nZ)==2)
      sn2 = kOneFake;
    else if ((nW+nZ)==1)
      sn2 = kTwoFake;
    else
      sn2 = kOthers;
  }

  return sn2;
}

//______________________________________________________________________________________
void setAnalysisDataSS()
{
  switch (getBkgTypeSS())
  {
    case kTrue:    ana_data_SS = &ana_data_SS_true;    return; break;
    case kChFlip:  ana_data_SS = &ana_data_SS_chflip;  return; break;
    case kLostLep: ana_data_SS = &ana_data_SS_lostlep; return; break;
    case kOneFake: ana_data_SS = &ana_data_SS_onefake; return; break;
    case kTwoFake: ana_data_SS = &ana_data_SS_twofake; return; break;
    case kOthers:  ana_data_SS = &ana_data_SS_others;  return; break;
    default: PrintUtil::error("I should not be here"); break;
  }
}

//______________________________________________________________________________________
void setAnalysisData3L()
{
  switch (getBkgType3L())
  {
    case kTrue:    ana_data_3L = &ana_data_3L_true;    return; break;
    case kChFlip:  ana_data_3L = &ana_data_3L_chflip;  return; break;
    case kLostLep: ana_data_3L = &ana_data_3L_lostlep; return; break;
    case kOneFake: ana_data_3L = &ana_data_3L_onefake; return; break;
    case kTwoFake: ana_data_3L = &ana_data_3L_twofake; return; break;
    case kOthers:  ana_data_3L = &ana_data_3L_others;  return; break;
    default: PrintUtil::error("I should not be here"); break;
  }
}

//______________________________________________________________________________________
ObjUtil::AnalysisData* getAnalysisData(TString prefix)
{
  if (getBabyVersion() < 6)
    return &ana_data;

  if (prefix.Contains("SS"))
  {
    setAnalysisDataSS();
    return ana_data_SS;
  }
  else if (prefix.Contains("ThreeLep"))
  {
    setAnalysisData3L();
    return ana_data_3L;
  }
  else
  {
    return 0;
  }
}

//eof
