/**
	* \file Txtrd.cpp
	* methods for reading hierarchical text input (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 8 Oct 2015
	*/

#include "Txtrd.h"

using namespace std;

/******************************************************************************
 class Txtrd
 ******************************************************************************/

Sbecore::Txtrd::Txtrd(
			const string& inpath
			, const string& rectpath
			, const string& iexsref
			, const Version& minversion
			, uint (*getIxVToken)(const string&)
		) {
	string s;

	this->getIxVToken = getIxVToken;

	buf = new char[4096];

	conv = iconv_open("ISO-8859-1", "MACINTOSH");

	skip = false;

	linecnt = 0;

	hdril = 0;

	infile.open(inpath.c_str(), ifstream::in);
	if (infile.fail()) throw SbeException(SbeException::PATHNF, {{"path",inpath}});

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

	if (rectpath != "") {
		rectfile.open(rectpath.c_str(), ios::out);
		if (rectfile.fail()) throw SbeException(SbeException::PATHNF, {{"path",rectpath}});

		rectfile << fields[0] << endl;
	};
};

Sbecore::Txtrd::~Txtrd() {
	if (infile.is_open()) infile.close();
	delete[] buf;
	iconv_close(conv);

	if (rectfile.is_open()) rectfile.close();
};

bool Sbecore::Txtrd::eof() {
	if (!infile.good()) return false;
	return infile.eof();
};

bool Sbecore::Txtrd::readLine() {
	string s;
	vector<string> ss;

	char* inbuf;
	size_t inlen;
	
	char* outbuf;
	size_t outlen;

	size_t Nconv;

	bool findil;

	size_t ptr;

	if (skip) {
		skip = false;
		return true;
	};

	if (!infile.good()) {
		//cout << "returning because infile not good" << endl;
		return false;
	};
	if (infile.eof()) {
		//cout << "returning at infile EOF" << endl;
		return false;
	};

	// read in \r / \n agnostic way
	s = StrMod::readLine(infile, buf, 4096);
	linecnt++;

	if (s.length() == 0) {
		//cout << "returning because of empty line" << endl;
		return false;
	};

	// perform character encoding conversions
	inbuf = (char*) s.c_str();
	inlen = s.length();

	outbuf = buf;
	outlen = 4095;

	Nconv = iconv(conv, &inbuf, &inlen, &outbuf, &outlen);

	if (Nconv == ((size_t) -1)) {
		//cout << "returning because of iconv problem (" << errno << "): s='" << s << "' inlen=" << inlen << ", outlen=" << outlen << endl;
		return false;
	};

	if (Nconv > 0) {
		//cout << "line " << linecnt << " modified by iconv" << endl;
		buf[4095 - outlen] = '\0';
		s = string(buf);
	};

	strISO8859(s);

	// split
	StrMod::stringToVector(s, ss, '\t');

	// remove quotes (MS Excel convention)
	for (unsigned int i = 0; i < ss.size(); i++) {
		s = ss[i];

		if (s.length() >= 2) if ((s[0] == '\"') && (s[s.length()-1] == '\"')) {
			s = s.substr(1, s.length() - 2);

			ptr = s.find("\"\"");
			while (ptr != string::npos) {
				s = s.substr(0, ptr) + s.substr(ptr + 1);
				ptr = s.find("\"\"", ptr + 1);
			};

			ss[i] = s;
		};
	};

	// find indentation, identify tokens
	findil = true;

	il = 0;
	ixVLinetype = VecVLinetype::DATA;
	ixVToken = 0;
	fields.resize(0);		

	for (unsigned int i = 0; i < ss.size(); i++) {
		s = ss[i];

		if (findil) {
			if (s == "") il++;
			else findil = false;
		};

		if (!findil) {
			if (s.find("//") == 0) ixVLinetype = VecVLinetype::COMMENT;

			else if (s == "^") {
				ss[i] = "";

			} else {
				ptr = s.find('.');
				if (ptr != string::npos) {
					ixVToken = getIxVToken(s.substr(0, ptr));

					if (ixVToken != 0) {
						hdril = il;

						if (s.substr(ptr).find(".end") == 0) {
							ixVLinetype = VecVLinetype::FOOTER;
							if (il != 0) hdril--;

						} else {
							ixVLinetype = VecVLinetype::HEADER;
						};
					};
				};
			};

			break;
		};
	};

	// in this case, stringToVector() returns one more vector item than is actually present
	if (findil) il--;

	// adjust data and comment lines to header
	if (findil) il = hdril;
	else if (!findil && (il > hdril)) il = hdril;

	// identify appended empty fields
	for (ptr = ss.size(); ptr > 0; ptr--) if (ss[ptr - 1] != "") break;

	if (ixVLinetype == VecVLinetype::DATA) {
		// extract fields
		for (unsigned int i = il; i < ptr; i++) fields.push_back(ss[i]);
	};

	if (rectfile.is_open()) {
		// write rectified line
		rectfile << string(il, '\t');

		for (unsigned int i = il; i < ptr; i++) {
			if (i != il) rectfile << "\t";
			rectfile << ss[i];
		};

		rectfile << endl;
	};

	//cout << "line " << linecnt << ": il=" << il;
	//cout << " ixVLinetype=" << ixVLinetype;
	//cout << " ixVToken=" << ixVToken;
	//cout << " fields.size()=" << fields.size();
	//cout << endl;

	return true;
};

string Sbecore::Txtrd::unicodeToHexcode(
			unsigned int unicode
		) {
	string hexcode;

	unsigned char c;

	if (unicode > 65535) unicode = 20;

	c = unicode / 4096;
	if (c < 10) hexcode = c+0x30; else hexcode = c-10+0x61;
	unicode -= c*4096;

	c = unicode / 256;
	if (c < 10) hexcode += c+0x30; else hexcode += c-10+0x61;
	unicode -= c*256;

	c = unicode / 16;
	if (c < 10) hexcode += c+0x30; else hexcode += c-10+0x61;
	unicode -= c*16;

	c = unicode;
	if (c < 10) hexcode += c+0x30; else hexcode += c-10+0x61;

	return hexcode;
};

void Sbecore::Txtrd::strISO8859(
			string& s
		) {
	size_t ptr, ptr1, ptr2;

	int unicode;

	unsigned char ci;
	string hexcode;

	ptr1 = s.find("&#");
	while (ptr1 != string::npos) {
		// find closing semicolon
		ptr2 = s.find(";", ptr1+2);
		if (ptr2 == string::npos) break;

		// make sure closest brace is found
		ptr = s.find("&#", ptr1+2);
		if (ptr != string::npos) {
			if (ptr < ptr2) {
				ptr1 = ptr;
				continue;
			};
		};

		// insert \u.... (hex) instead of HTML expression
		unicode = atoi(s.substr(ptr1+2, ptr2-ptr1-2).c_str());

		s = s.substr(0, ptr1) + "\\u" + unicodeToHexcode(unicode) + s.substr(ptr2+1);
		ptr1 = s.find("&#", ptr1+2);
	};

	// replace characters using MSB by \u.... (hex)
	for (unsigned int i = 0; i < s.length();) {
		ci = ((unsigned char) s[i]);

		if (ci > 0x7f) {
			s = s.substr(0, i) + "\\u" + unicodeToHexcode(ci) + s.substr(i+1);
			i += 6;

		} else i++;
	};
};
