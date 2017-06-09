// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef WWWTreeLooper_h
#define WWWTreeLooper_h

// ROOT
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TString.h>
#include <TTree.h>

// C++
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Ditto
#include "Ditto.h"

// WWWCommon
#include "WWWTree.h"
#include "WWWTreeConnector.h"
#include "WWWTreeLooperAnalysisData.h"
#include "WWWTreeLooperUtil.h"
#include "WWWTreeLooperObjectSelections.h"

using namespace Ditto;

int WWWTreeLooper(TChain* chain, TString output_name, int nEvents);
void loop();
void beforeLoop(TChain* chain, TString output_name, int nevents);
void afterLoop();
void initWWWTree();
void loadWWWTreeEvent();
void processWWWTreeEvent();

//-------------------------------------------------------------
// Event selections

enum {
  SSmm,
  SSem,
  SSee,
  SFOS0,
  SFOS1,
  SFOS2
};

// Run the analysis
void runSignalRegions();

// SS selections
bool passSMWWWSScommonselection(string prefix, int pdgidprod, int& counter);
bool passSMWWWSSmmAnalysis();
bool passSMWWWSSemAnalysis();
bool passSMWWWSSeeAnalysis();

bool passSSNLepCut   (float&, string&, std::vector<int>&, int);
bool passSSChannel   (float&, string&, std::vector<int>&, int);
bool passSSLep0PtCut (float&, string&, std::vector<int>&, float);
bool passSSLep1PtCut (float&, string&, std::vector<int>&, float);
bool passSSNJetCut   (float&, string&, std::vector<int>&, int);
bool passSSJet0PtCut (float&, string&, std::vector<int>&, float);
bool passSS3rdLepVeto(float&, string&, std::vector<int>&);
bool passSSLowMllCut (float&, string&, std::vector<int>&, float);
bool passSSWMassCut  (float&, string&, std::vector<int>&, float, float);
bool passSSMjjCut    (float&, string&, std::vector<int>&, float);
bool passSSDEtajjCut (float&, string&, std::vector<int>&, float);
bool passSSBVeto     (float&, string&, std::vector<int>&, string);

// 3L selections
bool passSMWWW3Lcommonselection(string prefix, int pdgidprod, int& counter);
bool doSMWWW3L0SFOSAnalysis();
bool doSMWWW3L1SFOSAnalysis();
bool doSMWWW3L2SFOSAnalysis();

#endif
//eof
