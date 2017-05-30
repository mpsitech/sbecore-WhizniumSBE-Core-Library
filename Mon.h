/**
  * \file Mon.h
  * monitoring (declarations)
  * \author Alexander Wirthmüller
  * \date created: 24 Jan 2016
  * \date modified: 26 Jan 2016
  */

#ifndef SBECORE_MON_H
#define SBECORE_MON_H

#include <sbecore/Types.h>

/**
  * xcref_t
  */
class xcref_t {

public:
	xcref_t(const ubigint xjref = 0, const bool stmgr = false, const string& srefIxVCall = "", const uint ixVJobmask = 0, const ubigint xjrefTrig = 0);

public:
	ubigint xjref;
	bool stmgr;
	string srefIxVCall;
	uint ixVJobmask;
	ubigint xjrefTrig;

public:
	bool operator<(const xcref_t& comp) const;
};

/**
  * xpref_t
  */
class xpref_t {

public:
	xpref_t(const ubigint xjref = 0, const string& srefIxVPreset = "");

public:
	ubigint xjref;
	string srefIxVPreset;

public:
	bool operator<(const xpref_t& comp) const;
};

/**
  * Mon
  */
class Mon {

public:
	Mon();
	virtual ~Mon();

public:
	pthread_mutex_t mAccess;

	double t0;

public:
	int lockAccess(const string& srefObject, const string& srefMember);
	int unlockAccess(const string& srefObject, const string& srefMember);

	double getDt();

	virtual void insertJob(const ubigint supXjref, const string& srefIxVJob, const ubigint xjref, const bool Master, const bool Slave, const bool Dcol, const bool Stmgr);
	virtual void insertClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref, const string& argMask, const string& srefIxVJactype);
	virtual void insertPreset(const ubigint xjref, const string& srefIxVPreset, const string& arg);
	virtual void insertNode(const ubigint xnref, const string& Ip, const usmallint Port, const utinyint Opprcn);

	virtual void eventAddJob(const ubigint supXjref, const string& srefIxVJob, const ubigint xjref);
	virtual void eventRemoveJob(const ubigint xjref);
	virtual void eventAddDcol(const ubigint xjref);
	virtual void eventRemoveDcol(const ubigint xjref);
	virtual void eventAddStmgr(const ubigint xjref);
	virtual void eventRemoveStmgr(const ubigint xjref);
	virtual void eventAddClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref, const string& argMask, const string& srefIxVJactype);
	virtual void eventChangeClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref, const string& argMask, const string& srefIxVJactype);
	virtual void eventRemoveClstn(const ubigint xjref, const string& srefIxVCall, const bool Stmgr, const string& srefIxVJobmask, const ubigint trgXjref);
	virtual void eventAddPreset(const ubigint xjref, const string& srefIxVPreset, const string& arg);
	virtual void eventChangePreset(const ubigint xjref, const string& srefIxVPreset, const string& arg);
	virtual void eventRemovePreset(const ubigint xjref, const string& srefIxVPreset);
	virtual void eventAddNode(const ubigint xnref, const string& Ip, const usmallint Port, const utinyint Opprcn);
	virtual void eventRemoveNode(const ubigint xnref);
	virtual ubigint eventTriggerCall(const ubigint xjref, const string& srefIxVCall, const string& argInv);
	virtual void eventHandleCall(const ubigint eref, const ubigint xjref);
	virtual void eventRetCall(const ubigint eref, const ubigint xjref, const string& argRet);
	virtual void eventFinalizeCall(const ubigint eref);
	virtual void eventHandleReqCmd(const ubigint xjref, const string& Cmd);
	virtual ubigint eventHandleReqRegular(const ubigint xjref, const string& srefIxVDpch, const string& srefsMask, const string& Content);
	virtual void eventReplyReqRegular(const ubigint eref, const ubigint xjref, const string& srefIxVDpch, const string& srefsMask, const string& Content);
	virtual void eventHandleReqUpload(const ubigint xjref, const string& Filename);
	virtual ubigint eventHandleReqDownload(const ubigint xjref);
	virtual void eventReplyReqDownload(const ubigint eref, const ubigint xjref, const string Filename);
	virtual void eventHandleReqRet(const ubigint xjref, const string& srefIxVDpch, const string& Content, const ubigint xoref);
	virtual void eventHandleReqTimer(const ubigint xjref, const string& xsref);
	virtual void eventSubmitDpch(const ubigint xjref, const string& srefIxVDpch, const string& srefsMask, const string& Content);
	virtual void eventAddInv(const ubigint xjref, const string& srefIxVDpch, const string& Content, const ubigint xoref);
	virtual void eventBecomeMaster(const ubigint xjref);
	virtual void eventGiveupMaster(const ubigint xjref);
	virtual void eventBecomeSlave(const ubigint xjref);
	virtual void eventGiveupSlave(const ubigint xjref);
};

#endif