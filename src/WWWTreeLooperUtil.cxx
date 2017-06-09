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
  return abs(mytree.lep_pdgId().at(0)/abs(mytree.lep_pdgId().at(0)) +
             mytree.lep_pdgId().at(1)/abs(mytree.lep_pdgId().at(1)) +
             mytree.lep_pdgId().at(2)/abs(mytree.lep_pdgId().at(2)));
}

//______________________________________________________________________________________
int getNumSFOSPairs(){
  /*Loops through pairs of entries in the lep_pdgId vector and counts how many have opposite value*/
  int num_SFOS = 0;
  for (int i = 0; i < (int) mytree.lep_pdgId().size(); i++){
    for (int j = i+1; j < (int) mytree.lep_pdgId().size(); j++){
      if (mytree.lep_pdgId().at(i) == -mytree.lep_pdgId().at(j)) num_SFOS++;
    }
  }
  return num_SFOS;
}

//______________________________________________________________________________________
void printEventList(string prefix)
{
  std::cout << prefix.c_str() << ": make_tuple(" << mytree.evt() << "," <<  mytree.run() << "," << mytree.lumi() << ")," << std::endl;
  std::cout << prefix.c_str() << "HJ: " << mytree.run() << ":" <<  mytree.lumi() << ":" << mytree.evt() << std::endl;
}

//______________________________________________________________________________________
bool failed(float cutid)
{
  std::vector<int> eventid;
  eventid.push_back(mytree.run());
  eventid.push_back(mytree.lumi());
  eventid.push_back(mytree.evt());
  return LoopUtil::failed(eventid, cutid);
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
void processSSEvents(string prefix, int iSR)
{
  /// Histogramming
  fillHistograms(prefix);
  fillHistograms("SS");
  printEventList("SSmm");
}

//______________________________________________________________________________________
void fillCounter(int iSR)
{
//	  HistUtil::fillCounter("SMWWWAnalysis_SR_counts", ana_data, iSR, SR[iSR]);
}
