/**
  * \file Stub.cpp
  * stub cache functionality (implementation)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 9 Oct 2015
  */

#include "Stub.h"

/******************************************************************************
 class stcchitemref_t
 ******************************************************************************/

stcchitemref_t::stcchitemref_t(
			const uint ixVStub
			, const ubigint ref
			, const uint ixVLocale
		) {
	this->ixVStub = ixVStub;
	this->ref = ref;
	this->ixVLocale = ixVLocale;
};

bool stcchitemref_t::operator<(
			const stcchitemref_t& comp
		) const {
	if (ixVStub < comp.ixVStub) return true;
	else if (ixVStub > comp.ixVStub) return false;

	if ((ref == 0) || (comp.ref == 0)) return false;
	if (ref < comp.ref) return true;
	else if (ref > comp.ref) return false;

	if ((ixVLocale == 0) || (comp.ixVLocale == 0)) return false;
	return(ixVLocale < comp.ixVLocale);
};

/******************************************************************************
 class Stcchitem
 ******************************************************************************/

Stcchitem::Stcchitem(
			const stcchitemref_t& stref
			, const string& stub
		) {
	this->stref = stref;
	this->stub = stub;
};

/******************************************************************************
 class Stcch
 ******************************************************************************/

Stcch::Stcch(
			const bool transact
		) {
	this->transact = transact;
};

Stcchitem* Stcch::addStit(
			const stcchitemref_t& stref
			, const string& stub
		) {
	Stcchitem* stit = NULL;
	
	stit = getStitByStref(stref);

	if (!stit) {
		stit = new Stcchitem(stref, stub);
		nodes.insert(pair<stcchitemref_t,Stcchitem*>(stref, stit));

		if (transact) {
			auto it = icsVStub.find(stref.ixVStub);
			if (it == icsVStub.end()) icsVStubNew.insert(stref.ixVStub);
		};
	};

	return stit;
};

Stcchitem* Stcch::getStitByStref(
			const stcchitemref_t& stref
		) {
	Stcchitem* stit = NULL;

	if ((stref.ref != 0) && (stref.ixVLocale != 0)) {
		auto it = nodes.find(stref);
		if (it != nodes.end()) stit = it->second;
	};

	return stit;
};

void Stcch::clear() {
	for (auto it=nodes.begin();it!=nodes.end();it++) delete it->second;
	nodes.clear();

	icsVStub.clear();
	icsVStubNew.clear();
};

void Stcch::link(
			const stcchitemref_t& strefSup
			, const stcchitemref_t& strefSub
		) {
	Stcchitem* stitSup = NULL;
	Stcchitem* stitSub = NULL;

	if (transact) {
		stitSup = getStitByStref(strefSup);
		stitSub = getStitByStref(strefSub);

		if (stitSup && stitSub) {
			stitSup->strefsSub.insert(strefSub);
			stitSub->strefsSup.insert(strefSup);
		};
	};
};

void Stcch::unlink(
			const stcchitemref_t& strefSup
			, const stcchitemref_t& strefSub
		) {
	Stcchitem* stitSup = NULL;
	Stcchitem* stitSub = NULL;

	if (transact) {
		stitSup = getStitByStref(strefSup);
		stitSub = getStitByStref(strefSub);

		if (stitSup && stitSub) {
			stitSup->strefsSub.erase(strefSub);
			stitSub->strefsSup.erase(strefSup);
		};
	};
};

void Stcch::begin() {
	if (transact) icsVStubNew.clear();
};

void Stcch::commit() {
	if (transact) {
		for (auto it=icsVStubNew.begin();it!=icsVStubNew.end();it++) icsVStub.insert(*it);
		icsVStubNew.clear();
	};
};

/******************************************************************************
 namespace Stub::VecVNonetype
 ******************************************************************************/

uint Stub::VecVNonetype::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("void") == 0) return VOID;
	else if (s.compare("dash") == 0) return DASH;
	else if (s.compare("full") == 0) return FULL;
	else if (s.compare("short") == 0) return SHORT;

	return(0);
};

string Stub::VecVNonetype::getSref(
			const uint ix
		) {
	if (ix == VOID) return("void");
	else if (ix == DASH) return("dash");
	else if (ix == FULL) return("full");
	else if (ix == SHORT) return("short");

	return("");
};
