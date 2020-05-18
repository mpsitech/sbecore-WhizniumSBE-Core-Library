/**
  * \file DbsMon.h
  * monitoring to database (declarations)
  * \author Alexander Wirthmüller
  * \date created: 24 Jan 2016
  * \date modified: 30 Apr 2020
  */

#ifndef SBECORE_DBSMON_H
#define SBECORE_DBSMON_H

#include <sbecore/Dbs.h>
#include <sbecore/Mon.h>

class DbsWzem;
class WzemMPeriod;

namespace Sbecore {
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

		std::map<xclstnref_t,ubigint> refsClstn; // {xjref,srefIxVTarget,srefIxVCall,ixVJobmask,xjrefTrig,arg,ixVSge} -> TblWzemMClstn.ref for DBS / scrRefWzemMClstn for API
		std::map<ubigint,ubigint> refsEvent; // eref->eref ; API: eref->scrRefWzemMEvent
		std::map<ubigint,ubigint> refsJob; // xjref->TblWzemMJob.ref ; API: xjref->scrRefWzemMJob
		std::map<ubigint,ubigint> refsNode; // xnref->TblWzemMNode.ref ; API: xnref->scrRefWzemMNode
		std::map<ubigint,ubigint> refsOp; // xoref->TblWzemMOp.ref ; API: xoref->scrRefWzemMOp
		std::map<xpresetref_t,ubigint> refsPreset; // {xjref,srefIxVPreset} -> TblWzemMPreset.ref for DBS / scrRefWzemMPreset for API

	public:
		void start(const std::string& Version, const uint ixDbsVDbstype, const std::string& dbspath, const std::string& dbsname, const std::string& ip, const uint port, const std::string& dbsusername, const std::string& dbspassword, const std::string& username, const std::string& password);
		void stop();

		void insertJob(const ubigint supXjref, const std::string& srefIxVJob, const ubigint xjref, const bool Clisrv, const bool srvNotCli, const bool Dcol, const bool Stmgr);
		void insertClstn(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		void insertPreset(const ubigint xjref, const std::string& srefIxVPreset, const Arg& arg);
		void insertNode(const ubigint xnref, const std::string& Ip, const usmallint Port, const utinyint Opprcn);

		void eventAddJob(const ubigint supXjref, const std::string& srefIxVJob, const ubigint xjref, const bool Clisrv, const bool srvNotCli);
		void eventRemoveJob(const ubigint xjref);
		void eventAddDcol(const ubigint xjref);
		void eventRemoveDcol(const ubigint xjref);
		void eventAddStmgr(const ubigint xjref);
		void eventRemoveStmgr(const ubigint xjref);
		void eventAddClstn(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		void eventChangeClstnArg(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		void eventRemoveClstn(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		void eventAddPreset(const ubigint xjref, const std::string& srefIxVPreset, const Arg& arg);
		void eventChangePreset(const ubigint xjref, const std::string& srefIxVPreset, const Arg& arg);
		void eventRemovePreset(const ubigint xjref, const std::string& srefIxVPreset);
		void eventAddNode(const ubigint xnref, const std::string& Ip, const usmallint Port, const utinyint Opprcn);
		void eventRemoveNode(const ubigint xnref);
		ubigint eventTriggerCall(const ubigint xjref, const std::string& srefIxVCall, const Arg& argInv);
		void eventHandleCall(const ubigint eref, const ubigint xjref);
		void eventRetCall(const ubigint eref, const ubigint xjref, const Arg& argRet);
		void eventFinalizeCall(const ubigint eref);
		void eventHandleReqCmd(const ubigint xjref, const std::string& Cmd);
		ubigint eventHandleReqDpchapp(const ubigint xjref, const std::string& srefIxVDpch, const std::string& srefsMask, const std::string& Content);
		void eventReplyReqDpchapp(const ubigint eref, const ubigint xjref, const std::string& srefIxVDpch, const std::string& srefsMask, const std::string& Content);
		void eventHandleReqUpload(const ubigint xjref, const std::string& Filename);
		ubigint eventHandleReqDownload(const ubigint xjref);
		void eventReplyReqDownload(const ubigint eref, const ubigint xjref, const std::string Filename);
		void eventHandleReqDpchret(const ubigint xjref, const std::string& srefIxVDpch, const std::string& Content, const ubigint xoref);
		void eventHandleReqMethod(const ubigint xjref, const std::string& srefIxVFeatgroup, const std::string& srefIxVMethod);
		void eventHandleReqTimer(const ubigint xjref, const std::string& xsref);
		void eventSubmitDpch(const ubigint xjref, const std::string& srefIxVDpch, const std::string& srefsMask, const std::string& Content);
		void eventAddInv(const ubigint xjref, const std::string& srefIxVDpch, const std::string& Content, const ubigint xoref);
	};
};

#endif
