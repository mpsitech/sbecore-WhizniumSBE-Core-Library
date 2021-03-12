/**
	* \file DbsMon.cpp
	* engine monitoring to database (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 24 Jan 2016
	*/

#include "DbsMon.h"

#include "DbsWzem.h"

using namespace std;
using namespace Sbecore;

/******************************************************************************
 class DbsMon
 ******************************************************************************/

Sbecore::DbsMon::DbsMon() : Mon() {
	dbswzem = new DbsWzem();

	prd = NULL;
};

Sbecore::DbsMon::~DbsMon() {
	if (prd) stop();

	delete dbswzem;
};

void Sbecore::DbsMon::start(
			const string& Version
			, const uint ixDbsVDbstype
			, const string& dbspath
			, const string& dbsname
			, const string& ip
			, const uint port
			, const string& dbsusername
			, const string& dbspassword
			, const string& username
			, const string& password
		) {
	lockAccess("DbsMon", "start");

	if (prd) stop();

	try {
		dbswzem->init(ixDbsVDbstype, dbspath, dbsname, ip, port, dbsusername, dbspassword);

	} catch (SbeException& e) {
		unlockAccess("DbsMon", "start");
		return;
	};

	WzemMUser* usr = NULL;

	if (dbswzem->tblwzemmuser->loadRecBySQL("SELECT * FROM TblWzemMUser WHERE sref = '" + username + "' AND Password = '" + password + "'", &usr)) {
		t0 = getDt();

		dbswzem->tblwzemmperiod->insertNewRec(&prd, usr->ref, usr->refWzemMUsergroup, Version, lround(t0), 0);
		delete usr;
	};

	unlockAccess("DbsMon", "start");
};

void Sbecore::DbsMon::stop() {
	lockAccess("DbsMon", "stop");

	if (prd) {
		prd->stop = lround(t0 + getDt());
		dbswzem->tblwzemmperiod->updateRec(prd);

		dbswzem->term();

		delete prd;
		prd = NULL;

		refsClstn.clear();
		refsEvent.clear();
		refsJob.clear();
		refsNode.clear();
		refsOp.clear();
		refsPreset.clear();

		t0 = 0.0;
	};

	unlockAccess("DbsMon", "stop");
};

void Sbecore::DbsMon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
			, const bool Dcol
			, const bool Stmgr
		) {
	if (!prd) return;
	lockAccess("DbsMon", "insertJob");

	ubigint ref;

	ubigint supRefWzemMJob = 0;

	auto it = refsJob.find(supXjref);
	if (it != refsJob.end()) supRefWzemMJob = it->second;

	ref = dbswzem->tblwzemmjob->insertNewRec(NULL, 0.0, 0.0, prd->ref, supRefWzemMJob, srefIxVJob, xjref, Clisrv, srvNotCli, Dcol, Stmgr);
	refsJob[xjref] = ref;

	dbswzem->tblwzemjmjobdcol->insertNewRec(NULL, ref, 0.0, 0.0, Dcol);
	dbswzem->tblwzemjmjobstmgr->insertNewRec(NULL, ref, 0.0, 0.0, Stmgr);

	unlockAccess("DbsMon", "insertJob");
};

void Sbecore::DbsMon::insertClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!prd) return;
	lockAccess("DbsMon", "insertClstn");

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint trgRefWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		it = refsJob.find(xjrefTrig);
		if (it != refsJob.end()) trgRefWzemMJob = it->second;

		ref = dbswzem->tblwzemmclstn->insertNewRec(NULL, 0.0, 0.0, refWzemMJob, srefIxVCall, VecWzemVMClstnTarget::getIx(srefIxVTarget), VecWzemVMClstnJobmask::getIx(srefIxVJobmask), trgRefWzemMJob, VecWzemWMClstnArgmask::getIx(arg.getMaskSrefs()), arg.to_string(), to_string(ixVSge), VecWzemVMClstnJactype::getIx(srefIxVJactype));
		refsClstn[xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), xjrefTrig, arg, ixVSge)] = ref;

		dbswzem->tblwzemjmclstn->insertNewRec(NULL, ref, 0.0, 0.0, arg.to_string());
	};

	unlockAccess("DbsMon", "insertClstn");
};

void Sbecore::DbsMon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
	if (!prd) return;
	lockAccess("DbsMon", "insertPreset");

	ubigint ref;

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmpreset->insertNewRec(NULL, 0.0, 0.0, refWzemMJob, srefIxVPreset, arg.to_string());
		refsPreset[xpresetref_t(xjref, srefIxVPreset)] = ref;

		dbswzem->tblwzemjmpresetarg->insertNewRec(NULL, ref, 0.0, 0.0, arg.to_string());
	};

	unlockAccess("DbsMon", "insertPreset");
};

void Sbecore::DbsMon::insertNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
	if (!prd) return;
	lockAccess("DbsMon", "insertNode");

	ubigint ref;

	ref = dbswzem->tblwzemmnode->insertNewRec(NULL, 0.0, 0.0, prd->ref, xnref, Ip, Port, Opprcn);
	refsNode[xnref] = ref;

	unlockAccess("DbsMon", "insertNode");
};

void Sbecore::DbsMon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Clisrv
			, const bool srvNotCli
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddJob");

	double dt = getDt();

	ubigint ref;

	ubigint supRefWzemMJob = 0;

	auto it = refsJob.find(supXjref);
	if (it != refsJob.end()) supRefWzemMJob = it->second;

	ref = dbswzem->tblwzemmjob->insertNewRec(NULL, dt, 0.0, prd->ref, supRefWzemMJob, srefIxVJob, xjref, Clisrv, srvNotCli, false, false);
	refsJob[xjref] = ref;

	dbswzem->tblwzemjmjobdcol->insertNewRec(NULL, ref, dt, 0.0, false);
	dbswzem->tblwzemjmjobstmgr->insertNewRec(NULL, ref, dt, 0.0, false);

	dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDJOB, 0, prd->ref, 0, dt, 0, 0, ref, 0, 0, 0, "", "", "", "", "");

	unlockAccess("DbsMon", "eventAddJob");
};

void Sbecore::DbsMon::eventRemoveJob(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRemoveJob");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint refWzemMEvent = 0;
	ubigint refWzemCEvent = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemMJob SET x1Stopu = " + to_string(dt) + " WHERE ref = " + to_string(refWzemMJob));

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::REMOVEJOB, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		if (dbswzem->loadRefBySQL("SELECT ref FROM TblWzemMEvent WHERE ixVBasetype = " + to_string(VecWzemVMEventBasetype::ADDJOB) + " AND refWzemMPeriod = " + to_string(prd->ref)
					+ " AND refWzemMJob = " + to_string(refWzemMJob), refWzemMEvent)) {

			refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(refWzemMEvent));
			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));
		};
	};

	unlockAccess("DbsMon", "eventRemoveJob");
};

void Sbecore::DbsMon::eventAddDcol(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddDcol");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJobDcol SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjobdcol->insertNewRec(NULL, refWzemMJob, dt, 0.0, true);

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDDCOL, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventAddDcol");
};

void Sbecore::DbsMon::eventRemoveDcol(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRemoveDcol");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint refWzemMEvent = 0;
	ubigint refWzemCEvent = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJobDcol SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjobdcol->insertNewRec(NULL, refWzemMJob, dt, 0.0, false);

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::REMOVEDCOL, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		if (dbswzem->loadRefBySQL("SELECT ref FROM TblWzemMEvent WHERE ixVBasetype = " + to_string(VecWzemVMEventBasetype::ADDDCOL) + " AND refWzemMPeriod = " + to_string(prd->ref)
					+ " AND refWzemMJob = " + to_string(refWzemMJob) + " ORDER BY startu DESC LIMIT 1", refWzemMEvent)) {

			refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(refWzemMEvent));
			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));
		};
	};

	unlockAccess("DbsMon", "eventRemoveDcol");
};

void Sbecore::DbsMon::eventAddStmgr(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddStmgr");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJobStmgr SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjobstmgr->insertNewRec(NULL, refWzemMJob, dt, 0.0, true);

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDSTMGR, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventAddStmgr");
};

void Sbecore::DbsMon::eventRemoveStmgr(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRemoveStmgr");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint refWzemMEvent = 0;
	ubigint refWzemCEvent = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJobStmgr SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjobstmgr->insertNewRec(NULL, refWzemMJob, dt, 0.0, false);

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::REMOVESTMGR, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		if (dbswzem->loadRefBySQL("SELECT ref FROM TblWzemMEvent WHERE ixVBasetype = " + to_string(VecWzemVMEventBasetype::ADDSTMGR) + " AND refWzemMPeriod = " + to_string(prd->ref)
					+ " AND refWzemMJob = " + to_string(refWzemMJob) + " ORDER BY startu DESC LIMIT 1", refWzemMEvent)) {

			refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(refWzemMEvent));
			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));
		};
	};

	unlockAccess("DbsMon", "eventRemoveStmgr");
};

void Sbecore::DbsMon::eventAddClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddClstn");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint tjbRefWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		it = refsJob.find(xjrefTrig);
		if (it != refsJob.end()) tjbRefWzemMJob = it->second;

		ref = dbswzem->tblwzemmclstn->insertNewRec(NULL, dt, 0.0, refWzemMJob, srefIxVCall, VecWzemVMClstnTarget::getIx(srefIxVTarget), VecWzemVMClstnJobmask::getIx(srefIxVJobmask), tjbRefWzemMJob, VecWzemWMClstnArgmask::getIx(arg.getMaskSrefs()), arg.to_string(), to_string(ixVSge), VecWzemVMClstnJactype::getIx(srefIxVJactype));
		refsClstn[xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), xjrefTrig, arg, ixVSge)] = ref;

		dbswzem->tblwzemjmclstn->insertNewRec(NULL, ref, dt, 0.0, arg.to_string());

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDCLSTN, 0, prd->ref, 0, dt, 0, ref, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventAddClstn");
};

void Sbecore::DbsMon::eventChangeClstnArg(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventChangeClstnArg");

	double dt = getDt();

	ubigint ref;

	Arg argFind;

	ubigint refWzemMClstn = 0;
	WzemMClstn* cln = NULL;

	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	argFind = arg;
	argFind.clearContent();

	auto it = refsClstn.find(xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), xjrefTrig, argFind, ixVSge));
	if (it != refsClstn.end()) refWzemMClstn = it->second;

	if (dbswzem->tblwzemmclstn->loadRecByRef(refWzemMClstn, &cln)) {
		dbswzem->executeQuery("UPDATE TblWzemJMClstn SET x1Stopu = " + to_string(dt) + " WHERE refWzemMClstn = " + to_string(refWzemMClstn) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmclstn->insertNewRec(NULL, refWzemMClstn, dt, 0.0, arg.to_string());

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::CHANGECLSTNARG, 0, prd->ref, 0, dt, 0, refWzemMClstn, cln->refWzemMJob, 0, 0, 0, "", "", "", "", "");

		if (dbswzem->tblwzemmevent->loadRecBySQL("SELECT * FROM TblWzemMEvent WHERE refWzemMPeriod = " + to_string(prd->ref) + " AND refWzemMClstn = " + to_string(cln->ref) + " ORDER BY startu ASC LIMIT 1", &evt)) {
			if (evt->refWzemCEvent == 0) {
				refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

				evt->refWzemCEvent = refWzemCEvent;

				dbswzem->tblwzemmevent->updateRec(evt);
			} else refWzemCEvent = evt->refWzemCEvent;

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));

			delete evt;
		};

		delete cln;
	};

	unlockAccess("DbsMon", "eventChangeClstnArg");
};

void Sbecore::DbsMon::eventRemoveClstn(
			const ubigint xjref
			, const string& srefIxVTarget
			, const string& srefIxVCall
			, const string& srefIxVJobmask
			, const ubigint xjrefTrig
			, const Arg& arg
			, const uint ixVSge
			, const string& srefIxVJactype
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRemoveClstn");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMClstn = 0;
	WzemMClstn* cln = NULL;

	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsClstn.find(xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), xjrefTrig, arg, ixVSge));
	if (it != refsClstn.end()) refWzemMClstn = it->second;

	if (dbswzem->tblwzemmclstn->loadRecByRef(refWzemMClstn, &cln)) {
		cln->x1Stopu = dt;

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::REMOVECLSTN, 0, prd->ref, 0, dt, 0, refWzemMClstn, cln->refWzemMJob, 0, 0, 0, "", "", "", "", "");

		if (dbswzem->tblwzemmevent->loadRecBySQL("SELECT * FROM TblWzemMEvent WHERE refWzemMPeriod = " + to_string(prd->ref) + " AND refWzemMClstn = " + to_string(cln->ref) + " ORDER BY startu ASC LIMIT 1", &evt)) {
			if (evt->refWzemCEvent == 0) {
				refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

				evt->refWzemCEvent = refWzemCEvent;

				dbswzem->tblwzemmevent->updateRec(evt);
			} else refWzemCEvent = evt->refWzemCEvent;

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));

			delete evt;
		};

		dbswzem->tblwzemmclstn->updateRec(cln);

		delete cln;
	};

	unlockAccess("DbsMon", "eventRemoveClstn");
};

void Sbecore::DbsMon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddPreset");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmpreset->insertNewRec(NULL, dt, 0.0, refWzemMJob, srefIxVPreset, arg.to_string());
		refsPreset[xpresetref_t(xjref, srefIxVPreset)] = ref;

		dbswzem->tblwzemjmpresetarg->insertNewRec(NULL, ref, dt, 0.0, arg.to_string());

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDPRESET, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, ref, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventAddPreset");
};


void Sbecore::DbsMon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const Arg& arg
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventChangePreset");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMPreset = 0;
	WzemMPreset* pst = NULL;

	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsPreset.find(xpresetref_t(xjref, srefIxVPreset));
	if (it != refsPreset.end()) refWzemMPreset = it->second;

	if (dbswzem->tblwzemmpreset->loadRecByRef(refWzemMPreset, &pst)) {
		dbswzem->executeQuery("UPDATE TblWzemJMPresetArg SET x1Stopu = " + to_string(dt) + " WHERE refWzemMPreset = " + to_string(refWzemMPreset) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmpresetarg->insertNewRec(NULL, refWzemMPreset, dt, 0.0, arg.to_string());

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::CHANGEPRESET, 0, prd->ref, 0, dt, 0, 0, pst->refWzemMJob, 0, 0, refWzemMPreset, "", "", "", "", "");

		if (dbswzem->tblwzemmevent->loadRecBySQL("SELECT * FROM TblWzemMEvent WHERE refWzemMPeriod = " + to_string(prd->ref) + " AND refWzemMPreset = " + to_string(pst->ref) + " ORDER BY startu ASC LIMIT 1", &evt)) {
			if (evt->refWzemCEvent == 0) {
				refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

				evt->refWzemCEvent = refWzemCEvent;

				dbswzem->tblwzemmevent->updateRec(evt);
			} else refWzemCEvent = evt->refWzemCEvent;

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));

			delete evt;
		};

		delete pst;
	};

	unlockAccess("DbsMon", "eventChangePreset");
};

void Sbecore::DbsMon::eventRemovePreset(
			const ubigint xjref
			, const string& srefIxVPreset
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRemovePreset");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMPreset = 0;
	WzemMPreset* pst = NULL;

	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsPreset.find(xpresetref_t(xjref, srefIxVPreset));
	if (it != refsPreset.end()) refWzemMPreset = it->second;

	if (dbswzem->tblwzemmpreset->loadRecByRef(refWzemMPreset, &pst)) {
		pst->x1Stopu = dt;

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::REMOVEPRESET, 0, prd->ref, 0, dt, 0, 0, pst->refWzemMJob, 0, 0, refWzemMPreset, "", "", "", "", "");

		if (dbswzem->tblwzemmevent->loadRecBySQL("SELECT * FROM TblWzemMEvent WHERE refWzemMPeriod = " + to_string(prd->ref) + " AND refWzemMPreset = " + to_string(pst->ref) + " ORDER BY startu ASC LIMIT 1", &evt)) {
			if (evt->refWzemCEvent == 0) {
				refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

				evt->refWzemCEvent = refWzemCEvent;

				dbswzem->tblwzemmevent->updateRec(evt);
			} else refWzemCEvent = evt->refWzemCEvent;

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));

			delete evt;
		};

		dbswzem->tblwzemmpreset->updateRec(pst);

		delete pst;
	};

	unlockAccess("DbsMon", "eventRemovePreset");
};

void Sbecore::DbsMon::eventAddNode(
			const ubigint xnref
			, const string& Ip
			, const usmallint Port
			, const utinyint Opprcn
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddNode");

	double dt = getDt();

	ubigint ref;

	ref = dbswzem->tblwzemmnode->insertNewRec(NULL, dt, 0.0, prd->ref, xnref, Ip, Port, Opprcn);
	refsNode[xnref] = ref;

	dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDNODE, 0, prd->ref, 0, dt, 0, 0, 0, ref, 0, 0, "", "", "", "", "");

	unlockAccess("DbsMon", "eventAddNode");
};

void Sbecore::DbsMon::eventRemoveNode(
			const ubigint xnref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRemoveNode");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMNode = 0;
	ubigint refWzemMEvent = 0;
	ubigint refWzemCEvent = 0;

	auto it = refsNode.find(xnref);
	if (it != refsNode.end()) refWzemMNode = it->second;

	if (refWzemMNode != 0) {
		dbswzem->executeQuery("UPDATE TblWzemMNode SET x1Stopu = " + to_string(dt) + " WHERE ref = " + to_string(refWzemMNode));

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::REMOVENODE, 0, prd->ref, 0, dt, 0, 0, 0, refWzemMNode, 0, 0, "", "", "", "", "");

		if (dbswzem->loadRefBySQL("SELECT ref FROM TblWzemMEvent WHERE ixVBasetype = " + to_string(VecWzemVMEventBasetype::ADDNODE) + " AND refWzemMPeriod = " + to_string(prd->ref)
					+ " AND refWzemMNode = " + to_string(refWzemMNode), refWzemMEvent)) {

			refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(refWzemMEvent));
			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));
		};
	};

	unlockAccess("DbsMon", "eventRemoveNode");
};

Sbecore::ubigint Sbecore::DbsMon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const Arg& argInv
		) {
	if (!prd) return 0;
	lockAccess("DbsMon", "eventTriggerCall");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	ubigint eref = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmcall->insertNewRec(NULL, dt, 0.0, refWzemMJob, srefIxVCall, argInv.to_string(), "");

		dbswzem->tblwzemjmcallargret->insertNewRec(NULL, ref, dt, 0.0, "");

		eref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::TRIGGERCALL, 0, prd->ref, 0, dt, ref, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
		refsEvent[eref] = eref;
	};

	unlockAccess("DbsMon", "eventTriggerCall");

	return eref;
};

void Sbecore::DbsMon::eventHandleCall(
			const ubigint eref
			, const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventHandleCall");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	ubigint refWzemMEvent = 0;
	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		it = refsEvent.find(eref);
		if (it != refsEvent.end()) refWzemMEvent = it->second;

		if (dbswzem->tblwzemmevent->loadRecByRef(refWzemMEvent, &evt)) {
			ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::HANDLECALL, 0, prd->ref, 0, dt, evt->refWzemMCall, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

			if (evt->refWzemCEvent == 0) {
				refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

				evt->refWzemCEvent = refWzemCEvent;

				dbswzem->tblwzemmevent->updateRec(evt);
			} else refWzemCEvent = evt->refWzemCEvent;

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));

			delete evt;
		};
	};

	unlockAccess("DbsMon", "eventHandleCall");
};

void Sbecore::DbsMon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const Arg& argRet
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRetCall");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	ubigint refWzemMEvent = 0;
	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		it = refsEvent.find(eref);
		if (it != refsEvent.end()) refWzemMEvent = it->second;

		if (dbswzem->tblwzemmevent->loadRecByRef(refWzemMEvent, &evt)) {
			dbswzem->executeQuery("UPDATE TblWzemJMCallArgRet SET x1Stopu = " + to_string(dt) + " WHERE refWzemMCall = " + to_string(evt->refWzemMCall) + " AND x1Stopu = 0.0");

			dbswzem->tblwzemjmcallargret->insertNewRec(NULL, evt->refWzemMCall, dt, 0.0, argRet.to_string());

			ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::RETCALL, 0, prd->ref, 0, dt, evt->refWzemMCall, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

			if (evt->refWzemCEvent == 0) {
				refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

				evt->refWzemCEvent = refWzemCEvent;

				dbswzem->tblwzemmevent->updateRec(evt);
			} else refWzemCEvent = evt->refWzemCEvent;

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));

			delete evt;
		};
	};

	unlockAccess("DbsMon", "eventRetCall");
};


void Sbecore::DbsMon::eventFinalizeCall(
			const ubigint eref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventFinalizeCall");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMEvent = 0;
	WzemMEvent* evt = NULL;

	WzemMCall* cal = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsEvent.find(eref);
	if (it != refsEvent.end()) refWzemMEvent = it->second;

	if (dbswzem->tblwzemmevent->loadRecByRef(refWzemMEvent, &evt)) {
		if (dbswzem->tblwzemmcall->loadRecByRef(evt->refWzemMCall, &cal)) {
			cal->x1Stopu = dt;

			ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::FINALIZECALL, 0, prd->ref, 0, dt, evt->refWzemMCall, 0, 0, 0, 0, 0, "", "", "", "", "");

			if (evt->refWzemCEvent == 0) {
				refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

				evt->refWzemCEvent = refWzemCEvent;

				dbswzem->tblwzemmevent->updateRec(evt);
			} else refWzemCEvent = evt->refWzemCEvent;

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));

			dbswzem->tblwzemmcall->updateRec(cal);

			delete cal;
		};

		delete evt;
	};

	unlockAccess("DbsMon", "eventFinalizeCall");
};

void Sbecore::DbsMon::eventHandleReqCmd(
			const ubigint xjref
			, const string& Cmd
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventHandleReqCmd");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::HANDLEREQCMD, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, Cmd, "", "", "", "");

	unlockAccess("DbsMon", "eventHandleReqCmd");
};

Sbecore::ubigint Sbecore::DbsMon::eventHandleReqDpchapp(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!prd) return 0;
	lockAccess("DbsMon", "eventHandleReqDpchapp");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	WzemMEvent* evt = NULL;
	ubigint eref = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::HANDLEREQDPCHAPP, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		eref = evt->ref;
		refsEvent[eref] = eref;

		ref = dbswzem->tblwzemmdpch->insertNewRec(NULL, evt->ref, srefIxVDpch, srefsMask, Content);
		evt->refWzemMDpch = ref;

		dbswzem->tblwzemmevent->updateRec(evt);

		delete evt;
	};

	unlockAccess("DbsMon", "eventHandleReqDpchapp");

	return eref;
};

void Sbecore::DbsMon::eventReplyReqDpchapp(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventReplyReqDpchapp");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint refWzemMEvent = 0;
	ubigint refWzemCEvent = 0;

	WzemMEvent* evt = NULL;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::REPLYREQDPCHAPP, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		ref = dbswzem->tblwzemmdpch->insertNewRec(NULL, evt->ref, srefIxVDpch, srefsMask, Content);
		evt->refWzemMDpch = ref;

		it = refsEvent.find(eref);
		if (it != refsEvent.end()) refWzemMEvent = it->second;

		if (refWzemMEvent != 0) {
			refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(refWzemMEvent));
			evt->refWzemCEvent = refWzemCEvent;
		};

		dbswzem->tblwzemmevent->updateRec(evt);

		delete evt;
	};

	unlockAccess("DbsMon", "eventReplyReqDpchapp");
};

void Sbecore::DbsMon::eventHandleReqUpload(
			const ubigint xjref
			, const string& Filename
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventHandleReqUpload");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::HANDLEREQUPLOAD, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", Filename, "", "", "");

	unlockAccess("DbsMon", "eventHandleReqUpload");
};

Sbecore::ubigint Sbecore::DbsMon::eventHandleReqDownload(
			const ubigint xjref
		) {
	if (!prd) return 0;
	lockAccess("DbsMon", "eventHandleReqDownload");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	ubigint eref = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::HANDLEREQDOWNLOAD, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		eref = ref;
		refsEvent[eref] = eref;
	};

	unlockAccess("DbsMon", "eventHandleReqDownload");

	return eref;
};

void Sbecore::DbsMon::eventReplyReqDownload(
			const ubigint eref
			, const ubigint xjref
			, const string Filename
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventReplyReqDownload");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint refWzemMEvent = 0;
	ubigint refWzemCEvent = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::REPLYREQDOWNLOAD, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", Filename, "", "", "");

		it = refsEvent.find(eref);
		if (it != refsEvent.end()) refWzemMEvent = it->second;

		if (refWzemMEvent != 0) {
			refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(refWzemMEvent));
			dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE ref = " + to_string(ref));
		};
	};

	unlockAccess("DbsMon", "eventReplyReqDownload");
};

void Sbecore::DbsMon::eventHandleReqDpchret(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventHandleReqDpchret");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMOp = 0;
	WzemMOp* opx = NULL;

	ubigint refWzemCEvent = 0;

	WzemMEvent* evt = NULL;

	auto it = refsOp.find(xoref);
	if (it != refsOp.end()) refWzemMOp = it->second;

	if (dbswzem->tblwzemmop->loadRecByRef(refWzemMOp, &opx)) {
		opx->x1Stopu = dt;

		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::HANDLEREQDPCHRET, 0, prd->ref, 0, dt, 0, 0, opx->refWzemMJob, 0, refWzemMOp, 0, "", "", "", "", "");

		ref = dbswzem->tblwzemmdpch->insertNewRec(NULL, evt->ref, srefIxVDpch, "all", Content);
		evt->refWzemMDpch = ref;

		refWzemCEvent = dbswzem->tblwzemcevent->getNewRef();

		dbswzem->executeQuery("UPDATE TblWzemMEvent SET refWzemCEvent = " + to_string(refWzemCEvent) + " WHERE refWzemMOp = " + to_string(refWzemMOp));
		evt->refWzemCEvent = refWzemCEvent;

		dbswzem->tblwzemmevent->updateRec(evt);

		delete evt;

		dbswzem->tblwzemmop->updateRec(opx);

		delete opx;
	};

	unlockAccess("DbsMon", "eventHandleReqDpchret");
};

void Sbecore::DbsMon::eventHandleReqMethod(
			const ubigint xjref
			, const string& srefIxVFeatgroup
			, const string& srefIxVMethod
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventHandleReqMethod");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::HANDLEREQMETHOD, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", srefIxVFeatgroup, srefIxVMethod, "");

	unlockAccess("DbsMon", "eventHandleReqMethod");
};

void Sbecore::DbsMon::eventHandleReqTimer(
			const ubigint xjref
			, const string& xsref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventHandleReqTimer");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::HANDLEREQTIMER, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", xsref);

	unlockAccess("DbsMon", "eventHandleReqTimer");
};

void Sbecore::DbsMon::eventSubmitDpch(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventSubmitDpch");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	WzemMEvent* evt = NULL;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::SUBMITDPCH, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		ref = dbswzem->tblwzemmdpch->insertNewRec(NULL, evt->ref, srefIxVDpch, srefsMask, Content);
		evt->refWzemMDpch = ref;

		dbswzem->tblwzemmevent->updateRec(evt);

		delete evt;
	};

	unlockAccess("DbsMon", "eventSubmitDpch");
};

void Sbecore::DbsMon::eventAddInv(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddInv");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	WzemMEvent* evt = NULL;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmop->insertNewRec(NULL, dt, 0.0, refWzemMJob, xoref);
		refsOp[xoref] = ref;

		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::ADDINV, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, ref, 0, "", "", "", "", "");

		ref = dbswzem->tblwzemmdpch->insertNewRec(NULL, evt->ref, srefIxVDpch, "all", Content);
		evt->refWzemMDpch = ref;

		dbswzem->tblwzemmevent->updateRec(evt);

		delete evt;
	};

	unlockAccess("DbsMon", "eventAddInv");
};
