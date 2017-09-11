// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWTreeLooperHistUtil.h"

//=====================================================================================
//=====================================================================================
//=====================================================================================
// Histogram functions
//=====================================================================================
//=====================================================================================
//=====================================================================================

//______________________________________________________________________________________
void fillHistograms(TString prefix, ObjUtil::AnalysisData* a)
{
  // If looping over an older version things simplify
  if (getBabyVersion() == 5)
  {
    fillHistogramsStandard(prefix, &ana_data);
    return;
  }

  // If the default method set the data to the default one ana_data
  if (a == 0)
    a = getAnalysisData(prefix);

  (*a).leptons = ana_data.leptons;
  (*a).jets    = ana_data.jets;
  (*a).met     = ana_data.met;
  (*a).wgt     = ana_data.wgt;

  fillHistogramsStandard(prefix, a);
}

//______________________________________________________________________________________
void fillHistogramsStandard(TString prefix, ObjUtil::AnalysisData* a)
{
  printEventList(prefix.Data());
  if (prefix.Contains("SSmm")) HistUtil::fillCounter("SR", *a, 0);
  if (prefix.Contains("SSem")) HistUtil::fillCounter("SR", *a, 1);
  if (prefix.Contains("SSee")) HistUtil::fillCounter("SR", *a, 2);
  if (prefix.Contains("0SFOS")) HistUtil::fillCounter("SR", *a, 3);
  if (prefix.Contains("1SFOS")) HistUtil::fillCounter("SR", *a, 4);
  if (prefix.Contains("2SFOS")) HistUtil::fillCounter("SR", *a, 5);
  if (prefix.Contains("SSARmm")) { HistUtil::fillCounter("SR", *a, 6); }
  if (prefix.Contains("SSARem")) { HistUtil::fillCounter("SR", *a, 7); }
  if (prefix.Contains("SSARee")) { HistUtil::fillCounter("SR", *a, 8); }
  if (prefix.Contains("SSWZCRmm")) { HistUtil::fillCounter("SR", *a, 9); }
  if (prefix.Contains("SSWZCRem")) { HistUtil::fillCounter("SR", *a,10); }
  if (prefix.Contains("SSWZCRee")) { HistUtil::fillCounter("SR", *a,11); }
  HistUtil::fillMET(prefix.Data(), *a);
  HistUtil::fillLeps(prefix.Data(), *a);
  HistUtil::fillFakeLeps(prefix.Data(), *a);
  HistUtil::fillJets(prefix.Data(), *a);
  HistUtil::fillDiLepChan(prefix.Data(), *a);
  HistUtil::fillDiLepVars(prefix.Data(), *a, 0, 1);
  HistUtil::fillDiJetVars(prefix.Data(), *a, 0, 1);
  if (ana_data.jetcol["goodSSjet"].size() >= 2)
  {
    selectWtaggedJets();
    PlotUtil::plot1D("wmassmjj",
        (ana_data.jetcol["WbosonSSjet"][0].p4 +
         ana_data.jetcol["WbosonSSjet"][1].p4).M(),
        (*a).wgt, (*a).hist_db , "", 180, 0., 180., prefix.Data());
  }
  HistUtil::fillLepMTs(prefix.Data(), *a);
  if (prefix.Contains("SFOS"))
    PlotUtil::plot1D("ptlll",
        (ana_data.lepcol["good3Llep"][0].p4 +
         ana_data.lepcol["good3Llep"][1].p4 +
         ana_data.lepcol["good3Llep"][2].p4).Pt(),
        (*a).wgt, (*a).hist_db , "ptlll", 180, 0., 180., prefix.Data());
}

//	//______________________________________________________________________________________
//	void fillLepWPair(string prefix)
//	{
//	  if (ana_data.jetcol["WbosonSSjet"].size() < 2)
//	    return;
//	  ObjUtil::Jet jet0 = ana_data.jetcol["WbosonSSjet"][0];
//	  ObjUtil::Jet jet1 = ana_data.jetcol["WbosonSSjet"][1];
//	  TLorentzVector wp4 = jet0.p4 + jet1.p4;
//	  ObjUtil::Lepton lep0 = ana_data.lepcol["goodSSlep"][0];
//	  ObjUtil::Lepton lep1 = ana_data.lepcol["goodSSlep"][1];
//	
//	  float Mlep0W = VarUtil::Mass(lep0, wp4);
//	  float Mlep1W = VarUtil::Mass(lep1, wp4);
//	  float minMlepW = Mlep0W > Mlep1W ? Mlep1W : Mlep0W;
//	  float maxMlepW = Mlep0W > Mlep1W ? Mlep0W : Mlep1W;
//	  float sumMlepW = minMlepW + maxMlepW;
//	  float dMlepW = maxMlepW - minMlepW;
//	  PlotUtil::plot1D("minmljj" , minMlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 360, prefix);
//	  PlotUtil::plot1D("maxmljj" , maxMlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 460, prefix);
//	  PlotUtil::plot1D("summljj" , sumMlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 1060, prefix);
//	  PlotUtil::plot1D("dmljj" , dMlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 360, prefix);
//	
//	  float DRlep0W = VarUtil::DR(lep0, wp4);
//	  float DRlep1W = VarUtil::DR(lep1, wp4);
//	  float minDRlepW = DRlep0W > DRlep1W ? DRlep1W : DRlep0W;
//	  float maxDRlepW = DRlep0W > DRlep1W ? DRlep0W : DRlep1W;
//	  float sumDRlepW = minDRlepW + maxDRlepW;
//	  float dDRlepW = maxDRlepW - maxDRlepW;
//	  PlotUtil::plot1D("mindrljj" , minDRlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	  PlotUtil::plot1D("maxdrljj" , maxDRlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	  PlotUtil::plot1D("sumdrljj" , sumDRlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	  PlotUtil::plot1D("ddrljj" , dDRlepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	
//	  float DPhilep0W = VarUtil::DPhi(lep0, wp4);
//	  float DPhilep1W = VarUtil::DPhi(lep1, wp4);
//	  float minDPhilepW = DPhilep0W > DPhilep1W ? DPhilep1W : DPhilep0W;
//	  float maxDPhilepW = DPhilep0W > DPhilep1W ? DPhilep0W : DPhilep1W;
//	  float sumDPhilepW = minDPhilepW + maxDPhilepW;
//	  float dDPhilepW = maxDPhilepW - maxDPhilepW;
//	  PlotUtil::plot1D("mindphiljj" , minDPhilepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	  PlotUtil::plot1D("maxdphiljj" , maxDPhilepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	  PlotUtil::plot1D("sumdphiljj" , sumDPhilepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	  PlotUtil::plot1D("ddphiljj" , dDPhilepW, ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 9, prefix);
//	}
//	
//	//______________________________________________________________________________________
//	void fillHiggsMassVariables(string prefix)
//	{
//	  ObjUtil::Jet jet0;
//	  ObjUtil::Jet jet1;
//	//	  VarUtil::MjjClosest(ana_data, jet0, jet1);
//	  PlotUtil::plot1D("mljj" , (jet0.p4+jet1.p4+ana_data.leptons[1].p4).M(), ana_data.wgt , ana_data.hist_db , "" , 180 , 0. , 360, prefix);
//	}
//	
//	//______________________________________________________________________________________
//	void fillHistogramsTruthMatchingLeptons3L(string prefix)
//	{
//	  /// Lepton truth matching category information
//	  /// Using isFromX fill the category
//	
//	  // If we don't have two leptons exit
//	  if (ana_data.lepcol["goodSSlep"].size() < 3)
//	    return;
//	
//	  // Read the three leptons
//	  ObjUtil::Lepton lep0 = ana_data.lepcol["good3Llep"][0];
//	  ObjUtil::Lepton lep1 = ana_data.lepcol["good3Llep"][1];
//	  ObjUtil::Lepton lep2 = ana_data.lepcol["good3Llep"][2];
//	
//	  // Fill histogram based on truth matching
//	  HistUtil::fillLepIsFromX(prefix, ana_data);
//	
//	  // Counting number of prompt vs. fake
//	  int prompt = 0;
//	  if (lep0.isFromX == (1 << 0) || lep0.isFromX == (1 << 1)) prompt++;
//	  if (lep1.isFromX == (1 << 0) || lep1.isFromX == (1 << 1)) prompt++;
//	  if (lep2.isFromX == (1 << 0) || lep2.isFromX == (1 << 1)) prompt++;
//	
//	  PlotUtil::plot1D("truth3L_nlep_isFromW", prompt, ana_data.wgt, ana_data.hist_db, "", 4, 0., 4., prefix);
//	}
//	
//	
//	
//	
//	//======================================================================================
//	//======================================================================================
//	//======================================================================================
//	//======================================================================================
//	//======================================================================================
//	//======================================================================================
//	
//	
//	
//	//______________________________________________________________________________________
//	void fillHistogramsTruthMatchingLeptons(string prefix)
//	{
//	  /// Lepton truth matching category information
//	  /// Using isFromX fill the category
//	
//	  // If we don't have two leptons exit
//	  if (ana_data.lepcol["goodSSlep"].size() != 2) return;
//	
//	  ObjUtil::Lepton lep0 = ana_data.lepcol["goodSSlep"][0];
//	  ObjUtil::Lepton lep1 = ana_data.lepcol["goodSSlep"][1];
//	
//	  // Leading lepton
//	  /* 1 : isFromW */        if (lep0.isFromX == 1                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 1, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 2 : isFromZ */        if (lep0.isFromX == 2                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 2, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 3 : isFromW/Z */      if (lep0.isFromX == 1 || lep0.isFromX == 2) PlotUtil::plot1D("leptruthcategorySS_lep0", 3, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 4 : isFromB/C/L/LF */ if (lep0.isFromX >= 4                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 4, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 5 : isFromB/C */      if (lep0.isFromX == 4 || lep0.isFromX == 8) PlotUtil::plot1D("leptruthcategorySS_lep0", 5, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 0 : not matched */    if (lep0.isFromX == 0                     ) PlotUtil::plot1D("leptruthcategorySS_lep0", 0, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	
//	  // Sub-leading lepton
//	  /* 1 : isFromW */        if (lep1.isFromX == 1                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 1, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 2 : isFromZ */        if (lep1.isFromX == 2                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 2, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 3 : isFromW/Z */      if (lep1.isFromX == 1 || lep1.isFromX == 2) PlotUtil::plot1D("leptruthcategorySS_lep1", 3, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 4 : isFromB/C/L/LF */ if (lep1.isFromX >= 4                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 4, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 5 : isFromB/C */      if (lep1.isFromX == 4 || lep1.isFromX == 8) PlotUtil::plot1D("leptruthcategorySS_lep1", 5, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	  /* 0 : not matched */    if (lep1.isFromX == 0                     ) PlotUtil::plot1D("leptruthcategorySS_lep1", 0, ana_data.wgt, ana_data.hist_db, "", 6, 0., 6., prefix);
//	
//	  // event category
//	  /* 1 : both are from W */if ( lep0.isFromX == 1 && lep1.isFromX == 1)  PlotUtil::plot1D("leptruthcategorySS"   , 0, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	  /* 2 : one is from W   */if ((lep0.isFromX == 1 && lep1.isFromX != 1)||
//	                               (lep1.isFromX == 1 && lep0.isFromX != 1)) PlotUtil::plot1D("leptruthcategorySS"   , 1, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	  /* 3 : none are from W */if ( lep0.isFromX != 1 && lep1.isFromX != 1)  PlotUtil::plot1D("leptruthcategorySS"   , 2, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	
//	  if ((lep0.isFromX == 1 && lep1.isFromX != 1) || (lep1.isFromX == 1 && lep0.isFromX != 1))
//	  {
//	    if (lep0.isFromX == 1)
//	    {
//	      if (lep1.isFromX >= 4)
//	      {
//	        PlotUtil::plot1D("leptruthcategorySS_oneW", 0, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	        ObjUtil::Jets removedjets = getRemovedJets();
//	        float mindr = -999;
//	        float csv = -999;
//	        for (auto& jet : removedjets)
//	        {
//	          float tmpdr = VarUtil::DR(lep1, jet);
//	          if (mindr < 0 || mindr > tmpdr)
//	          {
//	            mindr = tmpdr;
//	            csv = jet.btagCSV;
//	          }
//	        }
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso03EA" , lep1.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso04EA" , lep1.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_ip3d"       , lep1.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_sip3d"      , lep1.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	
//	        mindr = -999;
//	        csv = -999;
//	        for (auto& jet : removedjets)
//	        {
//	          float tmpdr = VarUtil::DR(lep0, jet);
//	          if (mindr < 0 || mindr > tmpdr)
//	          {
//	            mindr = tmpdr;
//	            csv = jet.btagCSV;
//	          }
//	        }
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso03EA" , lep0.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso03EA" , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso04EA" , lep0.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_ip3d"       , lep0.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_sip3d"      , lep0.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	
//	      }
//	      else if (lep1.isFromX == 2)
//	      {
//	        PlotUtil::plot1D("leptruthcategorySS_oneW", 1, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	      }
//	      else
//	      {
//	        PlotUtil::plot1D("leptruthcategorySS_oneW", 2, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	        PlotUtil::plot1D("ngentau", mytree.ngenTau(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
//	        PlotUtil::plot1D("nTaus20", mytree.nTaus20(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_reliso03EA" , lep1.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_reliso04EA" , lep1.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_ip3d"       , lep1.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_sip3d"      , lep1.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_pt"         , lep1.p4.Pt()                 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_eta"        , lep1.p4.Eta()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
//	        PlotUtil::plot1D("misidlep1isfromnothing_phi"        , lep1.p4.Phi()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_reliso03EA" , lep1.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_reliso04EA" , lep1.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_ip3d"       , lep1.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_sip3d"      , lep1.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_pt"         , lep1.p4.Pt()                 , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_eta"        , lep1.p4.Eta()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_phi"        , lep1.p4.Phi()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
//	      }
//	    }
//	    else if (lep1.isFromX == 1)
//	    {
//	      if (lep0.isFromX >= 4)
//	      {
//	        PlotUtil::plot1D("leptruthcategorySS_oneW", 0, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	        ObjUtil::Jets removedjets = getRemovedJets();
//	        float mindr = -999;
//	        float csv = -999;
//	        for (auto& jet : removedjets)
//	        {
//	          float tmpdr = VarUtil::DR(lep1, jet);
//	          if (mindr < 0 || mindr > tmpdr)
//	          {
//	            mindr = tmpdr;
//	            csv = jet.btagCSV;
//	          }
//	        }
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso03EA" , lep0.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso03EA" , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_reliso04EA" , lep0.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_ip3d"       , lep0.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_sip3d"      , lep0.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	
//	        mindr = -999;
//	        csv = -999;
//	        for (auto& jet : removedjets)
//	        {
//	          float tmpdr = VarUtil::DR(lep1, jet);
//	          if (mindr < 0 || mindr > tmpdr)
//	          {
//	            mindr = tmpdr;
//	            csv = jet.btagCSV;
//	          }
//	        }
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_dr"         , mindr                        , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_csv"        , csv                          , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.   , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso03EA" , lep1.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso03EA" , lep1.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso04EA" , lep1.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_ip3d"       , lep1.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("leptruthcategorySS_oneW_matched_sip3d"      , lep1.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	      }
//	      else if (lep0.isFromX == 2)
//	      {
//	        PlotUtil::plot1D("leptruthcategorySS_oneW", 1, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	      }
//	      else
//	      {
//	        PlotUtil::plot1D("leptruthcategorySS_oneW", 2, ana_data.wgt, ana_data.hist_db, "", 3, 0., 3., prefix);
//	        PlotUtil::plot1D("ngentau", mytree.ngenTau(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
//	        PlotUtil::plot1D("nTaus20", mytree.nTaus20(), ana_data.wgt, ana_data.hist_db, "", 5, 0., 5., prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_reliso03EA" , lep0.relIso03EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_absiso03EA" , lep0.relIso03EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_reliso04EA" , lep0.relIso04EA              , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_ip3d"       , lep0.ip3d                    , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_sip3d"      , lep0.sip3d                   , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_pt"         , lep0.p4.Pt()                 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_eta"        , lep0.p4.Eta()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
//	        PlotUtil::plot1D("misidlep0isfromnothing_phi"        , lep0.p4.Phi()                , ana_data.wgt , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_reliso03EA" , lep0.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_absiso03EA" , lep0.relIso03EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_reliso04EA" , lep0.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 7.25 , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_ip3d"       , lep0.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 0.015, prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_sip3d"      , lep0.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 4.   , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_pt"         , lep0.p4.Pt()                 , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0.  , 100. , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_eta"        , lep0.p4.Eta()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-2.5 , 2.5  , prefix);
//	        PlotUtil::plot1D("misidlepisfromnothing_phi"        , lep0.p4.Phi()                , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 ,-3.15, 3.15 , prefix);
//	      }
//	    }
//	  }
//	  else if (lep0.isFromX != 1 && lep1.isFromX != 1)
//	  {
//	    if (lep0.isFromX == (1<<1)) PlotUtil::plot1D("lep0misid_mcmatch"    , 0 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<2)) PlotUtil::plot1D("lep0misid_mcmatch"    , 1 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<3)) PlotUtil::plot1D("lep0misid_mcmatch"    , 2 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<4)) PlotUtil::plot1D("lep0misid_mcmatch"    , 3 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<5)) PlotUtil::plot1D("lep0misid_mcmatch"    , 4 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<1)) PlotUtil::plot1D("lep1misid_mcmatch"    , 0 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<2)) PlotUtil::plot1D("lep1misid_mcmatch"    , 1 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<3)) PlotUtil::plot1D("lep1misid_mcmatch"    , 2 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<4)) PlotUtil::plot1D("lep1misid_mcmatch"    , 3 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<5)) PlotUtil::plot1D("lep1misid_mcmatch"    , 4 , ana_data.wgt    , ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<1)) PlotUtil::plot1D("lepmisid_mcmatch"    , 0 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<2)) PlotUtil::plot1D("lepmisid_mcmatch"    , 1 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<3)) PlotUtil::plot1D("lepmisid_mcmatch"    , 2 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<4)) PlotUtil::plot1D("lepmisid_mcmatch"    , 3 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep0.isFromX == (1<<5)) PlotUtil::plot1D("lepmisid_mcmatch"    , 4 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<1)) PlotUtil::plot1D("lepmisid_mcmatch"    , 0 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<2)) PlotUtil::plot1D("lepmisid_mcmatch"    , 1 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<3)) PlotUtil::plot1D("lepmisid_mcmatch"    , 2 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<4)) PlotUtil::plot1D("lepmisid_mcmatch"    , 3 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	    if (lep1.isFromX == (1<<5)) PlotUtil::plot1D("lepmisid_mcmatch"    , 4 , ana_data.wgt / 2., ana_data.hist_db , "" , 5 , 0. , 5.    , prefix);
//	
//	    PlotUtil::plot1D("lep0misid_reliso03EA" , lep0.relIso03EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lep0misid_absiso03EA" , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lep0misid_reliso04EA" , lep0.relIso04EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lep0misid_absiso04EA" , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lep0misid_ip3d"       , lep0.ip3d                    , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
//	    PlotUtil::plot1D("lep0misid_sip3d"      , lep0.sip3d                   , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
//	    PlotUtil::plot1D("lep1misid_reliso03EA" , lep1.relIso03EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lep1misid_absiso03EA" , lep1.relIso03EA*lep0.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lep1misid_reliso04EA" , lep1.relIso04EA              , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lep1misid_absiso04EA" , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lep1misid_ip3d"       , lep1.ip3d                    , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
//	    PlotUtil::plot1D("lep1misid_sip3d"      , lep1.sip3d                   , ana_data.wgt    , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
//	    PlotUtil::plot1D("lepmisid_reliso03EA"  , lep0.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_absiso03EA"  , lep0.relIso03EA*lep0.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_reliso04EA"  , lep0.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_absiso04EA"  , lep0.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_ip3d"        , lep0.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
//	    PlotUtil::plot1D("lepmisid_sip3d"       , lep0.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
//	    PlotUtil::plot1D("lepmisid_reliso03EA"  , lep1.relIso03EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_absiso03EA"  , lep1.relIso03EA*lep0.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_reliso04EA"  , lep1.relIso04EA              , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_absiso04EA"  , lep1.relIso04EA*lep1.p4.Pt() , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 7.25  , prefix);
//	    PlotUtil::plot1D("lepmisid_ip3d"        , lep1.ip3d                    , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 0.015 , prefix);
//	    PlotUtil::plot1D("lepmisid_sip3d"       , lep1.sip3d                   , ana_data.wgt/2. , ana_data.hist_db , "" , 10000 , 0. , 4.    , prefix);
//	  }
//	  else
//	  {
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_csv"                , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.    , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_dr"                 , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.    , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW"                    , -1   , ana_data.wgt , ana_data.hist_db , "" , 3     , 0.  , 3.    , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_dr"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , -1. , 5.    , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_csv"        , -999 , ana_data.wgt , ana_data.hist_db , "" , 180   , 0.  , 1.    , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso03EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso03EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_reliso03EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_absiso03EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_reliso04EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_absiso04EA"         , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_ip3d"               , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015 , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_sip3d"              , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.    , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_reliso04EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_absiso04EA" , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 7.25  , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_ip3d"       , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 0.015 , prefix);
//	    PlotUtil::plot1D("leptruthcategorySS_oneW_matched_sip3d"      , -999 , ana_data.wgt , ana_data.hist_db , "" , 10000 , 0.  , 4.    , prefix);
//	  }
//	}
