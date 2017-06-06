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
#include "WWWTree.h"
#include "WWWTreeConnector.h"

using namespace Ditto;

extern ObjUtil::AnalysisData ana_data;
extern TString output_name;

int WWWTreeLooper(TChain* chain, TString output_name, int nEvents);
void loop();
void beforeLoop(TChain* chain, TString output_name, int nevents);
void afterLoop();
void initWWWTree();
void loadWWWTreeEvent();
void processWWWTreeEvent();

//-------------------------------------------------------------
// Object selections

void getObjects();
void selectObjects();

bool isGoodSSLepton(ObjUtil::Lepton& lepton);
bool isGoodSSElectron(ObjUtil::Lepton& lepton);
bool isGoodSSMuon(ObjUtil::Lepton& lepton);

bool isGood3LLepton(ObjUtil::Lepton& lepton);
bool isGood3LElectron(ObjUtil::Lepton& lepton);
bool isGood3LMuon(ObjUtil::Lepton& lepton);

bool isVetoLepton(ObjUtil::Lepton& lepton);
bool isVetoElectron(ObjUtil::Lepton& lepton);
bool isVetoMuon(ObjUtil::Lepton& lepton);

bool isGoodSSJet(ObjUtil::Jet& jet);
bool isGood3LJet(ObjUtil::Jet& jet);
bool isGoodWWWMediumBJet(ObjUtil::Jet& jet);
bool isGoodWWWLooseBJet(ObjUtil::Jet& jet);

//-------------------------------------------------------------
// Event selections

// Run the analysis
void runSignalRegions();

// SS selections
bool passSMWWWSScommonselection(string prefix, int pdgidprod, int& counter);
bool doSMWWWSSmmAnalysis();
bool doSMWWWSSemAnalysis();
bool doSMWWWSSeeAnalysis();

// 3L selections
bool passSMWWW3Lcommonselection(string prefix, int pdgidprod, int& counter);
bool doSMWWW3L0SFOSAnalysis();
bool doSMWWW3L1SFOSAnalysis();
bool doSMWWW3L2SFOSAnalysis();

// SS WWW* selections
bool doSMWWWStarSSmmAnalysis();

//-------------------------------------------------------------
// Utility functions

int totalcharge();
int getNumSFOSPairs();
void printEventList(string prefix);
bool failed(float cutid);
bool passGenLevelEventFilter();
bool passGenLevelWHWWW();
bool passGenLevelWjetsHTStitch();
bool passGenLevelZjetsHTStitch();
void reweightWWW2lepFilteredSample();

//-------------------------------------------------------------
// Histogramming functions

void fillHistograms(string prefix);
void fillHistogramsTruthMatchingLeptons3L(string prefix);
void fillHistogramsTruthMatchingLeptons(string prefix);
void fillHiggsMassVariables(string prefix);

#endif
//eof
