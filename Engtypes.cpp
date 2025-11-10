/**
	* \file Engtypes.cpp
	* engine data types (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 6 Oct 2015
	*/

#include "Engtypes.h"

using namespace std;

/******************************************************************************
 class clstnref_t
 ******************************************************************************/

Sbecore::clstnref_t::clstnref_t(
			const uint ixVCall
			, const ubigint jref
			, const uint ixVTarget
			, const uint ixVJobmask
			, const ubigint jrefTrig
			, const Arg& arg
			, const uint ixVSge
		) {
	this->ixVCall = ixVCall;
	this->jref = jref;
	this->ixVTarget = ixVTarget;
	this->ixVJobmask = ixVJobmask;
	this->jrefTrig = jrefTrig;
	this->arg = arg;
	this->ixVSge = ixVSge;
};

bool Sbecore::clstnref_t::operator<(
			const clstnref_t& comp
		) const {
	// rigged < operator to allow multimap "any" filtering

	if (ixVCall < comp.ixVCall) return true;
	if (ixVCall != comp.ixVCall) return false;

	if ((jref == 0) || (comp.jref == 0)) return false;
	if (jref < comp.jref) return true;
	if (jref != comp.jref) return false;

	if ((ixVTarget == 0) || (comp.ixVTarget == 0)) return false;
	if (ixVTarget < comp.ixVTarget) return true;
	if (ixVTarget != comp.ixVTarget) return false;

	if ((ixVJobmask == 0) || (comp.ixVJobmask == 0)) return false;
	if (ixVJobmask < comp.ixVJobmask) return true;
	if (ixVJobmask != comp.ixVJobmask) return false;

	// jrefTrig makes sense to be non-zero for job mask type SPEC only
	if (ixVJobmask == Clstn::VecVJobmask::SPEC) {
		if ((jrefTrig == 0) || (comp.jrefTrig == 0)) return false;
		if (jrefTrig < comp.jrefTrig) return true;
		if (jrefTrig != comp.jrefTrig) return false;
	};

	// arg can be empty
	if ((arg.mask != 0) || (comp.arg.mask != 0)) {
		if ((arg.mask == 0) || (comp.arg.mask == 0)) return false;
		if (arg.mask < comp.arg.mask) return true;
		if (arg.mask != comp.arg.mask) return false;
		
		// compare individual components of arg; "any" filtering available for IX, REF and SREF only
		if (arg.mask & Arg::IX) {
			if ((arg.ix == 0) || (comp.arg.ix == 0)) return false;
			if (arg.ix < comp.arg.ix) return true;
			if (arg.ix != comp.arg.ix) return false;
		};

		if (arg.mask & Arg::REF) {
			if ((arg.ref == 0) || (comp.arg.ref == 0)) return false;
			if (arg.ref < comp.arg.ref) return true;
			if (arg.ref != comp.arg.ref) return false;
		};

		if (arg.mask & Arg::SREF) {
			if ((arg.sref == "") || (comp.arg.sref == "")) return false;
			if (arg.sref < comp.arg.sref) return true;
			if (arg.sref != comp.arg.sref) return false;
		};

		if (arg < comp.arg) return true;
		if (arg != comp.arg) return false;
	};

	// ixVSge can be empty
	//if ((ixVSge == 0) && (comp.ixVSge == 0)) return false;
	if ((ixVSge == 0) || (comp.ixVSge == 0)) return false;
	return(ixVSge < comp.ixVSge);
};

/******************************************************************************
 class clstnref2_t
 ******************************************************************************/

Sbecore::clstnref2_t::clstnref2_t(
			const ubigint jref
			, const uint ixVTarget
			, const uint ixVCall
			, const uint ixVJobmask
			, const ubigint jrefTrig
			, const Arg& arg
			, const uint ixVSge
		) {
	this->jref = jref;
	this->ixVTarget = ixVTarget;
	this->ixVCall = ixVCall;
	this->ixVJobmask = ixVJobmask;
	this->jrefTrig = jrefTrig;
	this->arg = arg;
	this->ixVSge = ixVSge;
};

Sbecore::clstnref2_t::clstnref2_t(
			const clstnref_t& cref
		) : clstnref2_t(cref.jref, cref.ixVTarget, cref.ixVCall, cref.ixVJobmask, cref.jrefTrig, cref.arg, cref.ixVSge) {
};

bool Sbecore::clstnref2_t::operator<(
			const clstnref2_t& comp
		) const {
	// rigged < operator to allow multimap "any" filtering

	if (jref < comp.jref) return true;
	if (jref != comp.jref) return false;

	if ((ixVTarget == 0) || (comp.ixVTarget == 0)) return false;
	if (ixVTarget < comp.ixVTarget) return true;
	if (ixVTarget != comp.ixVTarget) return false;

	if ((ixVCall == 0) || (comp.ixVCall == 0)) return false;
	if (ixVCall < comp.ixVCall) return true;
	if (ixVCall != comp.ixVCall) return false;

	if ((ixVJobmask == 0) || (comp.ixVJobmask == 0)) return false;
	if (ixVJobmask < comp.ixVJobmask) return true;
	if (ixVJobmask != comp.ixVJobmask) return false;

	if ((jrefTrig == 0) || (comp.jrefTrig == 0)) return false;
	if (jrefTrig < comp.jrefTrig) return true;
	if (jrefTrig != comp.jrefTrig) return false;

	if (arg < comp.arg) return true;
	if (arg != comp.arg) return false;

	return(ixVSge < comp.ixVSge);
};

/******************************************************************************
 class Call
 ******************************************************************************/

Sbecore::Call::Call(
			const uint ixVCall
			, const ubigint jref
			, const Arg& argInv
		) {
	this->ixVCall = ixVCall;
	this->jref = jref;

	this->argInv = argInv;

	abort = false;
};

/******************************************************************************
 class Clstn::VecVJobmask
 ******************************************************************************/

uint Sbecore::Clstn::VecVJobmask::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("all") == 0) return ALL;
	else if (s.compare("imm") == 0) return IMM;
	else if (s.compare("self") == 0) return SELF;
	else if (s.compare("spec") == 0) return SPEC;
	else if (s.compare("tree") == 0) return TREE;

	return 0;
};

string Sbecore::Clstn::VecVJobmask::getSref(
			const uint ix
		) {
	if (ix == ALL) return("all");
	else if (ix == IMM) return("imm");
	else if (ix == SELF) return("self");
	else if (ix == SPEC) return("spec");
	else if (ix == TREE) return("tree");

	return("");
};

/******************************************************************************
 class Clstn::VecVJactype
 ******************************************************************************/

uint Sbecore::Clstn::VecVJactype::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("lock") == 0) return LOCK;
	else if (s.compare("try") == 0) return TRY;
	else if (s.compare("weak") == 0) return WEAK;

	return 0;
};

string Sbecore::Clstn::VecVJactype::getSref(
			const uint ix
		) {
	if (ix == LOCK) return("lock");
	else if (ix == TRY) return("try");
	else if (ix == WEAK) return("weak");

	return("");
};

/******************************************************************************
 class Clstn::VecVTarget
 ******************************************************************************/

uint Sbecore::Clstn::VecVTarget::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("job") == 0) return JOB;
	else if (s.compare("stmgr") == 0) return STMGR;
	else if (s.compare("ddspub") == 0) return DDSPUB;
	else if (s.compare("uasrv") == 0) return UASRV;

	return 0;
};

string Sbecore::Clstn::VecVTarget::getSref(
			const uint ix
		) {
	if (ix == JOB) return("job");
	else if (ix == STMGR) return("stmgr");
	else if (ix == DDSPUB) return("ddspub");
	else if (ix == UASRV) return("uasrv");

	return("");
};

/******************************************************************************
 class Clstn
 ******************************************************************************/

Sbecore::Clstn::Clstn(
			const clstnref_t& cref
			, const uint ixVJactype
		) {
	this->cref = cref;
	this->ixVJactype = ixVJactype;
};

/******************************************************************************
 class Claim
 ******************************************************************************/

Sbecore::Claim::Claim(
			const bool retractable
			, const bool run
		) {
	takenNotAvailable = false;
	fulfilled = false;

	this->retractable = retractable;
	this->run = run;
};

/******************************************************************************
 class Csjobinfo
 ******************************************************************************/

Sbecore::Csjobinfo::Csjobinfo(
			const uint ixVJob
		) :
			mClaims("mClaims", "Csjobinfo", "Csjobinfo")
		{
	this->ixVJob = ixVJob;

	jrefSrv = 0;
};

Sbecore::Csjobinfo::~Csjobinfo() {
	for (auto it = claims.begin(); it != claims.end(); it++) delete(it->second);
};

/******************************************************************************
 class Expr::VecVTokentype
 ******************************************************************************/

unsigned int Sbecore::Expr::VecVTokentype::getIx(
			const string& sref
		) {
	if (sref.compare("lpar") == 0) return LPAR;
	else if (sref.compare("rpar") == 0) return RPAR;
	else if (sref.compare("comma") == 0) return COMMA;
	else if (sref.compare("excl") == 0) return EXCL;
	else if (sref.compare("amp") == 0) return AMP;
	else if (sref.compare("vbar") == 0) return VBAR;
	else if (sref.compare("less") == 0) return LESS;
	else if (sref.compare("more") == 0) return MORE;
	else if (sref.compare("str") == 0) return STR;
	else if (sref.compare("int") == 0) return INT;
	else if (sref.compare("dbl") == 0) return DBL;
	else if (sref.compare("text") == 0) return TEXT;

	return(0);
};

string Sbecore::Expr::VecVTokentype::getSref(
			const unsigned int ix
		) {
	if (ix == LPAR) return("lpar");
	else if (ix == RPAR) return("rpar");
	else if (ix == COMMA) return("comma");
	else if (ix == EXCL) return("excl");
	else if (ix == AMP) return("amp");
	else if (ix == VBAR) return("vbar");
	else if (ix == LESS) return("less");
	else if (ix == MORE) return("more");
	else if (ix == STR) return("str");
	else if (ix == INT) return("int");
	else if (ix == DBL) return("dbl");
	else if (ix == TEXT) return("text");

	return("");
};

/******************************************************************************
 class Expr::Token
 ******************************************************************************/

Sbecore::Expr::Token::Token(
			const unsigned int ixVTokentype
			, const unsigned int ptr
			, const unsigned int len
		) {
	this->ixVTokentype = ixVTokentype;
	this->ptr = ptr;
	this->len = len;

	ixSibling = (0-1);
};

/******************************************************************************
 class Expr::VecVNodetype
 ******************************************************************************/

unsigned int Sbecore::Expr::VecVNodetype::getIx(
			const string& sref
		) {
	if (sref.compare("void") == 0) return VOID;
	else if (sref.compare("sref") == 0) return SREF;
	else if (sref.compare("intval") == 0) return INTVAL;
	else if (sref.compare("dblval") == 0) return DBLVAL;
	else if (sref.compare("txtval") == 0) return TXTVAL;
	else if (sref.compare("fct") == 0) return FCT;

	return(0);
};

string Sbecore::Expr::VecVNodetype::getSref(
			const unsigned int ix
		) {
	if (ix == VOID) return("void");
	else if (ix == SREF) return("sref");
	else if (ix == INTVAL) return("intval");
	else if (ix == DBLVAL) return("dblval");
	else if (ix == TXTVAL) return("txtval");
	else if (ix == FCT) return("fct");

	return("");
};

/******************************************************************************
 class Expr::Node
 ******************************************************************************/

Sbecore::Expr::Node::Node(
			const string& s
			, const vector<Token*>& tkns
			, unsigned int ixTkn0
			, unsigned int ixTkn1
			, string& err
		) {
	Token* tkn = NULL;

//	cout << "(" << ixTkn0 << "," << ixTkn1 << ")" << endl;

	unsigned int ixTkn;

	vector<unsigned int> icsTknstrength;
	icsTknstrength.resize(4);

	vector<unsigned int> icsTkncomma;

	ixVNodetype = VecVNodetype::VOID;

	intval = 0;
	dblval = 0.0;

	if (ixTkn0 == ixTkn1) {
		// leaf node
		tkn = tkns[ixTkn0];

		if (tkn->ixVTokentype == VecVTokentype::STR) {
			ixVNodetype = VecVNodetype::SREF;
			key = s.substr(tkn->ptr, tkn->len);
		} else if (tkn->ixVTokentype == VecVTokentype::INT) {
			ixVNodetype = VecVNodetype::INTVAL;
			intval = atoi(s.substr(tkn->ptr, tkn->len).c_str());
		} else if (tkn->ixVTokentype == VecVTokentype::DBL) {
			ixVNodetype = VecVNodetype::DBLVAL;
			dblval = atof(s.substr(tkn->ptr, tkn->len).c_str());
		} else if (tkn->ixVTokentype == VecVTokentype::TEXT) {
			ixVNodetype = VecVNodetype::TXTVAL;
			txtval = s.substr(tkn->ptr, tkn->len);
		} else {
			//ixVNodetype = VecVNodetype::VOID;
			err = "invalid token " + to_string(ixTkn0) + " at position " + to_string(tkn->ptr);
		};

	} else {
		// - expression to be processed further (at least one operator/function)

		tkn = tkns[ixTkn0];
		if (tkn->ixSibling == ixTkn1) {
			// evaluate interior of (.)
			ixTkn0++;
			ixTkn1--;
		};

		ixTkn = ixTkn0;

		// - find operator token with the lowest strength
		for (unsigned int i = 0; i<icsTknstrength.size(); i++) icsTknstrength[i] = ixTkn1+1;
		while (ixTkn < ixTkn1) {
			tkn = tkns[ixTkn];

			if ((tkn->ixVTokentype == VecVTokentype::VBAR) || (tkn->ixVTokentype == VecVTokentype::LESS) || (tkn->ixVTokentype == VecVTokentype::MORE)) {
				// level 0 (weakest): |, >, <
				if (icsTknstrength[0] == (ixTkn1+1)) icsTknstrength[0] = ixTkn;

			} else if (tkn->ixVTokentype == VecVTokentype::AMP) {
				// level 1: &
				if (icsTknstrength[1] == (ixTkn1+1)) icsTknstrength[1] = ixTkn;

			} else if (tkn->ixVTokentype == VecVTokentype::EXCL) {
				// level 2: !
				if (icsTknstrength[2] == (ixTkn1+1)) icsTknstrength[2] = ixTkn;

			} else if (tkn->ixVTokentype == VecVTokentype::LPAR) {
				// level 3: fct(), ()
				if (icsTknstrength[3] == (ixTkn1+1)) {
					icsTknstrength[3] = ixTkn;
					if (ixTkn != ixTkn0) if (tkns[ixTkn-1]->ixVTokentype == VecVTokentype::STR) icsTknstrength[3] = ixTkn-1;
				};
				ixTkn = tkn->ixSibling;
			};

			ixTkn++;
		};

		ixTkn = ixTkn1+1;
		for (unsigned int i = 0; i<icsTknstrength.size(); i++) {
			if (icsTknstrength[i] != (ixTkn1+1)) {
				ixTkn = icsTknstrength[i];
				break;
			};
		};

		if (ixTkn != (ixTkn1+1)) {
			// - split and follow up recursively depending on token type
			tkn = tkns[ixTkn];

			if (tkn->ixVTokentype == VecVTokentype::EXCL) {
				ixVNodetype = VecVNodetype::FCT;
				key = "not";
				subs.push_back(new Node(s, tkns, ixTkn+1, ixTkn1, err)); // prefix

			} else if (tkn->ixVTokentype == VecVTokentype::AMP) {
				ixVNodetype = VecVNodetype::FCT;
				key = "and";
				subs.push_back(new Node(s, tkns, ixTkn0, ixTkn-1, err)); // infix
				subs.push_back(new Node(s, tkns, ixTkn+1, ixTkn1, err));

			} else if (tkn->ixVTokentype == VecVTokentype::VBAR) {
				ixVNodetype = VecVNodetype::FCT;
				key = "or";
				subs.push_back(new Node(s, tkns, ixTkn0, ixTkn-1, err)); // infix
				subs.push_back(new Node(s, tkns, ixTkn+1, ixTkn1, err));

			} else if (tkn->ixVTokentype == VecVTokentype::LESS) {
				ixVNodetype = VecVNodetype::FCT;
				key = "less";
				subs.push_back(new Node(s, tkns, ixTkn0, ixTkn-1, err)); // infix
				subs.push_back(new Node(s, tkns, ixTkn+1, ixTkn1, err));

			} else if (tkn->ixVTokentype == VecVTokentype::MORE) {
				ixVNodetype = VecVNodetype::FCT;
				key = "more";
				subs.push_back(new Node(s, tkns, ixTkn0, ixTkn-1, err)); // infix
				subs.push_back(new Node(s, tkns, ixTkn+1, ixTkn1, err));

			} else if (tkn->ixVTokentype == VecVTokentype::STR) {
				ixVNodetype = VecVNodetype::FCT;
				key = s.substr(tkn->ptr, tkn->len);

				ixTkn0 = ixTkn+1;
				ixTkn1 = tkns[ixTkn0]->ixSibling;

				// function arguments are comma separated
				for (ixTkn=ixTkn0+1;ixTkn<ixTkn1;ixTkn++) {
					tkn = tkns[ixTkn];

					if (tkn->ixSibling < tkns.size()) {
						ixTkn = tkn->ixSibling;
					} else {
						if (tkn->ixVTokentype == VecVTokentype::COMMA) icsTkncomma.push_back(ixTkn);
					};
				};

				ixTkn = ixTkn0+1;

				for (unsigned int i = 0; i<icsTkncomma.size(); i++) {
					subs.push_back(new Node(s, tkns, ixTkn, icsTkncomma[i]-1, err));
					ixTkn = icsTkncomma[i]+1;
				};

				if (ixTkn != ixTkn1) subs.push_back(new Node(s, tkns, ixTkn, ixTkn1-1, err));

			} else {
				ixVNodetype = VecVNodetype::VOID;
				err = "invalid token " + to_string(ixTkn) + " at position " + to_string(tkn->ptr);
			};

			for (unsigned int i = 0; i<subs.size(); i++) {
				if (subs[i]->ixVNodetype == VecVNodetype::VOID) {
					ixVNodetype = VecVNodetype::VOID;
					break;
				};
			};
		};
	};
};

Sbecore::Expr::Node::~Node() {
	for (unsigned int i = 0; i<subs.size(); i++) delete subs[i];
	subs.resize(0);
};

void Sbecore::Expr::Node::expand() {
	bool valid;

	Node* node = NULL;

	// check node for eligibility
	valid = ((ixVNodetype == VecVNodetype::FCT) && (subs.size() == 1));
	if (valid) valid = !logicfct();

	// check sub-tree for eligibility
	if (valid) valid = subs[0]->logictree();

	if (valid) {
		// - shift function to leaves
		node = subs[0];

		node->fctToLeaves(key);

		// replace node content by that of the only sub-node
		*this = *node;

		node->subs.resize(0);
		delete node;

	} else {
		for (unsigned int i = 0; i<subs.size(); i++) subs[i]->expand();
	};
};

bool Sbecore::Expr::Node::has(
			unsigned int _ixVNodetype
			, const string& _key
		) {
	bool retval = (_ixVNodetype == ixVNodetype);
	if (retval && (_key.length() > 0)) retval = (key.compare(_key) == 0);

	if (!retval) {
		for (unsigned int i = 0; i<subs.size(); i++) {
			retval = subs[i]->has(_ixVNodetype, _key);
			if (retval) break;
		};
	};

	return retval;
};

bool Sbecore::Expr::Node::logicfct() {
	bool retval = (ixVNodetype == VecVNodetype::FCT);

	if (retval) {
		retval = (key.compare("not") == 0);
		if (!retval) retval = (key.compare("and") == 0);
		if (!retval) retval = (key.compare("or") == 0);
		if (!retval) retval = (key.compare("less") == 0);
		if (!retval) retval = (key.compare("more") == 0);
	};

	return retval;
};

bool Sbecore::Expr::Node::logictree() {
	bool retval = false;

	if (subs.size() == 0) {
		retval = true;

	} else {
		retval = logicfct();

		if (retval) {
			for (unsigned int i = 0; i<subs.size(); i++) {
				retval = subs[i]->logictree();
				if (!retval) break;
			};
		};
	};

	return retval;
};

void Sbecore::Expr::Node::fctToLeaves(
			const string& fctkey
		) {
	if (subs.size() == 0) {
		subs.push_back(new Node(*this));

		ixVNodetype = VecVNodetype::FCT;
		key = fctkey;
		intval = 0;
		dblval = 0.0;
		txtval = "";

	} else {
		for (unsigned int i = 0; i<subs.size(); i++) subs[i]->fctToLeaves(fctkey);
	};
};

void Sbecore::Expr::Node::dump(
			unsigned int il
		) {
	for (unsigned int i = 0; i<il; i++) cout << "\t";

	if (subs.size() == 0) cout << "- "; else cout << "+ ";

	cout << "(" << VecVNodetype::getSref(ixVNodetype) << ")";

	if (ixVNodetype == VecVNodetype::SREF) {
		cout << " " << key;
	} else if (ixVNodetype == VecVNodetype::INTVAL) {
		cout << " " << intval;
	} else if (ixVNodetype == VecVNodetype::DBLVAL) {
		cout << " " << dblval;
	} else if (ixVNodetype == VecVNodetype::TXTVAL) {
		cout << " " << txtval;
	} else if (ixVNodetype == VecVNodetype::FCT) {
		cout << " " << key;
	};

	cout << endl;

	for (unsigned int i = 0; i<subs.size(); i++) subs[i]->dump(il+1);
};

/******************************************************************************
 class Expr
 ******************************************************************************/

Sbecore::Expr::Expr() {
	root = NULL;

	ixVState = VecVState::RESET;
};

Sbecore::Expr::~Expr() {
	reset();
};

void Sbecore::Expr::reset() {
	for (unsigned int i = 0; i<tkns.size(); i++) delete tkns[i];
	tkns.resize(0);

	if (root) {
		delete root;
		root = NULL;
	};

	ixVState = VecVState::RESET;
	err = "";
};

bool Sbecore::Expr::tokenize(
			const string& expr
		) {
	bool retval;

	unsigned int ptr;

	bool skipstr;
	bool skipintdbl;
	bool skiptext;

	Token* tkn = NULL;

	vector<Token*> sbls;

	reset();

//	cout << "tokenizing '" << expr << "'" << endl;

	// --- initialize
	s = expr;

	// --- tokenize
	ptr = 0;
	
	skipstr = false;
	skipintdbl = false;
	skiptext = false;

	while (ptr < s.size()) {
		if (skipstr) {
			if ((s[ptr] == '.') || ((s[ptr] >= 'A') && (s[ptr] <= 'Z')) || ((s[ptr] >= 'a') && (s[ptr] <= 'z')) || ((s[ptr] >= '0') && (s[ptr] <= '9'))) {
			} else {
				tkn->len = ptr-tkn->ptr;
				tkns.push_back(tkn);
				skipstr = false;
				ptr--;
			};
		} else if (skipintdbl) {
			if (s[ptr] == '.') {
				tkn->ixVTokentype = VecVTokentype::DBL;
			} else if ((s[ptr] >= '0') && (s[ptr] <= '9')) {
			} else {
				tkn->len = ptr-tkn->ptr;
				tkns.push_back(tkn);
				skipintdbl = false;
				ptr--;
			};
		} else if (skiptext) {
			if (s[ptr] == '"') {
				tkn->len = ptr-tkn->ptr;
				tkns.push_back(tkn);
				skiptext = false;
			};
		} else {
			if (s[ptr] == '(') {
				tkns.push_back(new Token(VecVTokentype::LPAR, ptr, 1));
			} else if (s[ptr] == ')') {
				tkns.push_back(new Token(VecVTokentype::RPAR, ptr, 1));
			} else if (s[ptr] == ',') {
				tkns.push_back(new Token(VecVTokentype::COMMA, ptr, 1));
			} else if (s[ptr] == '!') {
				tkns.push_back(new Token(VecVTokentype::EXCL, ptr, 1));
			} else if (s[ptr] == '&') {
				tkns.push_back(new Token(VecVTokentype::AMP, ptr, 1));
			} else if (s[ptr] == '|') {
				tkns.push_back(new Token(VecVTokentype::VBAR, ptr, 1));
			} else if (s[ptr] == '<') {
				tkns.push_back(new Token(VecVTokentype::LESS, ptr, 1));
			} else if (s[ptr] == '>') {
				tkns.push_back(new Token(VecVTokentype::MORE, ptr, 1));
			} else if (((s[ptr] >= 'A') && (s[ptr] <= 'Z')) || ((s[ptr] >= 'a') && (s[ptr] <= 'z'))) {
				tkn = new Token(VecVTokentype::STR, ptr, 0);
				skipstr = true;
			} else if ((s[ptr] == '-') || (s[ptr] == '+') || ((s[ptr] >= '0') && (s[ptr] <= '9'))) {
				tkn = new Token(VecVTokentype::INT, ptr, 0);
				skipintdbl = true;
			} else if (s[ptr] == '"') {
				tkn = new Token(VecVTokentype::TEXT, ptr+1, 0);
				skiptext = true;
			};
		};

		ptr++;
	};

	if (skipstr || skipintdbl || skiptext) {
		tkn->len = ptr-tkn->ptr;
		tkns.push_back(tkn);
	};

	retval = (tkns.size() > 0);
	if (!retval) err = "no tokens recognized";

//	for (unsigned int i = 0; i<tkns.size(); i++) cout << tkns[i]->ixVBasetype << endl;

	if (retval) {
		// --- find siblings
		sbls.resize(0);
		for (unsigned int i = 0; i<tkns.size(); i++) {
			if (tkns[i]->ixVTokentype == VecVTokentype::LPAR) {
				sbls.push_back(tkns[i]);
			} else if (tkns[i]->ixVTokentype == VecVTokentype::RPAR) {
				if (sbls.size() > 0) {
					sbls[sbls.size()-1]->ixSibling = i;
					sbls.pop_back();
				};
			};
		};

		for (unsigned int i = 0; i<tkns.size(); i++) {
			if (tkns[i]->ixVTokentype == VecVTokentype::LPAR) {
				if (tkns[i]->ixSibling > tkns.size()) {
					err = "token " + to_string(i) + " at position " + to_string(tkns[i]->ptr) + " does not have a sibling";

					retval = false;
					break;
				};
			};
		};
	};

	if (retval) ixVState = VecVState::TOKENIZED;
	else ixVState = VecVState::TKNERR;
	
	return retval;
};

bool Sbecore::Expr::parse() {
	if (ixVState != VecVState::TOKENIZED) return false;

	bool retval;

	root = new Node(s, tkns, 0, tkns.size()-1, err);

	retval = (root->ixVNodetype != VecVNodetype::VOID);

	if (retval) {
		root->expand();

	} else {
		delete root;
		root = NULL;

		if (err.length() == 0) err = "malformed expression";

		retval = false;
	};

	if (retval) ixVState = VecVState::PARSED;
	else ixVState = VecVState::PRSERR;

	return retval;
};

bool Sbecore::Expr::has(
			unsigned int ixVNodetype
			, const string& key
		) {
	if (ixVState == VecVState::PARSED) return root->has(ixVNodetype, key);
	else return false;
};

void Sbecore::Expr::dump() {
	if (ixVState == VecVState::PARSED) root->dump(0);
};

/******************************************************************************
 class featix_t
 ******************************************************************************/

Sbecore::featix_t::featix_t(
			const uint ixVFeatgroup
			, const string& srefIxVFeature
		) {
	this->ixVFeatgroup = ixVFeatgroup;
	this->srefIxVFeature = srefIxVFeature;
};

bool Sbecore::featix_t::operator<(
			const featix_t& comp
		) const {
	// rigged < operator to allow multimap "any" filtering in srefIxVFeature
	if (ixVFeatgroup < comp.ixVFeatgroup) return true;
	if (ixVFeatgroup != comp.ixVFeatgroup) return false;

	if ((srefIxVFeature == "") || (comp.srefIxVFeature == "")) return false;
	return(srefIxVFeature < comp.srefIxVFeature);
};

/******************************************************************************
 class Jobinfo
 ******************************************************************************/

Sbecore::Jobinfo::Jobinfo(
			const ubigint jrefSup
		) {
	this->jrefSup = jrefSup;

	ixVSge = 1;
};

/******************************************************************************
 class lockref_t
 ******************************************************************************/

Sbecore::lockref_t::lockref_t(
			const ubigint jref
			, const uint ix
		) {
	this->jref = jref;
	this->ix = ix;
};

bool Sbecore::lockref_t::operator==(
			const lockref_t& comp
		) const {
	return((jref == comp.jref) && (ix == comp.ix));
};

bool Sbecore::lockref_t::operator<(
			const lockref_t& comp
		) const {
	// rigged < operator to allow multimap "any" filtering in ix
	if (jref < comp.jref) return true;
	if (jref != comp.jref) return false;

	if ((ix == 0) || (comp.ix == 0)) return false;
	return(ix < comp.ix);
};

/******************************************************************************
 class Method
 ******************************************************************************/

Sbecore::Method::Method(
			const uint ixVFeatgroup
			, const string& srefIxVMethod
		) {
	this->ixVFeatgroup = ixVFeatgroup;
	this->srefIxVMethod = srefIxVMethod;
	
	success = false;
};

/******************************************************************************
 namespace VecOpVOpres
 ******************************************************************************/

uint Sbecore::VecOpVOpres::getIx(
			const string& sref
		) {
	if (sref.compare("progress") == 0) return PROGRESS;
	else if (sref.compare("success") == 0) return SUCCESS;
	else if (sref.compare("failure") == 0) return FAILURE;
	else if (sref.compare("commerr") == 0) return COMMERR;

	return(0);
};

string Sbecore::VecOpVOpres::getSref(
			const uint ix
		) {
	if (ix == PROGRESS) return("progress");
	else if (ix == SUCCESS) return("success");
	else if (ix == FAILURE) return("failure");
	else if (ix == COMMERR) return("commerr");

	return("");
};

/******************************************************************************
 class Op
 ******************************************************************************/

Sbecore::Op::Op(
			const ubigint oref
			, const uint ixVDpch
			, const string& squawk
		) {
	this->oref = oref;
	this->ixVDpch = ixVDpch;
	this->squawk = squawk;
};

/******************************************************************************
 class presetref_t
 ******************************************************************************/

Sbecore::presetref_t::presetref_t(
			const ubigint jref
			, const uint ixVPreset
		) {
	this->jref = jref;
	this->ixVPreset = ixVPreset;
};

bool Sbecore::presetref_t::operator<(
			const presetref_t& comp
		) const {
	// rigged < operator to allow multimap "any" filtering in ixVPreset
	if (jref < comp.jref) return true;
	if (jref != comp.jref) return false;

	if ((ixVPreset == 0) || (comp.ixVPreset == 0)) return false;
	return(ixVPreset < comp.ixVPreset);
};

/******************************************************************************
 class Preset
 ******************************************************************************/

Sbecore::Preset::Preset(
			const presetref_t& pref
			, const Arg& arg
		) {
	this->pref = pref;
	this->arg = arg;
};

/******************************************************************************
 class Resultitem
 ******************************************************************************/

Sbecore::Resultitem::Resultitem() {
};

/******************************************************************************
 class Result
 ******************************************************************************/

Sbecore::Result::Result() :
			ext(NULL)
			, mAccess("mAccess", "Result", "Result")
			, ptr0(0)
			, ptr1(0)
		{
};

Sbecore::Result::~Result() {
	clear();
};

unsigned int Sbecore::Result::getNInqueue() const {
	if (ptr1 > ptr0) return(ptr1 - ptr0);
	return((ptr1 + size()) - ptr0);
};

void Sbecore::Result::clear() {
	if (ext) return;

	for (unsigned int i = 0; i < nodes.size(); i++) delete nodes[i];
	nodes.resize(0);

	icsQueue.clear();
	ptr0 = 0;
	ptr1 = 0;

	inqueues.clear();

	locks.clear();
};

void Sbecore::Result::setExternal(
			Result* _ext
		) {
	ext = _ext;
};

unsigned int Sbecore::Result::size() const {
	if (ext) return ext->size();

	return(nodes.size());
};

void Sbecore::Result::reset() {
	if (ext) return;

	mAccess.lock("Result", "reset");

	if (getNInqueue() == size()) {
		for (unsigned int i = 0; i < size(); i++) icsQueue[i] = i;

		ptr0 = 0;
		ptr1 = 0;
	};

	mAccess.unlock("Result", "reset");
};

void Sbecore::Result::append(
			Resultitem* ri
		) {
	if (ext) return;

	mAccess.lock("Result", "append");

	if (getNInqueue() == size()) {
		nodes.push_back(ri);
		icsQueue.resize(nodes.size(), 0);
		inqueues.resize(nodes.size(), true);

		ptr0 = 0;
		ptr1 = 0;

		reset();
	};

	mAccess.unlock("Result", "append");
};

void Sbecore::Result::queue(
			const uint ix
		) {
	// append item to queue, omit duplicates
	if (ext) {
		ext->queue(ix);
		return;
	};

	mAccess.lock("Result", "queue", "ix=" + to_string(ix));

	if (ix < size()) if (!inqueues[ix]) {
		icsQueue[ptr1] = ix;

		ptr1++;
		if (ptr1 >= size()) ptr1 = 0;

		inqueues[ix] = true;
	};

	mAccess.unlock("Result", "queue", "ix=" + to_string(ix));
};

bool Sbecore::Result::dequeue(
			uint& ix
		) {
	// release first item from queue
	if (ext) return ext->dequeue(ix);

	bool success = false;

	mAccess.lock("Result", "dequeue", "ix=" + to_string(ix));

	if (getNInqueue() > 0) {
		ix = icsQueue[ptr0];

		ptr0++;
		if (ptr0 >= size()) ptr0 = 0;

		inqueues[ix] = false;

		success = true;
	};

	mAccess.unlock("Result", "dequeue", "ix=" + to_string(ix));

	return success;
};

bool Sbecore::Result::lock(
			const ubigint jref
			, const uint ix
		) {
	// add lock to item, duplicates are allowed, only permissible if item not in queue
	if (ext) return ext->lock(jref, ix);

	bool success = false;

	lockref_t lockref(jref, ix);

	mAccess.lock("Result", "lock", "jref=" + to_string(jref) + ", ix=" + to_string(ix));

	if (ix < nodes.size()) if (!inqueues[ix]) {
		locks.push_back(lockref);

		success = true;
	};

	mAccess.unlock("Result", "lock", "jref=" + to_string(jref) + ", ix=" + to_string(ix));

	return success;
};

void Sbecore::Result::unlock(
			const ubigint jref
			, const uint ix
		) {
	// remove lock from item, without checking for duplicates, put back in queue if last lock on item removed
	if (ext) {
		ext->unlock(jref, ix);
		return;
	};

	lockref_t lockref(jref, ix);

	bool found;

	mAccess.lock("Result", "unlock", "jref=" + to_string(jref) + ", ix=" + to_string(ix));

	if (ix < nodes.size()) {
		for (auto it = locks.begin(); it != locks.end(); it++)
			if (*it == lockref) {
				locks.erase(it);
				break;
			};

		found = false;

		for (auto it = locks.begin(); it != locks.end(); it++)
			if ((*it).ix == ix) {
				found = true;
				break;
			};

		if (!found) queue(ix);
	};

	mAccess.unlock("Result", "unlock", "jref=" + to_string(jref) + ", ix=" + to_string(ix));
};

void Sbecore::Result::unlockByJref(
			const ubigint jref
		) {
	// remove all locks associated with specified job / jref
	if (ext) {
		ext->unlockByJref(jref);
		return;
	};

	bool found;

	mAccess.lock("Result", "unlockByJref", "jref=" + to_string(jref));

	do {
		found = false;

		for (auto it = locks.begin(); it != locks.end(); it++) {
			if ((*it).jref == jref) {
				unlock((*it).jref, (*it).ix);
				found = true;
			};
		};

	} while (found);

	mAccess.unlock("Result", "unlockByJref", "jref=" + to_string(jref));
};

Sbecore::Resultitem* Sbecore::Result::operator[](
			const uint ix
		) {
	Resultitem* ri = NULL;

	if (ext) ri = ext->operator[](ix);
	else if (ix < nodes.size()) ri = nodes[ix];

	return ri;
};
