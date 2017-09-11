// vim: tabstop=4:softtabstop=4:shiftwidth=4:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWTreeConnector.h"

ObjUtil::Leptons getLeptons( /*WWWTree& mytree*/ )
{
    ObjUtil::Leptons leptons;
    for ( int ilep = 0; ilep < mytree.nlep(); ++ilep )
    {
        ObjUtil::Lepton lepton;
        lepton.p4.SetPtEtaPhiM( mytree.lep_pt()[ilep],
                                mytree.lep_eta()[ilep],
                                mytree.lep_phi()[ilep],
                                mytree.lep_mass()[ilep] );
        // General
        lepton.dxy                 = mytree.lep_dxy()[ilep];
        lepton.dz                  = mytree.lep_dz()[ilep];
        lepton.ip3d                = fabs( mytree.lep_ip3d()[ilep] );
        lepton.sip3d               = fabs( mytree.lep_ip3d()[ilep] )
                                     / mytree.lep_ip3derr()[ilep];
        lepton.tightcharge         = mytree.lep_3ch_agree()[ilep];
        lepton.charge              = mytree.lep_charge()[ilep];
        lepton.pdgId               = mytree.lep_pdgId()[ilep];
        lepton.mc_Id               = mytree.lep_mc_Id()[ilep];

        // Isolation related
        lepton.ptRatio             = mytree.lep_ptRatio()[ilep];
        lepton.ptRel               = mytree.lep_ptRel()[ilep];
        lepton.relIso03            = mytree.lep_relIso03()[ilep];
        lepton.relIso03DB          = mytree.lep_relIso03DB()[ilep];
        lepton.relIso03EA          = mytree.lep_relIso03EA()[ilep];
        lepton.relIso03EAv2        = mytree.lep_relIso03EAv2()[ilep];
        lepton.relIso04DB          = mytree.lep_relIso04DB()[ilep];
        lepton.relIso04EA          = mytree.lep_relIso04EA()[ilep];
        lepton.relIso04EAv2        = mytree.lep_relIso04EAv2()[ilep];
        lepton.miniRelIsoCMS3_EA   = mytree.lep_miniRelIsoCMS3_EA()[ilep];
        lepton.miniRelIsoCMS3_EAv2 = mytree.lep_miniRelIsoCMS3_EAv2()[ilep];
        lepton.miniRelIsoCMS3_DB   = mytree.lep_miniRelIsoCMS3_DB()[ilep];

        if ( getBabyVersion() == 6 )
        {
            // Lepton ID
            if ( mytree.lep_pass_VVV_cutbased_tight()[ilep] )
                lepton.id                = 3;
            else if ( mytree.lep_pass_VVV_cutbased_fo_noiso()[ilep] )
                lepton.id                = 2;
            else if ( mytree.lep_pass_VVV_cutbased_veto_noiso()[ilep] )
                lepton.id                = 1;
            else
                lepton.id                = 0;
            std::vector<int> eventid;
            getEventID( eventid );
            if ( CutUtil::ifEventIdInVerboseCheckList( eventid ) )
                std::cout << ilep << " " << lepton.id << std::endl;
        }
        else if ( getBabyVersion() >= 9 )
        {
            // Lepton ID
            if ( mytree.lep_pass_VVV_cutbased_tight()[ilep] )
                lepton.id                = 3;
            else if ( mytree.lep_pass_VVV_cutbased_fo_noiso()[ilep] )
                lepton.id                = 2;
            else
                lepton.id                = 0;
        }
        else
        {
            lepton.id = 99;
        }

        if ( getBabyVersion() >= 9 )
        {
            lepton.motherId = mytree.lep_motherIdSS()[ilep];

            // Fix the charge issue
            if ( mytree.lep_isFromW()[ilep] || mytree.lep_isFromZ()[ilep] )
            {
                if ( mytree.lep_mc_Id()[ilep] == mytree.lep_pdgId()[ilep] )
                    lepton.motherId = 1;
                else if ( mytree.lep_mc_Id()[ilep] == -mytree.lep_pdgId()[ilep] )
                    lepton.motherId = 2;
                else
                    lepton.motherId = 0;
            }
        }

        // truth matching
        if ( getBabyVersion() >= 5 )
        {
            lepton.isFromX = 0;
            if ( mytree.lep_isFromW()[ilep] ) lepton.isFromX |= ( 1<<0 );
            if ( mytree.lep_isFromZ()[ilep] ) lepton.isFromX |= ( 1<<1 );
            if ( mytree.lep_isFromB()[ilep] ) lepton.isFromX |= ( 1<<2 );
            if ( mytree.lep_isFromC()[ilep] ) lepton.isFromX |= ( 1<<3 );
            if ( mytree.lep_isFromL()[ilep] ) lepton.isFromX |= ( 1<<4 );
            if ( mytree.lep_isFromLF()[ilep] ) lepton.isFromX |= ( 1<<5 );
        }
        leptons.push_back( lepton );
    }
    return leptons;
}

ObjUtil::Jets getJets( /*WWWTree& mytree*/ )
{
    ObjUtil::Jets jets;
    for ( int ijet = 0; ijet < mytree.njets(); ++ijet )
    {
        ObjUtil::Jet jet;
        jet.p4.SetPtEtaPhiE( mytree.jets_p4()[ijet].pt(),
                             mytree.jets_p4()[ijet].eta(),
                             mytree.jets_p4()[ijet].phi(),
                             mytree.jets_p4()[ijet].energy() );
        jet.btagCSV = mytree.jets_csv()[ijet];
        jets.push_back( jet );
    }
    return jets;
}

ObjUtil::Jets getRemovedJets( /*WWWTree& mytree*/ )
{
    ObjUtil::Jets jets;
    for ( unsigned int ijet = 0; ijet < mytree.removed_jets_p4().size(); ++ijet )
    {
        ObjUtil::Jet jet;
        jet.p4.SetPtEtaPhiE( mytree.removed_jets_p4()[ijet].pt(),
                             mytree.removed_jets_p4()[ijet].eta(),
                             mytree.removed_jets_p4()[ijet].phi(),
                             mytree.removed_jets_p4()[ijet].energy() );
        jet.btagCSV = mytree.removed_jets_csv()[ijet];
        jets.push_back( jet );
    }
    return jets;
}

ObjUtil::Jets getBJets( /*WWWTree& mytree*/ )
{
    ObjUtil::Jets jets;
    return jets;
}

ObjUtil::METs getMETs( /*WWWTree& mytree*/ )
{
    ObjUtil::METs mets;
    return mets;
}

ObjUtil::MET getMET( /*WWWTree& mytree*/ )
{
    ObjUtil::MET met;
    met.p4.SetPtEtaPhiE( mytree.met_pt(), 0, mytree.met_phi(), mytree.met_pt() );
    return met;
}

//eof
