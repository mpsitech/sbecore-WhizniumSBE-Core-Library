/**
  * \file Engtypes.cpp
  * engine data types (implementation)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 6 Oct 2015
  */

#include "Engtypes.h"

/******************************************************************************
 class clstnref_t
 ******************************************************************************/

clstnref_t::clstnref_t(
			const uint ixVCall
			, const ubigint jref
			, const uint ixVTarget
			, const uint ixVJobmask
			, const ubigint jrefTrig
		) {
	this->ixVCall = ixVCall;
	this->jref = jref;
	this->ixVTarget = ixVTarget;
	this->ixVJobmask = ixVJobmask;
	this->jrefTrig = jrefTrig;
};

bool clstnref_t::operator<(
			const clstnref_t& comp
		) const {
	if (ixVCall < comp.ixVCall) return true;
	else if (ixVCall > comp.ixVCall) return false;

	if ((jref == 0) || (comp.jref == 0)) return false;
	if (jref < comp.jref) return true;
	else if (jref > comp.jref) return false;

	if ((ixVTarget == 0) || (comp.ixVTarget == 0)) return false;
	if (ixVTarget < comp.ixVTarget) return true;
	else if (ixVTarget > comp.ixVTarget) return false;

	if ((ixVJobmask == 0) || (comp.ixVJobmask == 0)) return false;
	if (ixVJobmask < comp.ixVJobmask) return true;
	else if (ixVJobmask > comp.ixVJobmask) return false;

	if ((jrefTrig == 0) || (comp.jrefTrig == 0)) return false;
	return(jrefTrig < comp.jrefTrig);
};

/******************************************************************************
 class clstnref2_t
 ******************************************************************************/

clstnref2_t::clstnref2_t(
			const ubigint jref
			, const uint ixVTarget
			, const uint ixVCall
			, const uint ixVJobmask
			, const ubigint jrefTrig
		) {
	this->jref = jref;
	this->ixVTarget = ixVTarget;
	this->ixVCall = ixVCall;
	this->ixVJobmask = ixVJobmask;
	this->jrefTrig = jrefTrig;
};

clstnref2_t::clstnref2_t(
			const clstnref_t& cref
		) : clstnref2_t(cref.jref, cref.ixVTarget, cref.ixVCall, cref.ixVJobmask, cref.jrefTrig) {
};


bool clstnref2_t::operator<(
			const clstnref2_t& comp
		) const {
	if (jref < comp.jref) return true;
	else if (jref > comp.jref) return false;

	if ((ixVTarget == 0) || (comp.ixVTarget == 0)) return false;
	if (ixVTarget < comp.ixVTarget) return true;
	else if (ixVTarget > comp.ixVTarget) return false;

	if ((ixVCall == 0) || (comp.ixVCall == 0)) return false;
	if (ixVCall < comp.ixVCall) return true;
	else if (ixVCall > comp.ixVCall) return false;

	if ((ixVJobmask == 0) || (comp.ixVJobmask == 0)) return false;
	if (ixVJobmask < comp.ixVJobmask) return true;
	else if (ixVJobmask > comp.ixVJobmask) return false;

	if ((jrefTrig == 0) || (comp.jrefTrig == 0)) return false;
	return(jrefTrig < comp.jrefTrig);
};

/******************************************************************************
 class Call
 ******************************************************************************/

Call::Call(
			const uint ixVCall
			, const ubigint jref
			, const Arg& argInv
		) {
	this->ixVCall = ixVCall;
	this->jref = jref;
	this->argInv = argInv;

	abort = false;
};

/******************************************************************************
 class Clstn::VecVJobmask
 ******************************************************************************/

uint Clstn::VecVJobmask::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("all") == 0) return ALL;
	else if (s.compare("imm") == 0) return IMM;
	else if (s.compare("mast") == 0) return MAST;
	else if (s.compare("self") == 0) return SELF;
	else if (s.compare("slv") == 0) return SLV;
	else if (s.compare("spec") == 0) return SPEC;
	else if (s.compare("tree") == 0) return TREE;

	return 0;
};

string Clstn::VecVJobmask::getSref(
			const uint ix
		) {
	if (ix == ALL) return("all");
	else if (ix == IMM) return("imm");
	else if (ix == MAST) return("mast");
	else if (ix == SELF) return("self");
	else if (ix == SLV) return("slv");
	else if (ix == SPEC) return("spec");
	else if (ix == TREE) return("tree");

	return("");
};

/******************************************************************************
 class Clstn::VecVJactype
 ******************************************************************************/

uint Clstn::VecVJactype::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("lock") == 0) return LOCK;
	else if (s.compare("try") == 0) return TRY;
	else if (s.compare("weak") == 0) return WEAK;

	return 0;
};

string Clstn::VecVJactype::getSref(
			const uint ix
		) {
	if (ix == LOCK) return("lock");
	else if (ix == TRY) return("try");
	else if (ix == WEAK) return("weak");

	return("");
};

/******************************************************************************
 class Clstn::VecVTarget
 ******************************************************************************/

uint Clstn::VecVTarget::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("job") == 0) return JOB;
	else if (s.compare("stmgr") == 0) return STMGR;
	else if (s.compare("ddspub") == 0) return DDSPUB;
	else if (s.compare("uasrv") == 0) return UASRV;

	return 0;
};

string Clstn::VecVTarget::getSref(
			const uint ix
		) {
	if (ix == JOB) return("job");
	else if (ix == STMGR) return("stmgr");
	else if (ix == DDSPUB) return("ddspub");
	else if (ix == UASRV) return("uasrv");

	return("");
};

/******************************************************************************
 class Clstn
 ******************************************************************************/

Clstn::Clstn(
			const clstnref_t& cref
			, const Arg& argMask
			, const uint ixVJactype
		) {
	this->cref = cref;
	this->argMask = argMask;
	this->ixVJactype = ixVJactype;
};

/******************************************************************************
 class featix_t
 ******************************************************************************/

featix_t::featix_t(
			const uint ixVFeatgroup
			, const string& srefIxVFeature
		) {
	this->ixVFeatgroup = ixVFeatgroup;
	this->srefIxVFeature = srefIxVFeature;
};

bool featix_t::operator<(
			const featix_t& comp
		) const {
	if (ixVFeatgroup < comp.ixVFeatgroup) return true;
	else if (ixVFeatgroup > comp.ixVFeatgroup) return false;

	if ((srefIxVFeature == "") || (comp.srefIxVFeature == "")) return false;
	return(srefIxVFeature < comp.srefIxVFeature);
};

/******************************************************************************
 class Method
 ******************************************************************************/

Method::Method(
			const uint ixVFeatgroup
			, const string& srefIxVMethod
		) {
	this->ixVFeatgroup = ixVFeatgroup;
	this->srefIxVMethod = srefIxVMethod;
	
	success = false;
};

/******************************************************************************
 class Msjobinfo
 ******************************************************************************/

Msjobinfo::Msjobinfo(
			const uint ixVJob
		) {
	this->ixVJob = ixVJob;

	jrefMast = 0;
	
	ixVSge = 1;
};

/******************************************************************************
 namespace VecOpVOpres
 ******************************************************************************/

uint VecOpVOpres::getIx(
			const string& sref
		) {
	if (sref.compare("progress") == 0) return PROGRESS;
	else if (sref.compare("success") == 0) return SUCCESS;
	else if (sref.compare("failure") == 0) return FAILURE;
	else if (sref.compare("commerr") == 0) return COMMERR;

	return(0);
};

string VecOpVOpres::getSref(
			const uint ix
		) {
	if (ix == PROGRESS) return("progress");
	else if (ix == SUCCESS) return("success");
	else if (ix == FAILURE) return("failure");
	else if (ix == COMMERR) return("commerr");

	return("");
};

/******************************************************************************
 class Op
 ******************************************************************************/

Op::Op(
			const ubigint oref
			, const uint ixVDpch
			, const string& squawk
		) {
	this->oref = oref;
	this->ixVDpch = ixVDpch;
	this->squawk = squawk;
};

/******************************************************************************
 class presetref_t
 ******************************************************************************/

presetref_t::presetref_t(
			const ubigint jref
			, const uint ixVPreset
		) {
	this->jref = jref;
	this->ixVPreset = ixVPreset;
};

bool presetref_t::operator<(
			const presetref_t& comp
		) const {
	if (jref < comp.jref) return true;
	else if (jref > comp.jref) return false;

	if ((ixVPreset == 0) || (comp.ixVPreset == 0)) return false;
	return(ixVPreset < comp.ixVPreset);
};

/******************************************************************************
 class Preset
 ******************************************************************************/

Preset::Preset(
			const presetref_t& pref
			, const Arg& arg
		) {
	this->pref = pref;
	this->arg = arg;
};
