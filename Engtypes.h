/**
  * \file Engtypes.h
  * engine data types (declarations)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 6 Oct 2015
  */

#ifndef SBECORE_ENGTYPES_H
#define SBECORE_ENGTYPES_H

#include <list>
#include <set>

#include <sbecore/Strmod.h>

/**
  * clstnref_t
  */
class clstnref_t {

public:
	clstnref_t(const uint ixVCall = 0, const ubigint jref = 0, const uint ixVTarget = 0, const uint ixVJobmask = 0, const ubigint jrefTrig = 0);

public:
	uint ixVCall;
	ubigint jref;
	uint ixVTarget;
	uint ixVJobmask;
	ubigint jrefTrig;

public:
	bool operator<(const clstnref_t& comp) const;
};

/**
  * clstnref2_t
  */
class clstnref2_t {

public:
	clstnref2_t(const ubigint jref = 0, const uint ixVTarget = 0, const uint ixVCall = 0, const uint ixVJobmask = 0, const ubigint jrefTrig = 0);
	clstnref2_t(const clstnref_t& cref);

public:
	ubigint jref;
	uint ixVTarget;
	uint ixVCall;
	uint ixVJobmask;
	ubigint jrefTrig;

public:
	bool operator<(const clstnref2_t& comp) const;
};

/**
	* Call
	*/
class Call {

public:
	Call(const uint ixVCall, const ubigint jref, const Arg& argInv);

public:
	uint ixVCall;
	ubigint jref;

	Arg argInv;
	Arg argRet;

	bool abort;
};

/**
	* Clstn
	*/
class Clstn {

public:
	/**
		* VecVJobmask
		*/
	class VecVJobmask {

	public:
		static const uint ALL = 1;
		static const uint IMM = 2;
		static const uint MAST = 3;
		static const uint SELF = 4;
		static const uint SLV = 5;
		static const uint SPEC = 6;
		static const uint TREE = 7;

	public:
		static uint getIx(const string& sref);
		static string getSref(const uint ix);
	};

	/**
		* VecVJactype
		*/
	class VecVJactype {

	public:
		static const uint LOCK = 1;
		static const uint TRY = 2;
		static const uint WEAK = 3;

	public:
		static uint getIx(const string& sref);
		static string getSref(const uint ix);
	};

	/**
		* VecVTarget
		*/
	class VecVTarget {

	public:
		static const uint JOB = 1;
		static const uint STMGR = 2;
		static const uint DDSPUB = 3;
		static const uint UASRV = 4;

	public:
		static uint getIx(const string& sref);
		static string getSref(const uint ix);
	};

public:
	Clstn(const clstnref_t& cref, const Arg& argMask, const uint ixVJactype = VecVJactype::LOCK);

public:
	clstnref_t cref;

	Arg argMask;
	uint ixVJactype;
};

/**
  * featix_t
  */
class featix_t {

public:
	featix_t(const uint ixVFeatgroup = 0, const string& srefIxVFeature = "");

public:
	uint ixVFeatgroup;
	string srefIxVFeature;

public:
	bool operator<(const featix_t& comp) const;
};

/**
  * Method
  */
class Method {

public:
	Method(const uint ixVFeatgroup, const string& srefIxVMethod);

public:
	uint ixVFeatgroup;
	string srefIxVMethod;

	vector<const void*> parsInv;
	vector<void*> parsRet;

	bool success;
};

/**
  * Msjobinfo
  */
class Msjobinfo {

public:
	Msjobinfo(const uint ixVJob);

public:
	uint ixVJob;

	ubigint jrefMast;
	list<ubigint> jrefsSlv;

	set<ubigint> jrefsSrd;

	uint ixVSge;
};

/**
  * VecOpVOpres
  */
namespace VecOpVOpres {
	const uint PROGRESS = 1;
	const uint SUCCESS = 2;
	const uint FAILURE = 3;
	const uint COMMERR = 4;

	uint getIx(const string& sref);
	string getSref(const uint ix);
};

/**
  * Op
  */
class Op {

public:
	Op(const ubigint oref, const uint ixVDpch, const string& squawk = "");

public:
	ubigint oref;
	uint ixVDpch;
	string squawk;
};

/**
  * presetref_t
  */
class presetref_t {

public:
	presetref_t(const ubigint jref = 0, const uint ixVPreset = 0);

public:
	ubigint jref;
	uint ixVPreset;

public:
	bool operator<(const presetref_t& comp) const;
};

/**
	* Preset
	*/
class Preset {

public:
	Preset(const presetref_t& pref, const Arg& arg);

public:
	presetref_t pref;

	Arg arg;
};
#endif
