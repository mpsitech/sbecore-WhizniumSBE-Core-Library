/**
  * \file ApiMon.cpp
  * monitoring via API (implementation)
  * \author Alexander Wirthmüller
  * \date created: 25 Jan 2016
  * \date modified: 22 Apr 2020
  */

#include "ApiMon.h"

#include "ApiWzem.h"

using namespace std;
using namespace Sbecore;

/******************************************************************************
 class ApiMon
 ******************************************************************************/

Sbecore::ApiMon::ApiMon() : Mon() {
};

Sbecore::ApiMon::~ApiMon() {
};

void Sbecore::ApiMon::init(
			const string& _Version
			, const string& _ip
			, const uint _port
			, const string& _username
			, const string& _password
		) {
};

void Sbecore::ApiMon::term() {
};

void Sbecore::ApiMon::start() {
};

void Sbecore::ApiMon::stop() {
};

void Sbecore::ApiMon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
			, const bool Dcol
			, const bool Stmgr
		) {
};

void Sbecore::ApiMon::insertClstn(
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

void Sbecore::ApiMon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
};

void Sbecore::ApiMon::insertNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void Sbecore::ApiMon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
		) {
};

void Sbecore::ApiMon::eventRemoveJob(
			const ubigint xjref
		) {
};

void Sbecore::ApiMon::eventAddDcol(
			const ubigint xjref
		) {
};

void Sbecore::ApiMon::eventRemoveDcol(
			const ubigint xjref
		) {
};

void Sbecore::ApiMon::eventAddStmgr(
			const ubigint xjref
		) {
};

void Sbecore::ApiMon::eventRemoveStmgr(
			const ubigint xjref
		) {
};

void Sbecore::ApiMon::eventAddClstn(
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

void Sbecore::ApiMon::eventChangeClstnArg(
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

void Sbecore::ApiMon::eventRemoveClstn(
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

void Sbecore::ApiMon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
};

void Sbecore::ApiMon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
};

void Sbecore::ApiMon::eventRemovePreset(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
};

void Sbecore::ApiMon::eventAddNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
};

void Sbecore::ApiMon::eventRemoveNode(
			const ubigint xnref
		) {
};

Sbecore::ubigint Sbecore::ApiMon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const Arg& argInv
		) {
	return 0;
};

void Sbecore::ApiMon::eventHandleCall(
			const ubigint eref
			, const ubigint xjref
		) {
};

void Sbecore::ApiMon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const Arg& argRet
		) {
};

void Sbecore::ApiMon::eventFinalizeCall(
			const ubigint eref
		) {
};

void Sbecore::ApiMon::eventHandleReqCmd(
			const ubigint xjref
			, const string& Cmd
		) {
};

Sbecore::ubigint Sbecore::ApiMon::eventHandleReqDpchapp(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	return 0;
};

void Sbecore::ApiMon::eventReplyReqDpchapp(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void Sbecore::ApiMon::eventHandleReqUpload(
			const ubigint xjref
			, const string& Filename
		) {
};

Sbecore::ubigint Sbecore::ApiMon::eventHandleReqDownload(
			const ubigint xjref
		) {
	return 0;
};

void Sbecore::ApiMon::eventReplyReqDownload(
			const ubigint eref
			, const ubigint xjref
			, const string Filename
		) {
};

void Sbecore::ApiMon::eventHandleReqDpchret(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};

void Sbecore::ApiMon::eventHandleReqMethod(
			const ubigint xjref
			, const string& srefIxVFeatgroup
			, const string& srefIxVMethod
		) {
};

void Sbecore::ApiMon::eventHandleReqTimer(
			const ubigint xjref
			, const string& xsref
		) {
};

void Sbecore::ApiMon::eventSubmitDpch(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
};

void Sbecore::ApiMon::eventAddInv(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
};
