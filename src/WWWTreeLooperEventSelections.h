// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef WWWTreeLooperEventSelections_H
#define WWWTreeLooperEventSelections_H

#include <functional>
#include <vector>
#include "WWWTree.h"
#include "WWWTreeLooperUtil.h"
#include "Ditto.h"
using namespace Ditto;

extern CutUtil::Cuts ss_common_cuts;
extern CutUtil::Cuts ssmm_cuts;
extern CutUtil::Cuts ssem_cuts;
extern CutUtil::Cuts ssee_cuts;

void loadEventSelections();
void loadSSSelections();
void loadSSCommonSelections();
void loadSSmmSelections();
void loadSSemSelections();
void loadSSeeSelections();

void SS_NLepEqTwoCut(CutUtil::CutData&);
void SS_FlavorChan(int pdgidprod, CutUtil::CutData&);
void SS_Lep0PtCut(CutUtil::CutData&);
void SS_Lep1PtCut(CutUtil::CutData&);
void SS_NJetCut(CutUtil::CutData&);
void SS_Jet0PtCut(CutUtil::CutData&);
void SS_ThirdLepVeto(CutUtil::CutData&);
void SS_IsoTrackVeto(CutUtil::CutData&);
void SS_WmassCut(CutUtil::CutData&);
void SS_MjjCut(CutUtil::CutData&);
void SS_DEtajjCut(CutUtil::CutData&);
void SS_BVeto(CutUtil::CutData&);
void SS_MllCut(float, CutUtil::CutData&);
void SS_METCut(float, CutUtil::CutData&);
void SS_ZVeto(CutUtil::CutData&);

#endif
