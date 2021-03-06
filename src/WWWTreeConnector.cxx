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
        lepton.p4.SetPtEtaPhiM( mytree.lep_pt().at( ilep ),
                                mytree.lep_eta().at( ilep ),
                                mytree.lep_phi().at( ilep ),
                                mytree.lep_mass().at( ilep ) );
        // General
        lepton.dxy                 = mytree.lep_dxy().at( ilep );
        lepton.dz                  = mytree.lep_dz().at( ilep );
        lepton.ip3d                = fabs( mytree.lep_ip3d().at( ilep ) );
        lepton.sip3d               = fabs( mytree.lep_ip3d().at( ilep ) )
                                     / mytree.lep_ip3derr().at( ilep );
        lepton.tightcharge         = mytree.lep_3ch_agree().at( ilep );
        lepton.charge              = mytree.lep_charge().at( ilep );
        lepton.pdgId               = mytree.lep_pdgId().at( ilep );
        if ( getBabyVersion() == 6 )
            lepton.mc_Id             = mytree.lep_mc_Id().at( ilep );

        // Isolation related
        lepton.ptRatio             = mytree.lep_ptRatio().at( ilep );
        lepton.ptRel               = mytree.lep_ptRel().at( ilep );
        lepton.relIso03            = mytree.lep_relIso03().at( ilep );
        lepton.relIso03DB          = mytree.lep_relIso03DB().at( ilep );
        lepton.relIso03EA          = mytree.lep_relIso03EA().at( ilep );
        lepton.relIso03EAv2        = mytree.lep_relIso03EAv2().at( ilep );
        lepton.relIso04DB          = mytree.lep_relIso04DB().at( ilep );
        lepton.relIso04EA          = mytree.lep_relIso04EA().at( ilep );
        lepton.relIso04EAv2        = mytree.lep_relIso04EAv2().at( ilep );
        lepton.miniRelIsoCMS3_EA   = mytree.lep_miniRelIsoCMS3_EA().at( ilep );
        lepton.miniRelIsoCMS3_EAv2 = mytree.lep_miniRelIsoCMS3_EAv2().at( ilep );
        lepton.miniRelIsoCMS3_DB   = mytree.lep_miniRelIsoCMS3_DB().at( ilep );

        if ( getBabyVersion() == 6 )
        {
            // Lepton ID
            if ( mytree.lep_pass_VVV_cutbased_tight().at( ilep ) )
                lepton.id                = 3;
            else if ( mytree.lep_pass_VVV_cutbased_fo_noiso().at( ilep ) )
                lepton.id                = 2;
            else if ( mytree.lep_pass_VVV_cutbased_veto_noiso().at( ilep ) )
                lepton.id                = 1;
            else
                lepton.id                = 0;
            std::vector<int> eventid;
            getEventID( eventid );
            if ( CutUtil::ifEventIdInVerboseCheckList( eventid ) )
                std::cout << ilep << " " << lepton.id << std::endl;
        }
        else if ( getBabyVersion() == 9 )
        {
            // Lepton ID
            if ( mytree.lep_pass_VVV_MVAbased_tight_noiso().at( ilep ) )
                lepton.id                = 3;
            else
                lepton.id                = 0;
        }
        else
        {
            lepton.id = 99;
        }

        // truth matching
        if ( getBabyVersion() >= 5 )
        {
            lepton.isFromX = 0;
            if ( mytree.lep_isFromW().at( ilep ) ) lepton.isFromX |= ( 1<<0 );
            if ( mytree.lep_isFromZ().at( ilep ) ) lepton.isFromX |= ( 1<<1 );
            if ( mytree.lep_isFromB().at( ilep ) ) lepton.isFromX |= ( 1<<2 );
            if ( mytree.lep_isFromC().at( ilep ) ) lepton.isFromX |= ( 1<<3 );
            if ( mytree.lep_isFromL().at( ilep ) ) lepton.isFromX |= ( 1<<4 );
            if ( mytree.lep_isFromLF().at( ilep ) ) lepton.isFromX |= ( 1<<5 );
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
        jet.p4.SetPtEtaPhiE( mytree.jets_p4().at( ijet ).pt(),
                             mytree.jets_p4().at( ijet ).eta(),
                             mytree.jets_p4().at( ijet ).phi(),
                             mytree.jets_p4().at( ijet ).energy() );
        jet.btagCSV = mytree.jets_csv().at( ijet );
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
        jet.p4.SetPtEtaPhiE( mytree.removed_jets_p4().at( ijet ).pt(),
                             mytree.removed_jets_p4().at( ijet ).eta(),
                             mytree.removed_jets_p4().at( ijet ).phi(),
                             mytree.removed_jets_p4().at( ijet ).energy() );
        jet.btagCSV = mytree.removed_jets_csv().at( ijet );
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
