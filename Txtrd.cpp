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

Txtrd::Txtrd(
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


Txtrd::~Txtrd() {
	if (infile.is_open()) infile.close();
	delete[] buf;
};

void Txtrd::openFile(
			const string& fullpath
			, const string& iexsref
			, const Version& minversion
		) {
	infile.open(fullpath.c_str(), ifstream::in);
	if (infile.fail()) throw SbeException(SbeException::PATHNF, {{"path",fullpath}});

	// tolerance towards identifier line, e.g. 'IexWznmDbs v0.9.14'
	if (readLine())
		if (ixVLinetype == VecVLinetype::DATA)
			if (fields.size() >= 1)
				if (fields[0].find("Iex") == 0) return;

	skip = true;
};

void Txtrd::closeFile() {
	infile.close();
};

bool Txtrd::eof() {
	if (!infile.good()) return false;
	return infile.eof();
};

bool Txtrd::readLine() {
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

void Txtrd::tokenizeLine() {
	string dline;

	size_t ptr;

	il = 0;
///
	ixVLinetype = VecVLinetype::VOID;
/// > OUT
	comment = false;
	header = false;
	data = false;
/// <
	ixVToken = 0;
	fields.resize(0);		

	// find insertion level
	while (line[il] == '\t') il++;

	line = line.substr(il);

///
	if (line.find("- ") == 0) line = line.substr(2);

	if (line.find("//") == 0) {
///
		ixVLinetype = VecVLinetype::COMMENT;
		comment = true;

	} else {
		// check for header tokens
		ptr = line.find('.');
		if (ptr != string::npos) {
			ixVToken = getIxVToken(line.substr(0, ptr));

			if (ixVToken != 0) {
				if (line.substr(ptr).find(".end") == 0) {
					ixVLinetype = VecVLinetype::FOOTER;
/// OUT:
					comment = true; // helps skipping footer lines in old iex code
				} else {
					ixVLinetype = VecVLinetype::HEADER;
/// OUT:
					header = true;
				};
			};
		};

		if (ixVLinetype == VecVLinetype::VOID) {
			// regular data row - split at \t character

///
			ixVLinetype = VecVLinetype::DATA;
			data = true;

			while (line.find('\t') != string::npos) {
				dline = line.substr(0, line.find('\t'));
				fields.push_back(dline);

				line = line.substr(line.find('\t')+1);
			};
			if (line.length() > 0) fields.push_back(line);
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
