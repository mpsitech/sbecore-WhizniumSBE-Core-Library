/**
  * \file Txtrd.cpp
  * methods for reading hierarchical text input (implementation)
  * \author Alexander Wirthmüller
  * \date created: 8 Oct 2015
  * \date modified: 8 Oct 2015
  */

#include "Txtrd.h"

/******************************************************************************
 class Txtrd
 ******************************************************************************/

Txtrd::Txtrd(
			uint (*getIxVToken)(const string&)
		) {
	this->getIxVToken = getIxVToken;

	buf = new char[4096];

	skip = false;

	linecnt = 0;
};

Txtrd::~Txtrd() {
	delete[] buf;
};

void Txtrd::openFile(
			const string& fullpath
		) {
	infile.open(fullpath.c_str(), ifstream::in);
};

void Txtrd::closeFile() {
	infile.close();
};

bool Txtrd::readLine() {
	if (skip) {
		skip = false;
		return true;
	};

	if (!infile.good()) return false;
	if (infile.eof()) return false;

	infile.getline(buf, 4096, '\n');
	line = string(buf);
	linecnt++;

	if (line.length() == 0) return false;

	tokenizeLine();

	return true;
};

void Txtrd::tokenizeLine() {
	string dline;

	il = 0;
	comment = false;
	header = false;
	data = false;
	ixVToken = 0;
	fields.resize(0);		

	// find insertion level
	while (line[il] == '\t') il++;

	line = line.substr(il);

	if (line.find("- ") == 0) {
		line = line.substr(2);

		if (line.find("//") == 0) {
			comment = true;

		} else {
			// check for header tokens
			if (line.find('.') != string::npos) {
				ixVToken = getIxVToken(line.substr(0, line.find('.')));

				header = (ixVToken != 0);
			};

			if (!header) {
				// regular data row - split at \t character

				data = true;

				while (line.find('\t') != string::npos) {
					dline = line.substr(0, line.find('\t'));
					fields.push_back(dline);

					line = line.substr(line.find('\t')+1);
				};
				if (line.length() > 0) fields.push_back(line);
			};
		};
	};

//	cout << "line: il=" << il;
//	if (comment) cout << " comment";
//	if (header) cout << " header";
//	if (data) cout << " data";
//	cout << " ixVToken=" << ixVToken;
//	cout << " fields.size()=" << fields.size();
//	cout << endl;
};
