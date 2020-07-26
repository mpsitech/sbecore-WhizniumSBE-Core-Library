/**
  * \file Mon.cpp
  * monitoring (implementation)
  * \author Alexander Wirthmüller
  * \date created: 24 Jan 2016
  * \date modified: 29 Apr 2020
  */

#include "Mon.h"

using namespace std;

/******************************************************************************
 class xclstnref_t
 ******************************************************************************/

Sbecore::xclstnref_t::xclstnref_t(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const uint ixVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
		) {
	this->xjref = xjref;
	this->srefIxVTarget = srefIxVTarget;
	this->srefIxVCall = srefIxVCall;
	this->ixVJobmask = ixVJobmask;
	this->xjrefTrig = xjrefTrig;
	this->arg = arg;
	this->ixVSge = ixVSge;
};

bool Sbecore::xclstnref_t::operator<(
			const xclstnref_t& comp
		) const {
	// rigged < operator to allow finding existing arg

	if (xjref < comp.xjref) return true;
	if (xjref != comp.xjref) return false;

	if (srefIxVTarget < comp.srefIxVTarget) return true;
	if (srefIxVTarget != comp.srefIxVTarget) return false;

	if (srefIxVCall < comp.srefIxVCall) return true;
	if (srefIxVCall != comp.srefIxVCall) return false;

	if (ixVJobmask < comp.ixVJobmask) return true;
	if (ixVJobmask != comp.ixVJobmask) return false;

	if (xjrefTrig < comp.xjrefTrig) return true;
	if (xjrefTrig != comp.xjrefTrig) return false;

	if ((arg.mask != 0) || (comp.arg.mask != 0)) {
		if (arg.mask < comp.arg.mask) return true;
		if (arg.mask != comp.arg.mask) return false;

		// compare individual components of arg; find functionality available for IX, REF and SREF only
		if (arg.mask & Arg::IX) {
			if ((arg.ix == 0) || (comp.arg.ix == 0)) return false;
			if (arg.ix < comp.arg.ix) return true;
			if (arg.ix != comp.arg.ix) return false;
		};

		if (arg.mask & Arg::REF) {
			if ((arg.ref == 0) || (comp.arg.ref == 0)) return false;
			if (arg.ref < comp.arg.ref) return true;
			if (arg.ref != comp.arg.ref) return false;
		};

		if (arg.mask & Arg::SREF) {
			if ((arg.sref == "") || (comp.arg.sref == "")) return false;
			if (arg.sref < comp.arg.sref) return true;
			if (arg.sref != comp.arg.sref) return false;
		};

		if (arg < comp.arg) return true;
		if (arg != comp.arg) return false;
	};

	return(ixVSge < comp.ixVSge);
};

/******************************************************************************
 class xpresetref_t
 ******************************************************************************/

Sbecore::xpresetref_t::xpresetref_t(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
	this->xjref = xjref;
	this->srefIxVPreset = srefIxVPreset;
};

bool Sbecore::xpresetref_t::operator<(
			const xpresetref_t& comp
		) const {
	if (xjref < comp.xjref) return true;
	if (xjref != comp.xjref) return false;

	if (srefIxVPreset < comp.srefIxVPreset) return true;
	if (srefIxVPreset != comp.srefIxVPreset) return false;

	return false;
};

/******************************************************************************
 class Mon
 ******************************************************************************/

Sbecore::Mon::Mon() :
			mAccess("mAccess", "Mon", "Mon")
		{
	t0 = 0.0;
};

Sbecore::Mon::~Mon() {
};

void Sbecore::Mon::lockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	mAccess.lock(srefObject, srefMember);
};

void Sbecore::Mon::unlockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	mAccess.unlock(srefObject, srefMember);
};

bool Sbecore::Mon::isRunning() {
	return(t0 != 0.0);
};

string Sbecore::Mon::getSquawk(
			const string& srefLocale
		) {
	string s = StrMod::lc(srefLocale);

	if (isRunning()) {
		if (s == "dech") s = "l\\u00e4uft seit " + Ftm::stamp(t0);
		else if (s == "enus") s = "running since " + Ftm::stamp(t0);
		else if (s == "frch") s = "en cours d\\u00e8s " + Ftm::stamp(t0);
	} else {
		if (s == "dech") s = "bereit";
		else if (s == "enus") s = "idle";
		else if (s == "frch") s = "pr\\u00eat";
	};

	return s;
};

double Sbecore::Mon::getDt() {
	timeval t;

	gettimeofday(&t, NULL);

	return((1.0*t.tv_sec + 1e-6*t.tv_usec) - t0);
};

void Sbecore::Mon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
			, const bool Dcol
			, const bool Stmgr
		) {
};

void Sbecore::Mon::insertClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
};

void Sbecore::Mon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
};

void Sbecore::Mon::insertNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void Sbecore::Mon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
		) {
};

void Sbecore::Mon::eventRemoveJob(
			const ubigint xjref
		) {
};

void Sbecore::Mon::eventAddDcol(
			const ubigint xjref
		) {
};

void Sbecore::Mon::eventRemoveDcol(
			const ubigint xjref
		) {
};

void Sbecore::Mon::eventAddStmgr(
			const ubigint xjref
		) {
};

void Sbecore::Mon::eventRemoveStmgr(
			const ubigint xjref
		) {
};

void Sbecore::Mon::eventAddClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
};

void Sbecore::Mon::eventChangeClstnArg(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
};

void Sbecore::Mon::eventRemoveClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
};

void Sbecore::Mon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
};

void Sbecore::Mon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
};

void Sbecore::Mon::eventRemovePreset(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
};

void Sbecore::Mon::eventAddNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void Sbecore::Mon::eventRemoveNode(
			const ubigint xnref
		) {
};

Sbecore::ubigint Sbecore::Mon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const Arg& argInv
		) {
	return 0;
};

void Sbecore::Mon::eventHandleCall(
			const ubigint eref
			, const ubigint xjref
		) {
};

void Sbecore::Mon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const Arg& argRet
		) {
};

void Sbecore::Mon::eventFinalizeCall(
			const ubigint eref
		) {
};

void Sbecore::Mon::eventHandleReqCmd(
			const ubigint xjref
			, const string& Cmd
		) {
};

Sbecore::ubigint Sbecore::Mon::eventHandleReqDpchapp(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	return 0;
};

void Sbecore::Mon::eventReplyReqDpchapp(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void Sbecore::Mon::eventHandleReqUpload(
			const ubigint xjref
			, const string& Filename
		) {
};

Sbecore::ubigint Sbecore::Mon::eventHandleReqDownload(
			const ubigint xjref
		) {
	return 0;
};

void Sbecore::Mon::eventReplyReqDownload(
			const ubigint eref
			, const ubigint xjref
			, const string Filename
		) {
};

void Sbecore::Mon::eventHandleReqDpchret(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void Sbecore::Mon::eventHandleReqMethod(
			const ubigint xjref
			, const string& srefIxVFeatgroup
			, const string& srefIxVMethod
		) {
};

void Sbecore::Mon::eventHandleReqTimer(
			const ubigint xjref
			, const string& xsref
		) {
};

void Sbecore::Mon::eventSubmitDpch(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void Sbecore::Mon::eventAddInv(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};
