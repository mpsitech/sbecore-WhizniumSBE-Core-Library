/**
  * \file Engtypes.h
  * engine data types (declarations)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 12 May 2020
  */

#ifndef SBECORE_ENGTYPES_H
#define SBECORE_ENGTYPES_H

#include <list>
#include <set>

#include <sbecore/Mttypes.h>

namespace Sbecore {
	constexpr int intvalInvalid = std::numeric_limits<int>::min();
	constexpr double dblvalInvalid = -std::numeric_limits<double>::infinity();

	/**
		* clstnref_t
		*/
	class clstnref_t {

	public:
		clstnref_t(const uint ixVCall = 0, const ubigint jref = 0, const uint ixVTarget = 0, const uint ixVJobmask = 0, const ubigint jrefTrig = 0, const Arg& arg = Arg(), const uint ixVSge = 0);

	public:
		uint ixVCall;
		ubigint jref;
		uint ixVTarget;
		uint ixVJobmask;
		ubigint jrefTrig;
		Arg arg;
		uint ixVSge;

	public:
		bool operator<(const clstnref_t& comp) const;
	};

	/**
		* clstnref2_t
		*/
	class clstnref2_t {

	public:
		clstnref2_t(const ubigint jref = 0, const uint ixVTarget = 0, const uint ixVCall = 0, const uint ixVJobmask = 0, const ubigint jrefTrig = 0, const Arg& arg = Arg(), const uint ixVSge = 0);
		clstnref2_t(const clstnref_t& cref);

	public:
		ubigint jref;
		uint ixVTarget;
		uint ixVCall;
		uint ixVJobmask;
		ubigint jrefTrig;
		Arg arg;
		uint ixVSge;

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
			static const uint SELF = 3;
			static const uint SPEC = 4;
			static const uint TREE = 5;

		public:
			static uint getIx(const std::string& sref);
			static std::string getSref(const uint ix);
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
			static uint getIx(const std::string& sref);
			static std::string getSref(const uint ix);
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
			static uint getIx(const std::string& sref);
			static std::string getSref(const uint ix);
		};

	public:
		Clstn(const clstnref_t& cref, const uint ixVJactype);

	public:
		clstnref_t cref;

		uint ixVJactype;
	};

	/**
		* Claim
		*/
	class Claim {

	public:
		Claim(const bool retractable = true, const bool run = true);

	public:
		// managed by server
		bool takenNotAvailable;
		bool fulfilled;

		// managed by client
		bool retractable;
		bool run; // most frequent extra parameter for claim
	};

	/**
		* Csjobinfo
		*/
	class Csjobinfo {

	public:
		Csjobinfo(const uint ixVJob);
		~Csjobinfo();

	public:
		uint ixVJob;

		ubigint jrefSrv;
		std::set<ubigint> jrefsCli;

		Mutex mClaims;
		std::map<ubigint,Claim*> claims;
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

			static unsigned int getIx(const std::string& sref);
			static std::string getSref(const unsigned int ix);
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

			static unsigned int getIx(const std::string& sref);
			static std::string getSref(const unsigned int ix);
		};

		/**
			* Node
			*/
		class Node {

		public:
			Node(const std::string& s, const std::vector<Token*>& tkns, unsigned int ixTkn0, unsigned int ixTkn1, std::string& err);
			~Node();

		public:
			unsigned int ixVNodetype;

			std::string key; // used for sref, fct
			int intval;
			double dblval;
			std::string txtval;

			std::vector<Node*> subs;

		public:
			void expand();
			bool has(unsigned int _ixVNodetype, const std::string& _key);
			bool logicfct();
			bool logictree();
			void fctToLeaves(const std::string& fctkey);
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
		std::string s;
		std::vector<Token*> tkns;

		Node* root;

		unsigned int ixVState;
		std::string err;

	public:
		void reset();

		bool tokenize(const std::string& expr);
		bool parse();
		bool has(unsigned int ixVNodetype, const std::string& key);
		void dump();
	};

	/**
		* featix_t
		*/
	class featix_t {

	public:
		featix_t(const uint ixVFeatgroup = 0, const std::string& srefIxVFeature = "");

	public:
		uint ixVFeatgroup;
		std::string srefIxVFeature;

	public:
		bool operator<(const featix_t& comp) const;
	};

	/**
		* Jobinfo
		*/
	class Jobinfo {

	public:
		Jobinfo(const ubigint jrefSup);

	public:
		ubigint jrefSup;
		std::set<ubigint> jrefsSub;

		uint ixVSge;
	};

	/**
		* lockref_t
		*/
	class lockref_t {

	public:
		lockref_t(const ubigint jref = 0, const uint ix = 0);

	public:
		ubigint jref;
		uint ix;

	public:
		bool operator==(const lockref_t& comp) const;
		bool operator<(const lockref_t& comp) const;
	};

	/**
		* Method
		*/
	class Method {

	public:
		Method(const uint ixVFeatgroup, const std::string& srefIxVMethod);

	public:
		uint ixVFeatgroup;
		std::string srefIxVMethod;

		std::vector<const void*> parsInv;
		std::vector<void*> parsRet;

		bool success;
	};

	/**
		* VecOpVOpres
		*/
	namespace VecOpVOpres {
		const uint PROGRESS = 1;
		const uint SUCCESS = 2;
		const uint FAILURE = 3;
		const uint COMMERR = 4;

		uint getIx(const std::string& sref);
		std::string getSref(const uint ix);
	};

	/**
		* Op
		*/
	class Op {

	public:
		Op(const ubigint oref, const uint ixVDpch, const std::string& squawk = "");

	public:
		ubigint oref;
		uint ixVDpch;
		std::string squawk;
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

	/**
		* Resultitem
		*/
	class Resultitem {

	public:
		Resultitem();
	};

	/**
		* Result
		*/
	class Result {

	public:
		Result();
		virtual ~Result();

	public:
		void clear();
		unsigned int size() const;

		void append(Resultitem* ri);

		// methods for writing to result (one writer / writing thread)
		virtual void queue(const uint ix);
		virtual bool dequeue(uint& ix);

		// methods for reading from result (multiple readers)
		bool lock(const ubigint jref, const uint ix); // only de-queued items can be locked

		void unlock(const ubigint jref, const uint ix);
		void unlockByJref(const ubigint jref);

	public:
		Mutex mAccess;

		std::vector<Resultitem*> nodes;
		std::list<uint> icsQueue;
		std::list<lockref_t> locks;

		Resultitem* operator[](const uint ix);
	};
};
#endif
