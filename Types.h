/**
  * \file Types.h
  * basic data types and functional expression evaluation (declarations)
  * \author Alexander Wirthmüller
  * \date created: 10 Aug 2014
  * \date modified: 28 May 2018
  */

#ifndef SBECORE_TYPES_H
#define SBECORE_TYPES_H

typedef char tinyint;
typedef unsigned char utinyint;
typedef short int smallint;
typedef unsigned short int usmallint;
typedef unsigned int uint;
typedef long long int bigint;
typedef unsigned long long int ubigint;

#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include <sbecore/Mt.h>

/**
  * Floatmat
  */
class Floatmat {

public:
	Floatmat();
	
public:
	vector<float> vec;
	unsigned int M;
	unsigned int N;
};

/**
  * Doublemat
  */
class Doublemat {

public:
	Doublemat();
	
public:
	vector<double> vec;
	unsigned int M;
	unsigned int N;
};

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

constexpr int intvalInvalid = numeric_limits<int>::min();
constexpr double dblvalInvalid = -numeric_limits<double>::infinity();

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
	* Refseq
	*/
class Refseq {

public:
	Refseq(const string& sref = "");

public:
	string sref;

	Mutex mAccess;
	ubigint ref;

public:
	ubigint getNewRef();
};

/**
  * Scr
  */
namespace Scr {
	string scramble(const ubigint ref);
	ubigint descramble(const string& scrRef);
	string random();

	extern Rwmutex rwm;
	extern map<ubigint,string> scr;
	extern map<string,ubigint> descr;
};
#endif
