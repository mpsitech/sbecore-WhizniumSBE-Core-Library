/**
  * \file Types.cpp
  * basic data types and functional expression evaluation (implementation)
  * \author Alexander Wirthmüller
  * \date created: 10 Aug 2014
  * \date modified: 17 Sep 2015
  */

#include "Types.h"

/******************************************************************************
 class Arg
 ******************************************************************************/

Arg::Arg(
			const uint ix
			, const ubigint ref
			, const vector<ubigint>& refs
			, const string& sref
			, const int intval
			, const double dblval
			, const bool boolval
			, const string& txtval
			, const ubigint mask
		) {
	this->mask = mask;

	if (mask & IX) this->ix = ix; else this->ix = 0;
	if (mask & REF) this->ref = ref; else this->ref = 0;
	if (mask & REFS) this->refs = refs; else this->refs.resize(0);
	if (mask & SREF) this->sref = sref;
	if (mask & INTVAL) this->intval = intval; else this->intval = 0;
	if (mask & BOOLVAL) this->boolval = boolval; else this->boolval = false;
	if (mask & TXTVAL) this->txtval = txtval;
};

bool Arg::operator==(
			const Arg& comp
		) const {
	return((mask == comp.mask) && (ix == comp.ix) && (ref == comp.ref) && (refs == comp.refs) && (sref == comp.sref)
				&& (intval == comp.intval) && (dblval == comp.dblval) && (boolval == comp.boolval) && (txtval == comp.txtval));
};

bool Arg::operator!=(
			const Arg& comp
		) const {
	return(!operator==(comp));
};

string Arg::writeText() const {
	string retval;

	if (mask == IX) retval = to_string(ix);
	else if (mask == REF) retval = to_string(ref);
	else if (mask == REFS) {
		retval = "{";
		for (unsigned int i=0;i<refs.size();i++) {
			if (i != 0) retval += ",";
			retval += to_string(refs[i]);
		};
		retval += "}";
	} else if (mask == SREF) retval = sref;
	else if (mask == INTVAL) retval = to_string(intval);
	else if (mask == BOOLVAL) {
		if (boolval) retval = "true"; else retval = "false";
	} else if (mask == TXTVAL) retval = txtval;
	else if (mask != 0) retval = "(multiple components)";
	else retval = "(empty)";

	return(retval);
};

/******************************************************************************
 class Expr::VecVTokentype
 ******************************************************************************/

unsigned int Expr::VecVTokentype::getIx(
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

string Expr::VecVTokentype::getSref(
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

Expr::Token::Token(
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

unsigned int Expr::VecVNodetype::getIx(
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

string Expr::VecVNodetype::getSref(
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

Expr::Node::Node(
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
		for (unsigned int i=0;i<icsTknstrength.size();i++) icsTknstrength[i] = ixTkn1+1;
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
		for (unsigned int i=0;i<icsTknstrength.size();i++) {
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

				for (unsigned int i=0;i<icsTkncomma.size();i++) {
					subs.push_back(new Node(s, tkns, ixTkn, icsTkncomma[i]-1, err));
					ixTkn = icsTkncomma[i]+1;
				};

				if (ixTkn != ixTkn1) subs.push_back(new Node(s, tkns, ixTkn, ixTkn1-1, err));

			} else {
				ixVNodetype = VecVNodetype::VOID;
				err = "invalid token " + to_string(ixTkn) + " at position " + to_string(tkn->ptr);
			};

			for (unsigned int i=0;i<subs.size();i++) {
				if (subs[i]->ixVNodetype == VecVNodetype::VOID) {
					ixVNodetype = VecVNodetype::VOID;
					break;
				};
			};
		};
	};
};

Expr::Node::~Node() {
	for (unsigned int i=0;i<subs.size();i++) delete subs[i];
	subs.resize(0);
};

void Expr::Node::expand() {
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
		for (unsigned int i=0;i<subs.size();i++) subs[i]->expand();
	};
};

bool Expr::Node::has(
			unsigned int _ixVNodetype
			, const string& _key
		) {
	bool retval = (_ixVNodetype == ixVNodetype);
	if (retval && (_key.length() > 0)) retval = (key.compare(_key) == 0);

	if (!retval) {
		for (unsigned int i=0;i<subs.size();i++) {
			retval = subs[i]->has(_ixVNodetype, _key);
			if (retval) break;
		};
	};

	return retval;
};

bool Expr::Node::logicfct() {
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

bool Expr::Node::logictree() {
	bool retval = false;

	if (subs.size() == 0) {
		retval = true;

	} else {
		retval = logicfct();

		if (retval) {
			for (unsigned int i=0;i<subs.size();i++) {
				retval = subs[i]->logictree();
				if (!retval) break;
			};
		};
	};

	return retval;
};

void Expr::Node::fctToLeaves(
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
		for (unsigned int i=0;i<subs.size();i++) subs[i]->fctToLeaves(fctkey);
	};
};

void Expr::Node::dump(
			unsigned int il
		) {
	for (unsigned int i=0;i<il;i++) cout << "\t";

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

	for (unsigned int i=0;i<subs.size();i++) subs[i]->dump(il+1);
};

/******************************************************************************
 class Expr
 ******************************************************************************/

Expr::Expr() {
	root = NULL;

	ixVState = VecVState::RESET;
};

Expr::~Expr() {
	reset();
};

void Expr::reset() {
	for (unsigned int i=0;i<tkns.size();i++) delete tkns[i];
	tkns.resize(0);

	if (root) {
		delete root;
		root = NULL;
	};

	ixVState = VecVState::RESET;
	err = "";
};

bool Expr::tokenize(
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

//	for (unsigned int i=0;i<tkns.size();i++) cout << tkns[i]->ixVBasetype << endl;

	if (retval) {
		// --- find siblings
		sbls.resize(0);
		for (unsigned int i=0;i<tkns.size();i++) {
			if (tkns[i]->ixVTokentype == VecVTokentype::LPAR) {
				sbls.push_back(tkns[i]);
			} else if (tkns[i]->ixVTokentype == VecVTokentype::RPAR) {
				if (sbls.size() > 0) {
					sbls[sbls.size()-1]->ixSibling = i;
					sbls.pop_back();
				};
			};
		};

		for (unsigned int i=0;i<tkns.size();i++) {
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

bool Expr::parse() {
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

bool Expr::has(
			unsigned int ixVNodetype
			, const string& key
		) {
	if (ixVState == VecVState::PARSED) return root->has(ixVNodetype, key);
	else return false;
};

void Expr::dump() {
	if (ixVState == VecVState::PARSED) root->dump(0);
};

/******************************************************************************
 class Refseq
 ******************************************************************************/

Refseq::Refseq(
			const string& sref
		) {
	this->sref = sref;

	Mutex::init(&mAccess, true, "mAccess", "Refseq(" + sref + ")", "Refseq");

	ref = 0;
};

Refseq::~Refseq() {
	Mutex::destroy(&mAccess, true, "mAccess", "Refseq(" + sref + ")", "~Refseq");
};

ubigint Refseq::getNewRef() {
	ubigint ref_backup;

	Mutex::lock(&mAccess, "mAccess", "Refseq(" + sref + ")", "getNewRef");

	ref++;
	ref_backup = ref;

	Mutex::unlock(&mAccess, "mAccess", "Refseq(" + sref + ")", "getNewRef");

	return ref_backup;
};

/******************************************************************************
 namespace Scr
 ******************************************************************************/

string Scr::scramble(
			pthread_mutex_t* mScr
			, map<ubigint,string>& scr
			, map<string,ubigint>& descr
			, const ubigint ref
		) {
	string retval;

	if (ref == 0) return("");

	Mutex::lock(mScr, "mScr", "Scr", "scramble");

	auto it = scr.find(ref);

	if (it == scr.end()) {
		retval = random();
		scr[ref] = retval;
		descr[retval] = ref;
	} else {
		retval = it->second;
	};

	Mutex::unlock(mScr, "mScr", "Scr", "scramble");

	return retval;
};

ubigint Scr::descramble(
			pthread_mutex_t* mScr
			, map<string,ubigint>& descr
			, const string& scrRef
		) {
	ubigint retval = 0;

	Mutex::lock(mScr, "mScr", "Scr", "descramble");

	auto it = descr.find(scrRef);
	if (it != descr.end()) retval = it->second;

	Mutex::unlock(mScr, "mScr", "Scr", "descramble");

	return retval;
};

string Scr::random() {
	string retval;

	int digit;

	// assume advance random seed

	// fetch digits and make sure their ASCII code is in the range 0..9/a..z
	for (unsigned int i=0;i<16;i++) {
		digit = rand() % 36 + 48;
		if (digit > 57) digit += (97-48-10);

		retval = retval + ((char) digit);
	};

	return retval;
};
