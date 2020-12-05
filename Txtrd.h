/**
	* \file Txtrd.h
	* methods for reading hierarchical text input (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 8 Oct 2015
	*/

#ifndef SBECORE_TXTRD_H
#define SBECORE_TXTRD_H

#include <iconv.h>

#include <fstream>

#include <sbecore/Engtypes.h>

namespace Sbecore {
	/**
		* Txtrd
		*/
	class Txtrd {

	public:
		/**
			* VecVLinetype
			*/
		class VecVLinetype {

		public:
			static const uint HEADER = 1;
			static const uint DATA = 2;
			static const uint FOOTER = 3;
			static const uint COMMENT = 4;
		};

	public:
		Txtrd(const std::string& inpath, const std::string& rectpath, const std::string& iexsref, const Version& minversion, uint (*getIxVToken)(const std::string&));
		~Txtrd();

	public:
		uint (*getIxVToken)(const std::string&);

		std::ifstream infile;
		char* buf;
		iconv_t conv;

		std::fstream rectfile;

		bool skip;
		unsigned int hdril;
		unsigned int linecnt;

		unsigned int il;
		uint ixVLinetype;
		uint ixVToken;
		std::vector<std::string> fields;

	public:
		bool eof();

		bool readLine();

		std::string unicodeToHexcode(unsigned int unicode);
		void strISO8859(std::string& s);
	};
};
#endif
