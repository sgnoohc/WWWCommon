// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef WWWTreeLooperObjectSelections_H
#define WWWTreeLooperObjectSelections_H

#include "WWWTree.h"
#include "WWWTreeConnector.h"
#include "WWWTreeLooperUtil.h"
#include "WWWTreeLooperAnalysisData.h"
#include "Ditto.h"
using namespace Ditto;

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

#endif
