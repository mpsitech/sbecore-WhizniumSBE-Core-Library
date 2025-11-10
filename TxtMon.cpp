/**
	* \file TxtMon.cpp
	* engine monitoring to text file (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 26 Jan 2016
	*/

#include "TxtMon.h"

using namespace std;
using namespace Sbecore;

/******************************************************************************
 class TxtMon
 ******************************************************************************/

Sbecore::TxtMon::TxtMon() :
			Mon()
			, clstnseq("clstnseq")
			, presetseq("presetseq")
			, erefseq("erefseq")
		{
};

Sbecore::TxtMon::~TxtMon() {
	if (monfile.is_open()) stop();
};

void Sbecore::TxtMon::start(
			const string& Version
			, const string& monpath
		) {
	lockAccess("TxtMon", "start");

	string s;
	
	if (monfile.is_open()) stop();

	t0 = getDt();

	s = monpath + "/period_" + to_string(lround(t0)) + ".txt";
	monfile.open(s.c_str(), ios::out);
	if (!monfile.is_open()) return;

	monfile << "[" << Ftm::stamp(lround(t0)) << "] starting monitoring period for version " << Version << endl;

	unlockAccess("TxtMon", "start");
};

void Sbecore::TxtMon::stop() {
	lockAccess("TxtMon", "stop");

	if (monfile.is_open()) {
		monfile << "[" << Ftm::stamp(lround(t0 + getDt())) << "] stopping monitoring period" << endl;
		
		monfile.close();

		clstnseq.ref = 0;
		refsClstn.clear();
		presetseq.ref = 0;
		refsPreset.clear();

		t0 = 0.0;
	};

	unlockAccess("TxtMon", "stop");
};

void Sbecore::TxtMon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
			, const bool Dcol
			, const bool Stmgr
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "insertJob");

	monfile << "job/" << xjref << ": inserted " << srefIxVJob;
	if (xjref != 0) monfile << " as sub-job to " << supXjref;
	if (Clisrv) {
		if (!srvNotCli) monfile << " as client";
		else monfile << " as server";
	};
	if (Dcol) monfile << " with dispatch collector";
	if (Stmgr) monfile << " with stub manager";
	monfile << endl;

	unlockAccess("TxtMon", "insertJob");
};

void Sbecore::TxtMon::insertClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "insertClstn");

	ubigint ref = clstnseq.getNewRef();
	
	string s;
	
	monfile << "call listener/" << ref << ": inserted call listener for " << srefIxVTarget;
	if (xjref != 0) monfile << "/" << xjref;
	monfile << " to call " << srefIxVCall;
	monfile << " with triggering job mask " << srefIxVJobmask;
	if (xjrefTrig != 0) monfile << "/" << xjrefTrig;
	s = arg.to_string();
	if (s != "(empty)") monfile << " with argument mask " << s;
	if (ixVSge != 0) monfile << " with stage mask " << ixVSge;
	monfile << "; job access type: " << srefIxVJactype;
	monfile << endl;

	unlockAccess("TxtMon", "insertClstn");
};

void Sbecore::TxtMon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "insertPreset");

	ubigint ref = presetseq.getNewRef();
	
	monfile << "presetting/" << ref << ": inserted presetting " << srefIxVPreset << " for job " << xjref << " with argument " << arg.to_string() << endl;

	unlockAccess("TxtMon", "insertPreset");
};

void Sbecore::TxtMon::insertNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "insertNode");

	monfile << "node/" << xnref << ": inserted node " << Ip << ":" << Port << " with " << ((uint) Opprcn) << " operation processors" << endl;

	unlockAccess("TxtMon", "insertNode");
};

void Sbecore::TxtMon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventAddJob");

	monfile << "[" << getDt() << "] job/" << xjref << ": added " << srefIxVJob;
	if (xjref != 0) monfile << " as sub-job to " << supXjref;
	if (Clisrv) {
		if (!srvNotCli) monfile << " as client";
		else monfile << " as server";
	};
	monfile << endl;

	unlockAccess("TxtMon", "eventAddJob");
};

void Sbecore::TxtMon::eventRemoveJob(
			const ubigint xjref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventRemoveJob");

	monfile << "[" << getDt() << "] job/" << xjref << ": removed job" << endl;

	unlockAccess("TxtMon", "eventRemoveJob");
};

void Sbecore::TxtMon::eventAddDcol(
			const ubigint xjref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventAddDcol");

	monfile << "[" << getDt() << "] job/" << xjref << ": added dispatch collector" << endl;

	unlockAccess("TxtMon", "eventAddDcol");
};

void Sbecore::TxtMon::eventRemoveDcol(
			const ubigint xjref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventRemoveDcol");

	monfile << "[" << getDt() << "] job/" << xjref << ": removed dispatch collector" << endl;

	unlockAccess("TxtMon", "eventRemoveDcol");
};

void Sbecore::TxtMon::eventAddStmgr(
			const ubigint xjref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventAddStmgr");

	monfile << "[" << getDt() << "] job/" << xjref << ": added stub manager" << endl;

	unlockAccess("TxtMon", "eventAddStmgr");
};

void Sbecore::TxtMon::eventRemoveStmgr(
			const ubigint xjref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventRemoveStmgr");

	monfile << "[" << getDt() << "] job/" << xjref << ": removed stub manager" << endl;

	unlockAccess("TxtMon", "eventRemoveStmgr");
};

void Sbecore::TxtMon::eventAddClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventAddClstn");

	ubigint ref = clstnseq.getNewRef();
	
	string s;

	monfile << "[" << getDt() << "] call listener/" << ref << ": added call listener for " << srefIxVTarget;
	if (xjref != 0) monfile << "/" << xjref;
	monfile << " to call " << srefIxVCall;
	monfile << " with triggering job mask " << srefIxVJobmask;
	if (xjrefTrig != 0) monfile << "/" << xjrefTrig;
	s = arg.to_string();
	if (s != "(empty)") monfile << " with argument mask " << s;
	if (ixVSge != 0) monfile << " with stage mask " << ixVSge;
	monfile << "; job access type: " << srefIxVJactype;
	monfile << endl;

	unlockAccess("TxtMon", "eventAddClstn");
};

void Sbecore::TxtMon::eventChangeClstnArg(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventChangeClstnArg");

	Arg argFind;

	argFind = arg;
	argFind.clearContent();

	auto it = refsClstn.find(xclstnref_t(xjref, srefIxVTarget, srefIxVCall, Clstn::VecVJobmask::getIx(srefIxVJobmask), xjrefTrig, argFind, ixVSge));
	
	if (it != refsClstn.end()) {
		monfile << "[" << getDt() << "] call listener/" << it->second << ": changed argument mask to " << arg.to_string() << endl;
	};

	unlockAccess("TxtMon", "eventChangeClstnArg");
};

void Sbecore::TxtMon::eventRemoveClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventRemoveClstn");

	Arg argFind;

	argFind = arg;
	argFind.clearContent();

	auto it = refsClstn.find(xclstnref_t(xjref, srefIxVTarget, srefIxVCall, Clstn::VecVJobmask::getIx(srefIxVJobmask), xjrefTrig, argFind, ixVSge));
	
	if (it != refsClstn.end()) {
		monfile << "[" << getDt() << "] call listener/" << it->second << ": removed call listener" << endl;
	};

	unlockAccess("TxtMon", "eventRemoveClstn");
};

void Sbecore::TxtMon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventAddPreset");

	ubigint ref = presetseq.getNewRef();
	
	monfile << "[" << getDt() << "] presetting/" << ref << ": added presetting " << srefIxVPreset << " for job " << xjref << " with argument " << arg.to_string() << endl;

	unlockAccess("TxtMon", "eventAddPreset");
};

void Sbecore::TxtMon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventChangePreset");

	auto it = refsPreset.find(xpresetref_t(xjref, srefIxVPreset));

	if (it != refsPreset.end()) {
		monfile << "[" << getDt() << "] presetting/" << it->second << ": changed presetting argument to " << arg.to_string() << endl;
	};

	unlockAccess("TxtMon", "eventChangePreset");
};

void Sbecore::TxtMon::eventRemovePreset(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventRemovePreset");

	auto it = refsPreset.find(xpresetref_t(xjref, srefIxVPreset));

	if (it != refsPreset.end()) {
		monfile << "[" << getDt() << "] presetting/" << it->second << ": removed presetting" << endl;
	};

	unlockAccess("TxtMon", "eventRemovePreset");
};

void Sbecore::TxtMon::eventAddNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventAddNode");

	monfile << "[" << getDt() << "] node/" << xnref << ": added node " << Ip << ":" << Port << " with " << ((uint) Opprcn) << " operation processors" << endl;

	unlockAccess("TxtMon", "eventAddNode");
};

void Sbecore::TxtMon::eventRemoveNode(
			const ubigint xnref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventRemoveNode");

	monfile << "[" << getDt() << "] node/" << xnref << ": removed node" << endl;

	unlockAccess("TxtMon", "eventRemoveNode");
};

Sbecore::ubigint Sbecore::TxtMon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const Arg& argInv
		) {
	if (!monfile.is_open()) return 0;
	lockAccess("TxtMon", "eventTriggerCall");

	ubigint eref = erefseq.getNewRef();

	monfile << "[" << getDt() << "] call/" << eref << ": triggering call " << srefIxVCall;
	if (xjref != 0) monfile << " from job " << xjref;
	monfile << " with argument " << argInv.to_string() << endl;

	unlockAccess("TxtMon", "eventTriggerCall");
	
	return eref;
};

void Sbecore::TxtMon::eventHandleCall(
			const ubigint eref
			, const ubigint xjref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventHandleCall");

	monfile << "[" << getDt() << "] call/" << eref << ": handling call in job " << xjref << endl;

	unlockAccess("TxtMon", "eventHandleCall");
};

void Sbecore::TxtMon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const Arg& argRet
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventRetCall");

	monfile << "[" << getDt() << "] call/" << eref << ": job " << xjref << " returning argument " << argRet.to_string() << endl;

	unlockAccess("TxtMon", "eventRetCall");
};

void Sbecore::TxtMon::eventFinalizeCall(
			const ubigint eref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventFinalizeCall");

	monfile << "[" << getDt() << "] call/" << eref << ": finalizing call" << endl;

	unlockAccess("TxtMon", "eventFinalizeCall");
};

void Sbecore::TxtMon::eventHandleReqCmd(
			const ubigint xjref
			, const string& Cmd
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventHandleReqCmd");

	monfile << "[" << getDt() << "] job/" << xjref << ": handling command request " << Cmd << endl;

	unlockAccess("TxtMon", "eventHandleReqCmd");
};

Sbecore::ubigint Sbecore::TxtMon::eventHandleReqDpchapp(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!monfile.is_open()) return 0;
	lockAccess("TxtMon", "eventHandleReqDpchapp");

	ubigint eref = erefseq.getNewRef();

	monfile << "[" << getDt() << "] app request/" << eref << ": job " << xjref << " handling app dispatch " << srefIxVDpch << "{" << srefsMask << "}" << endl;

	unlockAccess("TxtMon", "eventHandleReqDpchapp");

	return eref;
};

void Sbecore::TxtMon::eventReplyReqDpchapp(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventReplyReqDpchapp");

	monfile << "[" << getDt() << "] app request/" << eref << ": job " << xjref << " replying with engine dispatch " << srefIxVDpch << "{" << srefsMask << "}" << endl;

	unlockAccess("TxtMon", "eventReplyReqDpchapp");
};

void Sbecore::TxtMon::eventHandleReqUpload(
			const ubigint xjref
			, const string& Filename
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventHandleReqUpload");

	monfile << "[" << getDt() << "] upload request: job " << xjref << " handling upload of file " << Filename << endl;

	unlockAccess("TxtMon", "eventHandleReqUpload");
};

Sbecore::ubigint Sbecore::TxtMon::eventHandleReqDownload(
			const ubigint xjref
		) {
	if (!monfile.is_open()) return 0;
	lockAccess("TxtMon", "eventHandleReqDownload");

	ubigint eref = erefseq.getNewRef();

	monfile << "[" << getDt() << "] download request/" << eref << ": job " << xjref << " handling download" << endl;

	unlockAccess("TxtMon", "eventHandleReqDownload");
	
	return eref;
};

void Sbecore::TxtMon::eventReplyReqDownload(
			const ubigint eref
			, const ubigint xjref
			, const string Filename
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventReplyReqDownload");

	monfile << "[" << getDt() << "] download request/" << eref << ": job " << xjref << " replying with file " << Filename << endl;

	unlockAccess("TxtMon", "eventReplyReqDownload");
};

void Sbecore::TxtMon::eventHandleReqDpchret(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventHandleReqDpchret");

	monfile << "[" << getDt() << "] operation/" << xoref << ": job " << xjref << " handling operation return request " << srefIxVDpch << endl;

	unlockAccess("TxtMon", "eventHandleReqDpchret");
};

void Sbecore::TxtMon::eventHandleReqMethod(
			const ubigint xjref
			, const string& srefIxVFeatgroup
			, const string& srefIxVMethod
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventHandleReqMethod");

	monfile << "[" << getDt() << "] job/" << xjref << ": handling method request " << srefIxVFeatgroup << "." << srefIxVMethod << endl;

	unlockAccess("TxtMon", "eventHandleReqMethod");
};

void Sbecore::TxtMon::eventHandleReqTimer(
			const ubigint xjref
			, const string& xsref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventHandleReqTimer");

	monfile << "[" << getDt() << "] job/" << xjref << ": handling timer request with identifier " << xsref << endl;

	unlockAccess("TxtMon", "eventHandleReqTimer");
};

void Sbecore::TxtMon::eventSubmitDpch(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventSubmitDpch");

	monfile << "[" << getDt() << "] job/" << xjref << ": submitting engine dispatch " << srefIxVDpch << "{" << srefsMask << "}" << endl;

	unlockAccess("TxtMon", "eventSubmitDpch");
};

void Sbecore::TxtMon::eventAddInv(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
	if (!monfile.is_open()) return;
	lockAccess("TxtMon", "eventAddInv");

	monfile << "[" << getDt() << "] operation/" << xoref << ": job " << xjref << " adding operation invocation request " << srefIxVDpch << endl;

	unlockAccess("TxtMon", "eventAddInv");
};
