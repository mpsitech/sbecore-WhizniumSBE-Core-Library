/**
  * \file DbsMon.cpp
  * monitoring to database (implementation)
  * \author Alexander Wirthmüller
  * \date created: 24 Jan 2016
  * \date modified: 18 Feb 2016
  */

#include "DbsMon.h"

#include "DbsWzem.h"

/******************************************************************************
 class DbsMon
 ******************************************************************************/

DbsMon::DbsMon() : Mon() {
	dbswzem = new DbsWzem();

	prd = NULL;
};

DbsMon::~DbsMon() {
	if (prd) stop();

	delete dbswzem;
};

void DbsMon::start(
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

	} catch (SbeException e) {
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

void DbsMon::stop() {
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

void DbsMon::insertJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
			, const bool Master
			, const bool Slave
			, const bool Dcol
			, const bool Stmgr
		) {
	if (!prd) return;
	lockAccess("DbsMon", "insertJob");

	ubigint ref;

	ubigint supRefWzemMJob = 0;

	auto it = refsJob.find(supXjref);
	if (it != refsJob.end()) supRefWzemMJob = it->second;

	ref = dbswzem->tblwzemmjob->insertNewRec(NULL, 0.0, 0.0, prd->ref, supRefWzemMJob, srefIxVJob, xjref, Master, Slave, Dcol, Stmgr);
	refsJob[xjref] = ref;

	dbswzem->tblwzemjmjob->insertNewRec(NULL, ref, 0.0, 0.0, Master, Slave);
	dbswzem->tblwzemjmjobdcol->insertNewRec(NULL, ref, 0.0, 0.0, Dcol);
	dbswzem->tblwzemjmjobstmgr->insertNewRec(NULL, ref, 0.0, 0.0, Stmgr);

	unlockAccess("DbsMon", "insertJob");
};

void DbsMon::insertClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& srefIxVTarget
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
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
		it = refsJob.find(trgXjref);
		if (it != refsJob.end()) trgRefWzemMJob = it->second;

		ref = dbswzem->tblwzemmclstn->insertNewRec(NULL, 0.0, 0.0, refWzemMJob, srefIxVCall, VecWzemVMClstnTarget::getIx(srefIxVTarget), VecWzemVMClstnJobmask::getIx(srefIxVJobmask), trgRefWzemMJob, argMask, VecWzemVMClstnJactype::getIx(srefIxVJactype));
		refsClstn[xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), trgXjref)] = ref;

		dbswzem->tblwzemjmclstn->insertNewRec(NULL, ref, 0.0, 0.0, argMask, VecWzemVMClstnJactype::getIx(srefIxVJactype));
	};

	unlockAccess("DbsMon", "insertClstn");
};

void DbsMon::insertPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
	if (!prd) return;
	lockAccess("DbsMon", "insertPreset");

	ubigint ref;

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmpreset->insertNewRec(NULL, 0.0, 0.0, refWzemMJob, srefIxVPreset, arg);
		refsPreset[xpresetref_t(xjref, srefIxVPreset)] = ref;

		dbswzem->tblwzemjmpresetarg->insertNewRec(NULL, ref, 0.0, 0.0, arg);
	};

	unlockAccess("DbsMon", "insertPreset");
};

void DbsMon::insertNode(
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

void DbsMon::eventAddJob(
			const ubigint supXjref
			, const string& srefIxVJob
			, const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddJob");

	double dt = getDt();

	ubigint ref;

	ubigint supRefWzemMJob = 0;

	auto it = refsJob.find(supXjref);
	if (it != refsJob.end()) supRefWzemMJob = it->second;

	ref = dbswzem->tblwzemmjob->insertNewRec(NULL, dt, 0.0, prd->ref, supRefWzemMJob, srefIxVJob, xjref, false, false, false, false);
	refsJob[xjref] = ref;

	dbswzem->tblwzemjmjob->insertNewRec(NULL, ref, dt, 0.0, false, false);
	dbswzem->tblwzemjmjobdcol->insertNewRec(NULL, ref, dt, 0.0, false);
	dbswzem->tblwzemjmjobstmgr->insertNewRec(NULL, ref, dt, 0.0, false);

	dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDJOB, 0, prd->ref, 0, dt, 0, 0, ref, 0, 0, 0, "", "", "", "", "");

	unlockAccess("DbsMon", "eventAddJob");
};

void DbsMon::eventRemoveJob(
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

void DbsMon::eventAddDcol(
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

void DbsMon::eventRemoveDcol(
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

void DbsMon::eventAddStmgr(
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

void DbsMon::eventRemoveStmgr(
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

void DbsMon::eventAddClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& srefIxVTarget
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
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
		it = refsJob.find(trgXjref);
		if (it != refsJob.end()) tjbRefWzemMJob = it->second;

		ref = dbswzem->tblwzemmclstn->insertNewRec(NULL, dt, 0.0, refWzemMJob, srefIxVCall, VecWzemVMClstnTarget::getIx(srefIxVTarget), VecWzemVMClstnJobmask::getIx(srefIxVJobmask), tjbRefWzemMJob, argMask, VecWzemVMClstnJactype::getIx(srefIxVJactype));
		refsClstn[xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), trgXjref)] = ref;

		dbswzem->tblwzemjmclstn->insertNewRec(NULL, ref, dt, 0.0, argMask, VecWzemVMClstnJactype::getIx(srefIxVJactype));

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDCLSTN, 0, prd->ref, 0, dt, 0, ref, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventAddClstn");
};

void DbsMon::eventChangeClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& srefIxVTarget
			, const string& srefIxVJobmask
			, const ubigint trgXjref
			, const string& argMask
			, const string& srefIxVJactype
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventChangeClstn");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMClstn = 0;
	WzemMClstn* cln = NULL;

	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsClstn.find(xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), trgXjref));
	if (it != refsClstn.end()) refWzemMClstn = it->second;

	if (dbswzem->tblwzemmclstn->loadRecByRef(refWzemMClstn, &cln)) {
		dbswzem->executeQuery("UPDATE TblWzemJMClstn SET x1Stopu = " + to_string(dt) + " WHERE refWzemMClstn = " + to_string(refWzemMClstn) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmclstn->insertNewRec(NULL, refWzemMClstn, dt, 0.0, argMask, VecWzemVMClstnJactype::getIx(srefIxVJactype));

		ref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::CHANGECLSTN, 0, prd->ref, 0, dt, 0, refWzemMClstn, cln->refWzemMJob, 0, 0, 0, "", "", "", "", "");

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

	unlockAccess("DbsMon", "eventChangeClstn");
};

void DbsMon::eventRemoveClstn(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& srefIxVTarget
			, const string& srefIxVJobmask
			, const ubigint trgXjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventRemoveClstn");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMClstn = 0;
	WzemMClstn* cln = NULL;

	WzemMEvent* evt = NULL;

	ubigint refWzemCEvent = 0;

	auto it = refsClstn.find(xclstnref_t(xjref, srefIxVTarget, srefIxVCall, VecWzemVMClstnJobmask::getIx(srefIxVJobmask), trgXjref));
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

void DbsMon::eventAddPreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventAddPreset");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		ref = dbswzem->tblwzemmpreset->insertNewRec(NULL, dt, 0.0, refWzemMJob, srefIxVPreset, arg);
		refsPreset[xpresetref_t(xjref, srefIxVPreset)] = ref;

		dbswzem->tblwzemjmpresetarg->insertNewRec(NULL, ref, dt, 0.0, arg);

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::ADDPRESET, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, ref, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventAddPreset");
};


void DbsMon::eventChangePreset(
			const ubigint xjref
			, const string& srefIxVPreset
			, const string& arg
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

		dbswzem->tblwzemjmpresetarg->insertNewRec(NULL, refWzemMPreset, dt, 0.0, arg);

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

void DbsMon::eventRemovePreset(
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

void DbsMon::eventAddNode(
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

void DbsMon::eventRemoveNode(
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

ubigint DbsMon::eventTriggerCall(
			const ubigint xjref
			, const string& srefIxVCall
			, const string& argInv
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
		ref = dbswzem->tblwzemmcall->insertNewRec(NULL, dt, 0.0, refWzemMJob, srefIxVCall, argInv, "");

		dbswzem->tblwzemjmcallargret->insertNewRec(NULL, ref, dt, 0.0, "");

		eref = dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::TRIGGERCALL, 0, prd->ref, 0, dt, ref, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
		refsEvent[eref] = eref;
	};

	unlockAccess("DbsMon", "eventTriggerCall");

	return eref;
};

void DbsMon::eventHandleCall(
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

void DbsMon::eventRetCall(
			const ubigint eref
			, const ubigint xjref
			, const string& argRet
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

			dbswzem->tblwzemjmcallargret->insertNewRec(NULL, evt->refWzemMCall, dt, 0.0, argRet);

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


void DbsMon::eventFinalizeCall(
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

void DbsMon::eventHandleReqCmd(
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

ubigint DbsMon::eventHandleReqRegular(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!prd) return 0;
	lockAccess("DbsMon", "eventHandleReqRegular");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;

	WzemMEvent* evt = NULL;
	ubigint eref = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::HANDLEREQREGULAR, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

		eref = evt->ref;
		refsEvent[eref] = eref;

		ref = dbswzem->tblwzemmdpch->insertNewRec(NULL, evt->ref, srefIxVDpch, srefsMask, Content);
		evt->refWzemMDpch = ref;

		dbswzem->tblwzemmevent->updateRec(evt);

		delete evt;
	};

	unlockAccess("DbsMon", "eventHandleReqRegular");

	return eref;
};

void DbsMon::eventReplyReqRegular(
			const ubigint eref
			, const ubigint xjref
			, const string& srefIxVDpch
			, const string& srefsMask
			, const string& Content
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventReplyReqRegular");

	double dt = getDt();

	ubigint ref;

	ubigint refWzemMJob = 0;
	ubigint refWzemMEvent = 0;
	ubigint refWzemCEvent = 0;

	WzemMEvent* evt = NULL;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::REPLYREQREGULAR, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");

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

	unlockAccess("DbsMon", "eventReplyReqRegular");
};

void DbsMon::eventHandleReqUpload(
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

ubigint DbsMon::eventHandleReqDownload(
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

void DbsMon::eventReplyReqDownload(
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

void DbsMon::eventHandleReqRet(
			const ubigint xjref
			, const string& srefIxVDpch
			, const string& Content
			, const ubigint xoref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventHandleReqRet");

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

		dbswzem->tblwzemmevent->insertNewRec(&evt, VecWzemVMEventBasetype::HANDLEREQRET, 0, prd->ref, 0, dt, 0, 0, opx->refWzemMJob, 0, refWzemMOp, 0, "", "", "", "", "");

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

	unlockAccess("DbsMon", "eventHandleReqRet");
};

void DbsMon::eventHandleReqMethod(
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

void DbsMon::eventHandleReqTimer(
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

void DbsMon::eventSubmitDpch(
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

void DbsMon::eventAddInv(
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

void DbsMon::eventBecomeMaster(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventBecomeMaster");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJob SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjob->insertNewRec(NULL, refWzemMJob, dt, 0.0, true, false);

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::BECOMEMASTER, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventBecomeMaster");
};

void DbsMon::eventGiveupMaster(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventGiveupMaster");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJob SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjob->insertNewRec(NULL, refWzemMJob, dt, 0.0, false, false);

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::GIVEUPMASTER, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventGiveupMaster");
};

void DbsMon::eventBecomeSlave(
			const ubigint xjref
		) {
	if (!prd) return;
	lockAccess("DbsMon", "eventBecomeSlave");

	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJob SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjob->insertNewRec(NULL, refWzemMJob, dt, 0.0, false, true);

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::BECOMESLAVE, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventBecomeSlave");
};

void DbsMon::eventGiveupSlave(
			const ubigint xjref
		) {
	if (!prd) return;

	lockAccess("DbsMon", "eventGiveupSlave");
	double dt = getDt();

	ubigint refWzemMJob = 0;

	auto it = refsJob.find(xjref);
	if (it != refsJob.end()) refWzemMJob = it->second;

	if (refWzemMJob != 0) {
		dbswzem->executeQuery("UPDATE TblWzemJMJob SET x1Stopu = " + to_string(dt) + " WHERE refWzemMJob = " + to_string(refWzemMJob) + " AND x1Stopu = 0.0");

		dbswzem->tblwzemjmjob->insertNewRec(NULL, refWzemMJob, dt, 0.0, false, true);

		dbswzem->tblwzemmevent->insertNewRec(NULL, VecWzemVMEventBasetype::GIVEUPSLAVE, 0, prd->ref, 0, dt, 0, 0, refWzemMJob, 0, 0, 0, "", "", "", "", "");
	};

	unlockAccess("DbsMon", "eventGiveupSlave");
};
