/**
  * \file Txtrd.h
  * methods for reading hierarchical text input (declarations)
  * \author Alexander Wirthmüller
  * \date created: 8 Oct 2015
  * \date modified: 25 Jan 2019
  */

#ifndef SBECORE_TXTRD_H
#define SBECORE_TXTRD_H

#include <fstream>

#include <sbecore/Engtypes.h>

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
	Txtrd(uint (*getIxVToken)(const string&));
///
	Txtrd(const string& fullpath, const string& iexsref, const Version& minversion, uint (*getIxVToken)(const string&));
	~Txtrd();

public:
	uint (*getIxVToken)(const string&);

	ifstream infile;
	char* buf;

	bool skip;
	unsigned int linecnt;
	string line;

	unsigned int il;
///
	uint ixVLinetype; // {header, data, footer, comment}
/// > OUT!
	bool comment;
	bool header;
	bool data;
/// <
	uint ixVToken;
	vector<string> fields;

public:
/// > OUT!
	void openFile(const string& fullpath, const string& iexsref = "", const Version& minversion = Version(""));
	void closeFile();

///
	bool eof();

	bool readLine();
	void tokenizeLine();
};
#endif
