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
void printEventList(string prefix);
bool failed(float cutid);
bool passGenLevelEventFilter();
bool passGenLevelWHWWW();
bool passGenLevelWjetsHTStitch();
bool passGenLevelZjetsHTStitch();
void reweightWWW2lepFilteredSample();
void selectWtaggedJets();
void selectWtaggedJetsViaLeadJets();

#endif
