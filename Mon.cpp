/**
  * \file Mon.cpp
  * monitoring (implementation)
  * \author Alexander Wirthmüller
  * \date created: 24 Jan 2016
  * \date modified: 26 Jan 2016
  */

#include "Mon.h"

/******************************************************************************
 class xcref_t
 ******************************************************************************/

xcref_t::xcref_t(
			const ubigint xjref
			, const bool stmgr
			, const string& srefIxVCall
			, const uint ixVJobmask
			, const ubigint xjrefTrig
		) {
	this->xjref = xjref;
	this->stmgr = stmgr;
	this->srefIxVCall = srefIxVCall;
	this->ixVJobmask = ixVJobmask;
	this->xjrefTrig = xjrefTrig;
};

bool xcref_t::operator<(
			const xcref_t& comp
		) const {
	if (xjref < comp.xjref) return true;
	else if (xjref > comp.xjref) return false;

	if (stmgr < comp.stmgr) return true;
	else if (stmgr > comp.stmgr) return false;

	if (srefIxVCall < comp.srefIxVCall) return true;
	else if (srefIxVCall > comp.srefIxVCall) return false;

	if (ixVJobmask < comp.ixVJobmask) return true;
	else if (ixVJobmask > comp.ixVJobmask) return false;

	if (xjrefTrig < comp.xjrefTrig) return true;
	else if (xjrefTrig > comp.xjrefTrig) return false;

	return false;
};

/******************************************************************************
 class xpref_t
 ******************************************************************************/

xpref_t::xpref_t(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
	this->xjref = xjref;
	this->srefIxVPreset = srefIxVPreset;
};

bool xpref_t::operator<(
			const xpref_t& comp
		) const {
	if (xjref < comp.xjref) return true;
	else if (xjref > comp.xjref) return false;

	if (srefIxVPreset < comp.srefIxVPreset) return true;
	else if (srefIxVPreset > comp.srefIxVPreset) return false;

	return false;
};

/******************************************************************************
 class Mon
 ******************************************************************************/

Mon::Mon() {
	Mutex::init(&mAccess, true, "mAccess", "Mon", "Mon");

	t0 = 0.0;
};

Mon::~Mon() {
	Mutex::destroy(&mAccess, true, "mAccess", "Mon", "~Mon");
};

int Mon::lockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	return Mutex::lock(&mAccess, "mAccess", srefObject, srefMember);
};

int Mon::unlockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	return Mutex::unlock(&mAccess, "mAccess", srefObject, srefMember);
};

double Mon::getDt() {
	timeval t;

	gettimeofday(&t, NULL);

	return((1.0*t.tv_sec + 1e-6*t.tv_usec) - t0);
};

void Mon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Master
			, const bool Slave
			, const bool Dcol
			, const bool Stmgr
		) {
};

void Mon::insertClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void Mon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void Mon::insertNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void Mon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
		) {
};

void Mon::eventRemoveJob(
			const ubigint xjref
		) {
};

void Mon::eventAddDcol(
			const ubigint xjref
		) {
};

void Mon::eventRemoveDcol(
			const ubigint xjref
		) {
};

void Mon::eventAddStmgr(
			const ubigint xjref
		) {
};

void Mon::eventRemoveStmgr(
			const ubigint xjref
		) {
};

void Mon::eventAddClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void Mon::eventChangeClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void Mon::eventRemoveClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
		) {
};

void Mon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void Mon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void Mon::eventRemovePreset(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
};

void Mon::eventAddNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void Mon::eventRemoveNode(
			const ubigint xnref
		) {
};

ubigint Mon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& argInv
		) {
	return 0;
};

void Mon::eventHandleCall(
			const ubigint eref
			, const ubigint xjref
		) {
};

void Mon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const string& argRet
		) {
};

void Mon::eventFinalizeCall(
			const ubigint eref
		) {
};

void Mon::eventHandleReqCmd(
			const ubigint xjref
			, const string& Cmd
		) {
};

ubigint Mon::eventHandleReqRegular(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	return 0;
};

void Mon::eventReplyReqRegular(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void Mon::eventHandleReqUpload(
			const ubigint xjref
			, const string& Filename
		) {
};

ubigint Mon::eventHandleReqDownload(
			const ubigint xjref
		) {
	return 0;
};

void Mon::eventReplyReqDownload(
			const ubigint eref
			, const ubigint xjref
			, const string Filename
		) {
};

void Mon::eventHandleReqRet(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void Mon::eventHandleReqTimer(
			const ubigint xjref
			, const string& xsref
		) {
};

void Mon::eventSubmitDpch(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void Mon::eventAddInv(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void Mon::eventBecomeMaster(
			const ubigint xjref
		) {
};

void Mon::eventGiveupMaster(
			const ubigint xjref
		) {
};

void Mon::eventBecomeSlave(
			const ubigint xjref
		) {
};

void Mon::eventGiveupSlave(
			const ubigint xjref
		) {
};
