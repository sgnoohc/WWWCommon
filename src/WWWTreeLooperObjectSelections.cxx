// vim: tabstop=4:softtabstop=4:shiftwidth=4:expandtab

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
    ana_data.lepcol["lbntSSlep"] = getLeptons();
    ana_data.lepcol["looseSSlep"] = getLeptons();
    ana_data.lepcol["good3Llep"] = getLeptons();
    ana_data.lepcol["vetolep"] = getLeptons();
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
    Analyses::selectObjs<ObjUtil::Lepton>(ana_data.lepcol["lbntSSlep"] , isLbntSSLepton);
    Analyses::selectObjs<ObjUtil::Lepton>(ana_data.lepcol["looseSSlep"], isLooseSSLepton);
    Analyses::selectObjs<ObjUtil::Lepton>(ana_data.lepcol["good3Llep"] , isGood3LLepton);
    Analyses::selectObjs<ObjUtil::Lepton>(ana_data.lepcol["vetolep"]   , isVetoLepton);
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
bool isLbntSSLepton(ObjUtil::Lepton& lepton)
{
    return isLbntSSElectron(lepton) || isLbntSSMuon(lepton);
}

//______________________________________________________________________________________
bool isLbntSSElectron(ObjUtil::Lepton& lepton)
{
    return isLooseSSElectron(lepton) && !isGoodSSElectron(lepton);
}

//______________________________________________________________________________________
bool isLbntSSMuon(ObjUtil::Lepton& lepton)
{
    return isLooseSSMuon(lepton) && !isGoodSSMuon(lepton);
}

//______________________________________________________________________________________
bool isLooseSSLepton(ObjUtil::Lepton& lepton)
{
    return isLooseSSElectron(lepton) || isLooseSSMuon(lepton);
}

//______________________________________________________________________________________
bool isGoodSSLepton(ObjUtil::Lepton& lepton)
{
    return isGoodSSElectron(lepton) || isGoodSSMuon(lepton);
}

//______________________________________________________________________________________
bool isGoodSSElectron(ObjUtil::Lepton& lepton)
{
    if (!( abs(lepton.pdgId) == 11             )) return false;
    if (!( lepton.p4.Pt() > 30.                )) return false;
    if (!( lepton.relIso03EA < 0.06            )) return false;
    if (!( fabs(lepton.ip3d) < 0.015           )) return false;
    if (!( lepton.tightcharge != 0             )) return false;
    if (!( lepton.id >= 3                      )) return false;
    if (!( fabs(lepton.p4.Eta()) < 1.4 ||
           fabs(lepton.p4.Eta()) > 1.6     )) return false;
    return true;
}

//______________________________________________________________________________________
bool isGoodSSMuon(ObjUtil::Lepton& lepton)
{
    std::vector<int> eventid;
    getEventID(eventid);
    if (!( abs(lepton.pdgId) == 13             )) return CutUtil::fail_event(eventid, "pdgID");
    if (!( lepton.p4.Pt() > 30.                )) return CutUtil::fail_event(eventid, "PT");
    if (!( fabs(lepton.p4.Eta()) < 2.4         )) return CutUtil::fail_event(eventid, "2.4");
    if (!( lepton.relIso03EA < 0.06            )) return CutUtil::fail_event(eventid, "reliso");
    if (!( fabs(lepton.ip3d) < 0.015           )) return CutUtil::fail_event(eventid, "ip3d");
    if (!( lepton.id >= 3                      )) return CutUtil::fail_event(eventid, "lepid");
    if (getBabyVersion() == 5)
        if (!( fabs(lepton.p4.Eta()) < 1.4 ||
               fabs(lepton.p4.Eta()) > 1.6     )) return CutUtil::fail_event(eventid, "Should not happen for v9");
    return true;
}

//______________________________________________________________________________________
bool isLooseSSElectron(ObjUtil::Lepton& lepton)
{
    if (!( abs(lepton.pdgId) == 11             )) return false;
    if (!( lepton.p4.Pt() > 30.                )) return false;
    if (!( lepton.relIso03EA < 0.2             )) return false;
    if (!( lepton.id >= 2                      )) return false;
    if (!( fabs(lepton.p4.Eta()) < 1.4 ||
           fabs(lepton.p4.Eta()) > 1.6     )) return false;
    return true;
}

//______________________________________________________________________________________
bool isLooseSSMuon(ObjUtil::Lepton& lepton)
{
    std::vector<int> eventid;
    getEventID(eventid);
    if (!( abs(lepton.pdgId) == 13             )) return CutUtil::fail_event(eventid, "loosepdgID");
    if (!( lepton.p4.Pt() > 30.                )) return CutUtil::fail_event(eventid, "loosePT");
    if (!( fabs(lepton.p4.Eta()) < 2.4         )) return CutUtil::fail_event(eventid, "loose2.4");
    if (!( lepton.relIso03EA < 0.40            )) return CutUtil::fail_event(eventid, "loosereliso");
    if (!( lepton.id >= 2                      )) return CutUtil::fail_event(eventid, "looselepid");
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
//    if (!( fabs(lepton.p4.Eta()) < 2.4  )) return false;
    if (!( lepton.relIso03EA < 0.1      )) return false;
    if (!( fabs(lepton.ip3d) < 0.015    )) return false;
    if (!( lepton.id >= 3               )) return false;
    if (getBabyVersion() >= 9)
        if (!( fabs(lepton.p4.Eta()) < 1.4 ||
               fabs(lepton.p4.Eta()) > 1.6     )) return false;
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
    if (!( lepton.id >= 3               )) return false;
    if (getBabyVersion() == 5)
        if (!( fabs(lepton.p4.Eta()) < 1.4 || fabs(lepton.p4.Eta()) > 1.6  )) return false;
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
    if (!( lepton.p4.Pt() > 5.         )) return false;
    if (!( fabs(lepton.p4.Eta()) < 2.4 )) return false;
    if (!( lepton.relIso03EA < 0.4     )) return false;
    if (!( lepton.id == 1              )) return false;
    return true;
}

//______________________________________________________________________________________
bool isVetoMuon(ObjUtil::Lepton& lepton)
{
    if (!( lepton.p4.Pt() > 5.         )) return false;
    if (!( fabs(lepton.p4.Eta()) < 2.4 )) return false;
    if (!( lepton.relIso03EA < 0.4     )) return false;
    if (!( lepton.id == 1              )) return false;
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
    if (getBabyVersion() == 5)
    {
        if (!( jet.p4.Pt()        > 25. )) return false;
        if (!( fabs(jet.p4.Eta()) < 4.5 )) return false;
    }
    else if (getBabyVersion() == 6)
    {
        if (!( jet.p4.Pt()        > 25. )) return false;
        if (!( fabs(jet.p4.Eta()) < 4.5 )) return false;
    }
    else if (getBabyVersion() >= 9)
    {
        if (!( jet.p4.Pt()        > 30. )) return false;
        if (!( fabs(jet.p4.Eta()) < 5.0 )) return false;
    }
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






