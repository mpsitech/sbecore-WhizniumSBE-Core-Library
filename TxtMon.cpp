/**
  * \file TxtMon.cpp
  * monitoring to text file (implementation)
  * \author Alexander Wirthmüller
  * \date created: 26 Jan 2016
  * \date modified: 26 Jan 2016
  */

#include "TxtMon.h"

/******************************************************************************
 class TxtMon
 ******************************************************************************/

TxtMon::TxtMon() : Mon() {
};

TxtMon::~TxtMon() {
};

void TxtMon::start(
			const string& Version
			, const string& monpath) {
};

void TxtMon::stop() {
};

void TxtMon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Master
			, const bool Slave
			, const bool Dcol
			, const bool Stmgr
		) {
};

void TxtMon::insertClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void TxtMon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void TxtMon::insertNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void TxtMon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
		) {
};

void TxtMon::eventRemoveJob(
			const ubigint xjref
		) {
};

void TxtMon::eventAddDcol(
			const ubigint xjref
		) {
};

void TxtMon::eventRemoveDcol(
			const ubigint xjref
		) {
};

void TxtMon::eventAddStmgr(
			const ubigint xjref
		) {
};

void TxtMon::eventRemoveStmgr(
			const ubigint xjref
		) {
};

void TxtMon::eventAddClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void TxtMon::eventChangeClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void TxtMon::eventRemoveClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
		) {
};

void TxtMon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void TxtMon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void TxtMon::eventRemovePreset(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
};

void TxtMon::eventAddNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void TxtMon::eventRemoveNode(
			const ubigint xnref
		) {
};

ubigint TxtMon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& argInv
		) {
	return 0;
};

void TxtMon::eventHandleCall(
			const ubigint eref
			, const ubigint xjref
		) {
};

void TxtMon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const string& argRet
		) {
};

void TxtMon::eventFinalizeCall(
			const ubigint eref
		) {
};

void TxtMon::eventHandleReqCmd(
			const ubigint xjref
			, const string& Cmd
		) {
};

ubigint TxtMon::eventHandleReqRegular(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	return 0;
};

void TxtMon::eventReplyReqRegular(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void TxtMon::eventHandleReqUpload(
			const ubigint xjref
			, const string& Filename
		) {
};

ubigint TxtMon::eventHandleReqDownload(
			const ubigint xjref
		) {
	return 0;
};

void TxtMon::eventReplyReqDownload(
			const ubigint eref
			, const ubigint xjref
			, const string Filename
		) {
};

void TxtMon::eventHandleReqRet(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void TxtMon::eventHandleReqTimer(
			const ubigint xjref
			, const string& xsref
		) {
};

void TxtMon::eventSubmitDpch(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void TxtMon::eventAddInv(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void TxtMon::eventBecomeMaster(
			const ubigint xjref
		) {
};

void TxtMon::eventGiveupMaster(
			const ubigint xjref
		) {
};

void TxtMon::eventBecomeSlave(
			const ubigint xjref
		) {
};

void TxtMon::eventGiveupSlave(
			const ubigint xjref
		) {
};
