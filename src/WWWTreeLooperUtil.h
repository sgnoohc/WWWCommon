// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef WWWTreeLooperUtil_H
#define WWWTreeLooperUtil_H

#include <iostream>
#include <string.h>

#include "WWWTree.h"
#include "WWWTreeLooperAnalysisData.h"
#include "WWWTreeLooperHistUtil.h"
#include "Ditto.h"
using namespace Ditto;

//-------------------------------------------------------------
// Utility functions

int totalcharge();
int getNumSFOSPairs();
float get0SFOSMll();
float get0SFOSMee();
float get1SFOSMll();
float get2SFOSMll0();
float get2SFOSMll1();
float getMTmax();
float getSSWZCRSFOSMll();
void printEventList(string prefix);
bool passGenLevelEventFilter();
bool passGenLevelWHWWW();
bool passGenLevelWjetsHTStitch();
bool passGenLevelZjetsHTStitch();
void reweightWWW2lepFilteredSample();
void selectWtaggedJets();
void selectWtaggedJetsViaLeadJets();
void getEventID(std::vector<int>&);

bool isSignalSample();
bool isDataSample();
int getBabyVersion();

unsigned int getNLepFromX(ObjUtil::Leptons&, unsigned int);
unsigned int getNLepFromW(ObjUtil::Leptons&);
unsigned int getNLepFromZ(ObjUtil::Leptons&);

enum BkgType {
  kTrueBkg,
  kChFlip,
  kLostLep,
  kOneFake,
  kTwoFake,
  kOthers
};

BkgType getBkgTypeSS();
BkgType getBkgType3L();

void setAnalysisDataSS();
void setAnalysisData3L();
ObjUtil::AnalysisData* getAnalysisData(TString prefix);

bool modifyGoodSSLepContainerForApplicationRegion();
bool modifyGoodSSLepContainerForOLOT();
void restoreGoodSSLepContainer();

#endif
