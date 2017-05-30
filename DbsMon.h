/**
  * \file DbsMon.h
  * monitoring to database (declarations)
  * \author Alexander Wirthmüller
  * \date created: 24 Jan 2016
  * \date modified: 26 Jan 2016
  */

#ifndef SBECORE_DBSMON_H
#define SBECORE_DBSMON_H

#include <sbecore/Mon.h>

class DbsWzem;
class WzemMPeriod;

/**
  * DbsMon
  */
class DbsMon : public Mon {

public:
	DbsMon();
	~DbsMon();

public:
	DbsWzem* dbswzem;

	WzemMPeriod* prd;

	map<xcref_t,ubigint> refsClstn; // {xjref,stmgr,srefIxVCall,ixVJobmask,xjrefTrig}->TblWzemMClstn.ref ; API: {xjref,stmgr,srefIxVCall,ixVJobmask,xjrefTrig}->scrRefWzemMClstn
	map<ubigint,ubigint> refsEvent; // eref->eref ; API: eref->scrRefWzemMEvent
	map<ubigint,ubigint> refsJob; // xjref->TblWzemMJob.ref ; API: xjref->scrRefWzemMJob
	map<ubigint,ubigint> refsNode; // xnref->TblWzemMNode.ref ; API: xnref->scrRefWzemMNode
	map<ubigint,ubigint> refsOp; // xoref->TblWzemMOp.ref ; API: xoref->scrRefWzemMOp
	map<xpref_t,ubigint> refsPreset; // {xjref,srefIxVPreset}->TblWzemMPreset.ref ; API: {xjref,srefIxVPreset}->scrRefWzemMPreset

public:
	void start(const string& Version, const uint ixDbsVDbstype, const string& dbspath, const string& dbsname, const string& ip, const uint port, const string& dbsusername, const string& dbspassword, const string& username, const string& password);
	void stop();

	void insertJob(const ubigint supXjref, const string& srefIxVJob, const ubigint xjref, const bool Master, const bool Slave, const bool Dcol, const bool Stmgr);
	void insertClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref, const string& argMask, const string& srefIxVJactype);
	void insertPreset(const ubigint xjref, const string& srefIxVPreset, const string& arg);
	void insertNode(const ubigint xnref, const string& Ip, const usmallint Port, const utinyint Opprcn);

	void eventAddJob(const ubigint supXjref, const string& srefIxVJob, const ubigint xjref);
	void eventRemoveJob(const ubigint xjref);
	void eventAddDcol(const ubigint xjref);
	void eventRemoveDcol(const ubigint xjref);
	void eventAddStmgr(const ubigint xjref);
	void eventRemoveStmgr(const ubigint xjref);
	void eventAddClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref, const string& argMask, const string& srefIxVJactype);
	void eventChangeClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref, const string& argMask, const string& srefIxVJactype);
	void eventRemoveClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref);
	void eventAddPreset(const ubigint xjref, const string& srefIxVPreset, const string& arg);
	void eventChangePreset(const ubigint xjref, const string& srefIxVPreset, const string& arg);
	void eventRemovePreset(const ubigint xjref, const string& srefIxVPreset);
	void eventAddNode(const ubigint xnref, const string& Ip, const usmallint Port, const utinyint Opprcn);
	void eventRemoveNode(const ubigint xnref);
	ubigint eventTriggerCall(const ubigint xjref, const string& srefIxVCall, const string& argInv);
	void eventHandleCall(const ubigint eref, const ubigint xjref);
	void eventRetCall(const ubigint eref, const ubigint xjref, const string& argRet);
	void eventFinalizeCall(const ubigint eref);
	void eventHandleReqCmd(const ubigint xjref, const string& Cmd);
	ubigint eventHandleReqRegular(const ubigint xjref, const string& srefIxVDpch, const string& srefsMask, const string& Content);
	void eventReplyReqRegular(const ubigint eref, const ubigint xjref, const string& srefIxVDpch, const string& srefsMask, const string& Content);
	void eventHandleReqUpload(const ubigint xjref, const string& Filename);
	ubigint eventHandleReqDownload(const ubigint xjref);
	void eventReplyReqDownload(const ubigint eref, const ubigint xjref, const string Filename);
	void eventHandleReqRet(const ubigint xjref, const string& srefIxVDpch, const string& Content, const ubigint xoref);
	void eventHandleReqTimer(const ubigint xjref, const string& xsref);
	void eventSubmitDpch(const ubigint xjref, const string& srefIxVDpch, const string& srefsMask, const string& Content);
	void eventAddInv(const ubigint xjref, const string& srefIxVDpch, const string& Content, const ubigint xoref);
	void eventBecomeMaster(const ubigint xjref);
	void eventGiveupMaster(const ubigint xjref);
	void eventBecomeSlave(const ubigint xjref);
	void eventGiveupSlave(const ubigint xjref);
};

#endif
