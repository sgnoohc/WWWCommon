// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef WWWTreeLooperHistUtil_H
#define WWWTreeLooperHistUtil_H

#include <string>

#include "WWWTree.h"
#include "WWWTreeConnector.h"
#include "WWWTreeLooperAnalysisData.h"
#include "WWWTreeLooperUtil.h"
#include "Ditto.h"

using namespace Ditto;

//-------------------------------------------------------------
// Histogramming functions

void fillHistograms(TString prefix, ObjUtil::AnalysisData* ana_data=0);
void fillHistogramsStandard(TString prefix, ObjUtil::AnalysisData* ana_data);

//	void fillLepWPair(string prefix);
//	void fillHistogramsTruthMatchingLeptons3L(string prefix);
//	void fillHistogramsTruthMatchingLeptons(string prefix);
//	void fillHiggsMassVariables(string prefix);

#endif
