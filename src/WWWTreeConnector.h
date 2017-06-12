// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef WWWTreeConnector_h
#define WWWTreeConnector_h

#include "Ditto.h"
#include "WWWTree.h"
#include "WWWTreeLooperUtil.h"

using namespace Ditto;

ObjUtil::Leptons getLeptons(/*WWWTree& mytree*/);
ObjUtil::Jets getJets(/*WWWTree& mytree*/);
ObjUtil::Jets getRemovedJets(/*WWWTree& mytree*/);
ObjUtil::Jets getBJets(/*WWWTree& mytree*/);
ObjUtil::METs getMETs(/*WWWTree& mytree*/);
ObjUtil::MET getMET(/*WWWTree& mytree*/);

#endif
//eof
