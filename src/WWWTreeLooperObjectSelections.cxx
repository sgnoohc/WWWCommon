// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWTreeLooperObjectSelections.h"

//=====================================================================================
//=====================================================================================
//=====================================================================================
// Object selections
//=====================================================================================
//=====================================================================================
//=====================================================================================

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

  // To hadd 2lep WWW samples reweight each files for maximal statistics
  reweightWWW2lepFilteredSample();
}

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
  std::sort(ana_data.lepcol["good3Llep"].begin(),
            ana_data.lepcol["good3Llep"].end(),
            comparator_abspdgId<ObjUtil::Lepton>);
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
  if (!( lepton.relIso03EA < 0.06     )) return false;
  if (!( fabs(lepton.ip3d) < 0.015    )) return false;
  if (!( lepton.tightcharge != 0      )) return false;
  return true;
}

//______________________________________________________________________________________
bool isGoodSSMuon(ObjUtil::Lepton& lepton)
{
  if (!( abs(lepton.pdgId) == 13      )) return false;
  if (!( lepton.p4.Pt() > 30.         )) return false;
  if (!( fabs(lepton.p4.Eta()) < 2.4  )) return false;
  if (!( lepton.relIso03EA < 0.06     )) return false;
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






