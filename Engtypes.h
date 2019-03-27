/**
  * \file Engtypes.h
  * engine data types (declarations)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 29 Dec 2018
  */

#ifndef SBECORE_ENGTYPES_H
#define SBECORE_ENGTYPES_H

#include <list>
#include <set>

#include <sbecore/Mttypes.h>

constexpr int intvalInvalid = numeric_limits<int>::min();
constexpr double dblvalInvalid = -numeric_limits<double>::infinity();

/**
  * Arg
  */
class Arg {

public:
	static const ubigint IX = 1;
	static const ubigint REF = 2;
	static const ubigint REFS = 4;
	static const ubigint SREF = 8;
	static const ubigint INTVAL = 16;
	static const ubigint DBLVAL = 32;
	static const ubigint BOOLVAL = 64;
	static const ubigint TXTVAL = 128;
	static const ubigint ALL = 255;

public:
	Arg(const uint ix = 0, const ubigint ref = 0, const vector<ubigint>& refs = {}, const string& sref = "", const int intval = 0, const double dblval = 0.0, const bool boolval = false, const string& txtval = "", const ubigint mask = 0);

public:
	ubigint mask;

	uint ix;
	ubigint ref;
	vector<ubigint> refs;
	string sref;
	int intval;
	double dblval;
	bool boolval;
	string txtval;

public:
	bool operator==(const Arg& comp) const;
	bool operator!=(const Arg& comp) const;
	string writeText() const;
};

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
  * Expr
  */
class Expr {

public:
	/**
		* VecVTokentype
		*/
	class VecVTokentype {

	public:
		static const unsigned int LPAR = 1;
		static const unsigned int RPAR = 2;
		static const unsigned int COMMA = 3;
		static const unsigned int EXCL = 4;
		static const unsigned int AMP = 5;
		static const unsigned int VBAR = 6;
		static const unsigned int LESS = 7;
		static const unsigned int MORE = 8;
		static const unsigned int STR = 9;
		static const unsigned int INT = 10;
		static const unsigned int DBL = 11;
		static const unsigned int TEXT = 12;

		static unsigned int getIx(const string& sref);
		static string getSref(const unsigned int ix);
	};

	/**
		* Token
		*/
	class Token {

	public:
		Token(const unsigned int ixVTokentype, const unsigned int ptr, const unsigned int len);

	public:
		unsigned int ixVTokentype;

		unsigned int ptr;
		unsigned int len;

		unsigned int ixSibling;
	};

	/**
		* VecVNodetype
		*/
	class VecVNodetype {

	public:
		static const unsigned int VOID = 0;
		static const unsigned int SREF = 1;
		static const unsigned int INTVAL = 2;
		static const unsigned int DBLVAL = 3;
		static const unsigned int TXTVAL = 4;
		static const unsigned int FCT = 5;

		static unsigned int getIx(const string& sref);
		static string getSref(const unsigned int ix);
	};

	/**
		* Node
		*/
	class Node {

	public:
		Node(const string& s, const vector<Token*>& tkns, unsigned int ixTkn0, unsigned int ixTkn1, string& err);
		~Node();

	public:
		unsigned int ixVNodetype;

		string key; // used for sref, fct
		int intval;
		double dblval;
		string txtval;

		vector<Node*> subs;

	public:
		void expand();
		bool has(unsigned int _ixVNodetype, const string& _key);
		bool logicfct();
		bool logictree();
		void fctToLeaves(const string& fctkey);
		void dump(unsigned int il = 0);
	};

	/**
		* VecVState
		*/
	class VecVState {

	public:
		static const unsigned int RESET = 0;
		static const unsigned int TOKENIZED = 1;
		static const unsigned int TKNERR = 2;
		static const unsigned int PARSED = 3;
		static const unsigned int PRSERR = 4;
	};

public:
	Expr();
	~Expr();

public:
	string s;
	vector<Token*> tkns;

	Node* root;

	unsigned int ixVState;
	string err;

public:
	void reset();

	bool tokenize(const string& expr);
	bool parse();
	bool has(unsigned int ixVNodetype, const string& key);
	void dump();
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
