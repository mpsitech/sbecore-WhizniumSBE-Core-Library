/**
  * \file Stub.h
  * stub cache functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 6 Oct 2015
  * \date modified: 9 Oct 2015
  */

#ifndef SBECORE_STUB_H
#define SBECORE_STUB_H

#include <set>

#include <sbecore/Strmod.h>

/**
  * stcchitemref_t
  */
class stcchitemref_t {

public:
	stcchitemref_t(const uint ixVStub = 0, const ubigint ref = 0, const uint ixVLocale = 0);

public:
	uint ixVStub;
	ubigint ref;
	uint ixVLocale;

public:
	bool operator<(const stcchitemref_t& comp) const;
};

/**
  * Stcchitem
  */
class Stcchitem {

public:
	Stcchitem(const stcchitemref_t& stref, const string& stub = "");

public:
	stcchitemref_t stref;
	string stub;

	set<stcchitemref_t> strefsSup;
	set<stcchitemref_t> strefsSub;
};

/**
  * Stcch
  */
class Stcch {

public:
	Stcch(const bool transact);

public:
	bool transact;

	multimap<stcchitemref_t,Stcchitem*> nodes;

	set<uint> icsVStub;
	set<uint> icsVStubNew;

public:
	Stcchitem* addStit(const stcchitemref_t& stref, const string& stub = "");
	Stcchitem* getStitByStref(const stcchitemref_t& stref);

	void clear();

	void link(const stcchitemref_t& strefSup, const stcchitemref_t& strefSub);
	void unlink(const stcchitemref_t& strefSup, const stcchitemref_t& strefSub);

	void begin();
	void commit();
};

/**
  * Stub
  */
namespace Stub {
	/**
		* VecVNonetype
		*/
	namespace VecVNonetype {
		const uint VOID = 1;
		const uint DASH = 2;
		const uint FULL = 3;
		const uint SHORT = 4;

		uint getIx(const string& sref);
		string getSref(const uint ix);
	};
};

#endif
