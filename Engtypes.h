/**
  * \file Engtypes.h
  * engine data types (declarations)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 6 Oct 2015
  */

#ifndef SBECORE_ENGTYPES_H
#define SBECORE_ENGTYPES_H

#include <set>

#include <sbecore/Strmod.h>

/**
  * cref_t
  */
class cref_t {

public:
	cref_t(const uint ixVCall = 0, const ubigint jref = 0, const bool stmgr = false, const uint ixVJobmask = 0, const ubigint jrefTrig = 0);

public:
	uint ixVCall;
	ubigint jref;
	bool stmgr;
	uint ixVJobmask;
	ubigint jrefTrig;

public:
	bool operator<(const cref_t& comp) const;
};

/**
  * cref2_t
  */
class cref2_t {

public:
	cref2_t(const ubigint jref = 0, const bool stmgr = false, const uint ixVCall = 0, const uint ixVJobmask = 0, const ubigint jrefTrig = 0);
	cref2_t(const cref_t& cref);

public:
	ubigint jref;
	bool stmgr;
	uint ixVCall;
	uint ixVJobmask;
	ubigint jrefTrig;

public:
	bool operator<(const cref2_t& comp) const;
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

public:
	Clstn(const cref_t& cref, const Arg& argMask, const uint ixVJactype = VecVJactype::LOCK);

public:
	cref_t cref;

	Arg argMask;
	uint ixVJactype;
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
  * pref_t
  */
class pref_t {

public:
	pref_t(const ubigint jref = 0, const uint ixVPreset = 0);

public:
	ubigint jref;
	uint ixVPreset;

public:
	bool operator<(const pref_t& comp) const;
};

/**
	* Preset
	*/
class Preset {

public:
	Preset(const pref_t& pref, const Arg& arg);

public:
	pref_t pref;

	Arg arg;
};
#endif
