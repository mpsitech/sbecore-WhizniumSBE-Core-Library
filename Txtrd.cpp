/**
  * \file Txtrd.cpp
  * methods for reading hierarchical text input (implementation)
  * \author Alexander Wirthmüller
  * \date created: 8 Oct 2015
  * \date modified: 22 Apr 2020
  */

#include "Txtrd.h"

using namespace std;

/******************************************************************************
 class Txtrd
 ******************************************************************************/

Sbecore::Txtrd::Txtrd(
			const string& fullpath
			, const string& iexsref
			, const Version& minversion
			, uint (*getIxVToken)(const string&)
		) {
	string s;

	this->getIxVToken = getIxVToken;

	buf = new char[4096];

	skip = false;

	linecnt = 0;

	infile.open(fullpath.c_str(), ifstream::in);
	if (infile.fail()) throw SbeException(SbeException::PATHNF, {{"path",fullpath}});

	if (iexsref != "") {
		if (readLine()) {
			if (ixVLinetype == VecVLinetype::DATA) {
				if (fields.size() >= 1) {
					s = fields[0];
					
					if (s.find(iexsref) == 0) {
						Version version(s.substr(iexsref.length()));

						if (version.defined() && minversion.defined())
							if (version < minversion)
								throw SbeException(SbeException::IEX_VERSION, {{"version",version.to_string()},{"minversion",minversion.to_string()}});

					} else throw SbeException(SbeException::IEX_FILETYPE, {{"iexsref",iexsref}});
				} else throw SbeException(SbeException::IEX_FILETYPE, {{"iexsref",iexsref}});
			} else throw SbeException(SbeException::IEX_FILETYPE, {{"iexsref",iexsref}});
		} else throw SbeException(SbeException::IEX_FILETYPE, {{"iexsref",iexsref}});
	};
};


Sbecore::Txtrd::~Txtrd() {
	if (infile.is_open()) infile.close();
	delete[] buf;
};

bool Sbecore::Txtrd::eof() {
	if (!infile.good()) return false;
	return infile.eof();
};

bool Sbecore::Txtrd::readLine() {
	if (skip) {
		skip = false;
		return true;
	};

	if (!infile.good()) return false;
	if (infile.eof()) return false;

	line = StrMod::readLine(infile, buf, 4096);

	linecnt++;

	if (line.length() == 0) return false;

	tokenizeLine();

	return true;
};

void Sbecore::Txtrd::tokenizeLine() {
	string dline;

	size_t ptr;

	il = 0;
	ixVLinetype = VecVLinetype::VOID;
	ixVToken = 0;
	fields.resize(0);		

	// find insertion level
	while (line[il] == '\t') il++;

	line = line.substr(il);

	if (line.find("- ") == 0) line = line.substr(2);

	if (line.find("//") == 0) {
		ixVLinetype = VecVLinetype::COMMENT;

	} else {
		// check for header tokens
		ptr = line.find('.');
		if (ptr != string::npos) {
			ixVToken = getIxVToken(line.substr(0, ptr));

			if (ixVToken != 0) {
				if (line.substr(ptr).find(".end") == 0) {
					ixVLinetype = VecVLinetype::FOOTER;
				} else {
					ixVLinetype = VecVLinetype::HEADER;
				};
			};
		};

		if (ixVLinetype == VecVLinetype::VOID) {
			// regular data row - split at \t character
			ixVLinetype = VecVLinetype::DATA;

			while (line.find('\t') != string::npos) {
				dline = line.substr(0, line.find('\t'));
				fields.push_back(dline);

				line = line.substr(line.find('\t')+1);
			};
			if (line.length() > 0) fields.push_back(line);
		};
	};

//	cout << "line: il=" << il;
//	cout << " ixVLinetype=" << ixVLinetype;
//	cout << " ixVToken=" << ixVToken;
//	cout << " fields.size()=" << fields.size();
//	cout << endl;
};
