/**
  * \file Txtrd.h
  * methods for reading hierarchical text input (declarations)
  * \author Alexander Wirthmüller
  * \date created: 8 Oct 2015
  * \date modified: 22 Apr 2020
  */

#ifndef SBECORE_TXTRD_H
#define SBECORE_TXTRD_H

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
			static const uint VOID = 1;
			static const uint HEADER = 2;
			static const uint DATA = 3;
			static const uint FOOTER = 4;
			static const uint COMMENT = 5;
		};

	public:
		Txtrd(const std::string& fullpath, const std::string& iexsref, const Version& minversion, uint (*getIxVToken)(const std::string&));
		~Txtrd();

	public:
		uint (*getIxVToken)(const std::string&);

		std::ifstream infile;
		char* buf;

		bool skip;
		unsigned int linecnt;
		std::string line;

		unsigned int il;
		uint ixVLinetype;
		uint ixVToken;
		std::vector<std::string> fields;

	public:
		bool eof();

		bool readLine();
		void tokenizeLine();
	};
};
#endif
