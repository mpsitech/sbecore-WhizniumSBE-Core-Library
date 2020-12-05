/**
	* \file Stub.h
	* stub cache functionality (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 6 Oct 2015
	*/

#ifndef SBECORE_STUB_H
#define SBECORE_STUB_H

#include <set>

#include <sbecore/Types.h>

namespace Sbecore {
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
		Stcchitem(const stcchitemref_t& stref, const std::string& stub = "");

	public:
		stcchitemref_t stref;
		std::string stub;

		std::set<stcchitemref_t> strefsSup;
		std::set<stcchitemref_t> strefsSub;
	};

	/**
		* Stcch
		*/
	class Stcch {

	public:
		Stcch(const bool transact);

	public:
		bool transact;

		std::multimap<stcchitemref_t,Stcchitem*> nodes;

		std::set<uint> icsVStub;
		std::set<uint> icsVStubNew;

	public:
		Stcchitem* addStit(const stcchitemref_t& stref, const std::string& stub = "");
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

			uint getIx(const std::string& sref);
			std::string getSref(const uint ix);
		};
	};
};
#endif
