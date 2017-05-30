/**
  * \file ApiMon.cpp
  * monitoring via API (implementation)
  * \author Alexander Wirthmüller
  * \date created: 25 Jan 2016
  * \date modified: 26 Jan 2016
  */

#include "Awem.h"

#include "ApiMon.h"

/******************************************************************************
 class ApiMon
 ******************************************************************************/

ApiMon::ApiMon() : Mon() {
};

ApiMon::~ApiMon() {
};

void ApiMon::init(
			const string& _Version
			, const string& _ip
			, const uint _port
			, const string& _username
			, const string& _password
		) {
};

void ApiMon::term() {
};

void ApiMon::start() {
};

void ApiMon::stop() {
};

void ApiMon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Master
			, const bool Slave
			, const bool Dcol
			, const bool Stmgr
		) {
};

void ApiMon::insertClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void ApiMon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void ApiMon::insertNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void ApiMon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
		) {
};

void ApiMon::eventRemoveJob(
			const ubigint xjref
		) {
};

void ApiMon::eventAddDcol(
			const ubigint xjref
		) {
};

void ApiMon::eventRemoveDcol(
			const ubigint xjref
		) {
};

void ApiMon::eventAddStmgr(
			const ubigint xjref
		) {
};

void ApiMon::eventRemoveStmgr(
			const ubigint xjref
		) {
};

void ApiMon::eventAddClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void ApiMon::eventChangeClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
};

void ApiMon::eventRemoveClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const bool Stmgr
			, const string& srefIxVJobmask
			, const ubigint trgXjref
		) {
};

void ApiMon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void ApiMon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
};

void ApiMon::eventRemovePreset(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
};

void ApiMon::eventAddNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void ApiMon::eventRemoveNode(
			const ubigint xnref
		) {
};

ubigint ApiMon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& argInv
		) {
	return 0;
};

void ApiMon::eventHandleCall(
			const ubigint eref
			, const ubigint xjref
		) {
};

void ApiMon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const string& argRet
		) {
};

void ApiMon::eventFinalizeCall(
			const ubigint eref
		) {
};

void ApiMon::eventHandleReqCmd(
			const ubigint xjref
			, const string& Cmd
		) {
};

ubigint ApiMon::eventHandleReqRegular(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	return 0;
};

void ApiMon::eventReplyReqRegular(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void ApiMon::eventHandleReqUpload(
			const ubigint xjref
			, const string& Filename
		) {
};

ubigint ApiMon::eventHandleReqDownload(
			const ubigint xjref
		) {
	return 0;
};

void ApiMon::eventReplyReqDownload(
			const ubigint eref
			, const ubigint xjref
			, const string Filename
		) {
};

void ApiMon::eventHandleReqRet(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void ApiMon::eventHandleReqTimer(
			const ubigint xjref
			, const string& xsref
		) {
};

void ApiMon::eventSubmitDpch(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void ApiMon::eventAddInv(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void ApiMon::eventBecomeMaster(
			const ubigint xjref
		) {
};

void ApiMon::eventGiveupMaster(
			const ubigint xjref
		) {
};

void ApiMon::eventBecomeSlave(
			const ubigint xjref
		) {
};

void ApiMon::eventGiveupSlave(
			const ubigint xjref
		) {
};
