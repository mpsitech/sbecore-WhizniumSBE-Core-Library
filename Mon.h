/**
	* \file Mon.h
	* monitoring (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 24 Jan 2016
	*/

#ifndef SBECORE_MON_H
#define SBECORE_MON_H

#include <sbecore/Mttypes.h>

namespace Sbecore {
	/**
		* xclstnref_t
		*/
	class xclstnref_t {

	public:
		xclstnref_t(const ubigint xjref = 0, const std::string& srefIxVTarget = "", const std::string& srefIxVCall = "", const uint ixVJobmask = 0, const ubigint xjrefTrig = 0, const Arg& arg = Arg(), const uint ixVSge = 0);

	public:
		ubigint xjref;
		std::string srefIxVTarget;
		std::string srefIxVCall;
		uint ixVJobmask;
		ubigint xjrefTrig;
		Arg arg;
		uint ixVSge;

	public:
		bool operator<(const xclstnref_t& comp) const;
	};

	/**
		* xpresetref_t
		*/
	class xpresetref_t {

	public:
		xpresetref_t(const ubigint xjref = 0, const std::string& srefIxVPreset = "");

	public:
		ubigint xjref;
		std::string srefIxVPreset;

	public:
		bool operator<(const xpresetref_t& comp) const;
	};

	/**
		* Mon
		*/
	class Mon {

	public:
		Mon();
		virtual ~Mon();

	public:
		Mutex mAccess;

		double t0;

	public:
		void lockAccess(const std::string& srefObject, const std::string& srefMember);
		void unlockAccess(const std::string& srefObject, const std::string& srefMember);

		bool isRunning();
		std::string getSquawk(const std::string& srefLocale);

		double getDt();

		virtual void insertJob(const ubigint supXjref, const std::string& srefIxVJob, const ubigint xjref, const bool Clisrv, const bool srvNotCli, const bool Dcol, const bool Stmgr);
		virtual void insertClstn(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		virtual void insertPreset(const ubigint xjref, const std::string& srefIxVPreset, const Arg& arg);
		virtual void insertNode(const ubigint xnref, const std::string& Ip, const usmallint Port, const utinyint Opprcn);

		virtual void eventAddJob(const ubigint supXjref, const std::string& srefIxVJob, const ubigint xjref, const bool Clisrv, const bool srvNotCli);
		virtual void eventRemoveJob(const ubigint xjref);
		virtual void eventAddDcol(const ubigint xjref);
		virtual void eventRemoveDcol(const ubigint xjref);
		virtual void eventAddStmgr(const ubigint xjref);
		virtual void eventRemoveStmgr(const ubigint xjref);
		virtual void eventAddClstn(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		virtual void eventChangeClstnArg(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		virtual void eventRemoveClstn(const ubigint xjref, const std::string& srefIxVTarget, const std::string& srefIxVCall, const std::string& srefIxVJobmask, const ubigint xjrefTrig, const Arg& arg, const uint ixVSge, const std::string& srefIxVJactype);
		virtual void eventAddPreset(const ubigint xjref, const std::string& srefIxVPreset, const Arg& arg);
		virtual void eventChangePreset(const ubigint xjref, const std::string& srefIxVPreset, const Arg& arg);
		virtual void eventRemovePreset(const ubigint xjref, const std::string& srefIxVPreset);
		virtual void eventAddNode(const ubigint xnref, const std::string& Ip, const usmallint Port, const utinyint Opprcn);
		virtual void eventRemoveNode(const ubigint xnref);
		virtual ubigint eventTriggerCall(const ubigint xjref, const std::string& srefIxVCall, const Arg& argInv);
		virtual void eventHandleCall(const ubigint eref, const ubigint xjref);
		virtual void eventRetCall(const ubigint eref, const ubigint xjref, const Arg& argRet);
		virtual void eventFinalizeCall(const ubigint eref);
		virtual void eventHandleReqCmd(const ubigint xjref, const std::string& Cmd);
		virtual ubigint eventHandleReqDpchapp(const ubigint xjref, const std::string& srefIxVDpch, const std::string& srefsMask, const std::string& Content);
		virtual void eventReplyReqDpchapp(const ubigint eref, const ubigint xjref, const std::string& srefIxVDpch, const std::string& srefsMask, const std::string& Content);
		virtual void eventHandleReqUpload(const ubigint xjref, const std::string& Filename);
		virtual ubigint eventHandleReqDownload(const ubigint xjref);
		virtual void eventReplyReqDownload(const ubigint eref, const ubigint xjref, const std::string Filename);
		virtual void eventHandleReqDpchret(const ubigint xjref, const std::string& srefIxVDpch, const std::string& Content, const ubigint xoref);
		virtual void eventHandleReqMethod(const ubigint xjref, const std::string& srefIxVFeatgroup, const std::string& srefIxVMethod);
		virtual void eventHandleReqTimer(const ubigint xjref, const std::string& xsref);
		virtual void eventSubmitDpch(const ubigint xjref, const std::string& srefIxVDpch, const std::string& srefsMask, const std::string& Content);
		virtual void eventAddInv(const ubigint xjref, const std::string& srefIxVDpch, const std::string& Content, const ubigint xoref);
	};
};
#endif
