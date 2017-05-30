/**
  * \file Engtypes.cpp
  * engine data types (implementation)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 6 Oct 2015
  */

#include "Engtypes.h"

/******************************************************************************
 class cref_t
 ******************************************************************************/

cref_t::cref_t(
			const uint ixVCall
			, const ubigint jref
			, const bool stmgr
			, const uint ixVJobmask
			, const ubigint jrefTrig
		) {
	this->ixVCall = ixVCall;
	this->jref = jref;
	this->stmgr = stmgr;
	this->ixVJobmask = ixVJobmask;
	this->jrefTrig = jrefTrig;
};

bool cref_t::operator<(
			const cref_t& comp
		) const {
	if (ixVCall < comp.ixVCall) return true;
	else if (ixVCall > comp.ixVCall) return false;

	if ((jref == 0) || (comp.jref == 0)) return false;
	if (jref < comp.jref) return true;
	else if (jref > comp.jref) return false;

	if (stmgr < comp.stmgr) return true;
	else if (stmgr > comp.stmgr) return false;

	if ((ixVJobmask == 0) || (comp.ixVJobmask == 0)) return false;
	if (ixVJobmask < comp.ixVJobmask) return true;
	else if (ixVJobmask > comp.ixVJobmask) return false;

	if ((jrefTrig == 0) || (comp.jrefTrig == 0)) return false;
	return(jrefTrig < comp.jrefTrig);
};

/******************************************************************************
 class cref2_t
 ******************************************************************************/

cref2_t::cref2_t(
			const ubigint jref
			, const bool stmgr
			, const uint ixVCall
			, const uint ixVJobmask
			, const ubigint jrefTrig
		) {
	this->jref = jref;
	this->stmgr = stmgr;
	this->ixVCall = ixVCall;
	this->ixVJobmask = ixVJobmask;
	this->jrefTrig = jrefTrig;
};

cref2_t::cref2_t(
			const cref_t& cref
		) : cref2_t(cref.jref, cref.stmgr, cref.ixVCall, cref.ixVJobmask, cref.jrefTrig) {
};


bool cref2_t::operator<(
			const cref2_t& comp
		) const {
	if (jref < comp.jref) return true;
	else if (jref > comp.jref) return false;

	if (stmgr < comp.stmgr) return true;
	else if (stmgr > comp.stmgr) return false;

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
 class Clstn
 ******************************************************************************/

Clstn::Clstn(
			const cref_t& cref
			, const Arg& argMask
			, const uint ixVJactype
		) {
	this->cref = cref;
	this->argMask = argMask;
	this->ixVJactype = ixVJactype;
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
 class pref_t
 ******************************************************************************/

pref_t::pref_t(
			const ubigint jref
			, const uint ixVPreset
		) {
	this->jref = jref;
	this->ixVPreset = ixVPreset;
};

bool pref_t::operator<(
			const pref_t& comp
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
			const pref_t& pref
			, const Arg& arg
		) {
	this->pref = pref;
	this->arg = arg;
};
