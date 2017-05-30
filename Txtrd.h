/**
  * \file Txtrd.h
  * methods for reading hierarchical text input (declarations)
  * \author Alexander Wirthmüller
  * \date created: 8 Oct 2015
  * \date modified: 8 Oct 2015
  */

#ifndef SBECORE_TXTRD_H
#define SBECORE_TXTRD_H

#include <fstream>

#include <sbecore/Types.h>

/**
  * Txtrd
  */
class Txtrd {

public:
	Txtrd(uint (*getIxVToken)(const string&));
	~Txtrd();

public:
	uint (*getIxVToken)(const string&);

	ifstream infile;
	char* buf;

	bool skip;
	unsigned int linecnt;
	string line;

	unsigned int il;
	bool comment;
	bool header;
	bool data;
	uint ixVToken;
	vector<string> fields;

public:
	void openFile(const string& fullpath);
	void closeFile();

	bool readLine();
	void tokenizeLine();
};
#endif
