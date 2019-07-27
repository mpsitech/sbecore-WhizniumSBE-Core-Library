/**
  * \file Types.cpp
  * common data types, string manipulation and exception (implementation)
  * \author Alexander Wirthmüller
  * \date created: 10 Aug 2014
  * \date modified: 2 Jun 2019
  */

#include "Types.h"

/******************************************************************************
 class Doublemat
 ******************************************************************************/

Doublemat::Doublemat() {
	M = 0;
	N = 0;
};

/******************************************************************************
 class Floatmat
 ******************************************************************************/

Floatmat::Floatmat() {
	M = 0;
	N = 0;
};

/******************************************************************************
 namespace Ftm
 ******************************************************************************/

string Ftm::date(
			const unsigned int dateval
		) {
	// dateval is system time / (3600*24) ; turn into '1-1-2010' (ex.)

	if (dateval == 0) return("");

	time_t stamp = dateval*3600*24;

#ifdef _WIN32
	tm tmStamp;

	gmtime_s(&tmStamp, &stamp);

	return(to_string(tmStamp.tm_mday) + "-" + to_string(tmStamp.tm_mon + 1) + "-" + to_string(tmStamp.tm_year + 1900));
#else
	tm* tmStamp = gmtime(&stamp);

	return(to_string(tmStamp->tm_mday) + "-" + to_string(tmStamp->tm_mon+1) + "-" + to_string(tmStamp->tm_year+1900));
#endif
};

string Ftm::time(
			const int timeval
		) {
	// timeval is second count ; turn into '9:34:07' (ex.)

	string retval;

	unsigned int s, m, h;

	if (timeval < 0) {
		s = (-timeval) % 60;
		m = ((-timeval-s)/60) % 60;
		h = ((-timeval-60*m-s)/3600);
	} else {
		s = timeval % 60;
		m = ((timeval-s)/60) % 60;
		h = ((timeval-60*m-s)/3600);
	};

	if (timeval < 0) retval = "-";

	retval += to_string(h) + ":";

	if (m < 10) retval += "0";
	retval += to_string(m) + ":";

	if (s < 10) retval += "0";
	retval += to_string(s);

	return(retval);
};

string Ftm::timeOfDay(
			const unsigned int stampval
		) {
	// is system time ; turn into '9:34:07' (ex.)

	string retval;

	time_t stamp = stampval;
#ifdef _WIN32
	tm tmStamp;

	gmtime_s(&tmStamp, &stamp);

	retval = to_string(tmStamp.tm_hour) + ":";

	if (tmStamp.tm_min < 10) retval += "0";
	retval += to_string(tmStamp.tm_min) + ":";

	if (tmStamp.tm_sec < 10) retval += "0";
	retval += to_string(tmStamp.tm_sec);
#else
	tm* tmStamp = gmtime(&stamp);

	retval = to_string(tmStamp->tm_hour) + ":";

	if (tmStamp->tm_min < 10) retval += "0";
	retval += to_string(tmStamp->tm_min) + ":";

	if (tmStamp->tm_sec < 10) retval += "0";
	retval += to_string(tmStamp->tm_sec);
#endif

	return(retval);
};

string Ftm::stamp(
			const unsigned int stampval
		) {
	// is system time ; turn into '1-1-2010 9:34:07' (ex.)

	if (stampval == 0) return("");

	string retval;

	time_t stamp = stampval;
#ifdef _WIN32
	tm tmStamp;

	gmtime_s(&tmStamp, &stamp);

	retval = to_string(tmStamp.tm_mday) + "-" + to_string(tmStamp.tm_mon + 1) + "-" + to_string(tmStamp.tm_year + 1900) + " "
		+ to_string(tmStamp.tm_hour) + ":";

	if (tmStamp.tm_min < 10) retval += "0";
	retval += to_string(tmStamp.tm_min) + ":";

	if (tmStamp.tm_sec < 10) retval += "0";
	retval += to_string(tmStamp.tm_sec);
#else
	tm* tmStamp = gmtime(&stamp);

	retval = to_string(tmStamp->tm_mday) + "-" + to_string(tmStamp->tm_mon+1) + "-" + to_string(tmStamp->tm_year+1900) + " "
				+ to_string(tmStamp->tm_hour) + ":";

	if (tmStamp->tm_min < 10) retval += "0";
	retval += to_string(tmStamp->tm_min) + ":";

	if (tmStamp->tm_sec < 10) retval += "0";
	retval += to_string(tmStamp->tm_sec);
#endif

	return(retval);
};

string Ftm::hmsstamp(
			const unsigned int stampval
		) {
	// is system time ; turn into '1-1-2010_9h34m07s' (ex.)

	if (stampval == 0) return("");

	string retval;

	time_t stamp = stampval;
#ifdef _WIN32
	tm tmStamp;

	gmtime_s(&tmStamp, &stamp);

	retval = to_string(tmStamp.tm_mday) + "-" + to_string(tmStamp.tm_mon + 1) + "-" + to_string(tmStamp.tm_year + 1900) + "_"
		+ to_string(tmStamp.tm_hour) + "h";

	if (tmStamp.tm_min < 10) retval += "0";
	retval += to_string(tmStamp.tm_min) + "m";

	if (tmStamp.tm_sec < 10) retval += "0";
	retval += to_string(tmStamp.tm_sec) + "s";
#else
	tm* tmStamp = gmtime(&stamp);

	retval = to_string(tmStamp->tm_mday) + "-" + to_string(tmStamp->tm_mon+1) + "-" + to_string(tmStamp->tm_year+1900) + "_"
				+ to_string(tmStamp->tm_hour) + "h";

	if (tmStamp->tm_min < 10) retval += "0";
	retval += to_string(tmStamp->tm_min) + "m";

	if (tmStamp->tm_sec < 10) retval += "0";
	retval += to_string(tmStamp->tm_sec) + "s";
#endif

return(retval);
};

string Ftm::usecstamp(
			const double stampval
		) {
	// is system time ; turn into '1-1-2010 9:34:07 12345' (ex.)

	if (stampval == 0.0) return("");

	double usec = 1e6 * (stampval - ((double) ((unsigned int) stampval)));

	return(Ftm::stamp(stampval) + " " + to_string(lround(usec)));
};

unsigned int Ftm::invdate(
			const string& dateval
		) {
	// turn '1-1-2010' into system date (ex.)
	unsigned int retval = 0;

	string str = dateval;
	size_t ptr;

	unsigned int D, M, Y;

	time_t now;

	if ((str == "today") || (str == "&today;")) {
		now = chrono::system_clock::to_time_t(chrono::system_clock::now());
		retval = now/3600/24;

	} else {
		ptr = str.find('-');
		if (ptr != string::npos) {
			D = atoi(str.substr(0, ptr).c_str());
			str = str.substr(ptr+1);

			ptr = str.find('-');
			if (ptr != string::npos) {
				M = atoi(str.substr(0, ptr).c_str());
				str = str.substr(ptr+1);

				Y = atoi(str.c_str());

				D = D-1;
				if (M > 1) D += 31;
				if (M > 2) {
					D += 28;
					if ((Y%4) == 0) D += 1;
					if ((Y%100) == 0) D -= 1;
					if ((Y%400) == 0) D += 1;
				};
				if (M > 3) D += 31;
				if (M > 4) D += 30;
				if (M > 5) D += 31;
				if (M > 6) D += 30;
				if (M > 7) D += 31;
				if (M > 8) D += 31;
				if (M > 9) D += 30;
				if (M > 10) D += 31;
				if (M > 11) D += 30;

				retval = D + (Y-1970)*365 + ((Y-1969)/4) - ((Y-1901)/100) + ((Y-1900+299)/400);
			};
		};
	};

	return retval;
};

int Ftm::invtime(
			const string& timeval
		) {
	// turn '-0:34:07' into second count (ex.)
	int retval = 0;

	bool neg = false;

	string str = timeval;
	size_t ptr;

	unsigned int h, m, s;

	if (str.length() > 0) neg = (str[0] == '-');
	if (neg) str = str.substr(1);

	ptr = str.find(':');
	if (ptr != string::npos) {
		h = atoi(str.substr(0, ptr).c_str());

		str = str.substr(ptr+1);

		ptr = str.find(':');
		if (ptr != string::npos) {
			m = atoi(str.substr(0, ptr).c_str());
			str = str.substr(ptr+1);

			s = atoi(str.c_str());

			retval = 3600*h + 60*m + s;
			if (neg) retval = -retval;
		};
	};

	return retval;
};

unsigned int Ftm::invstamp(
			const string& stampval
		) {
	// turn '1-1-2010 9:34:07' into system time (ex.)
	unsigned int retval = 0;

	string str = stampval;
	size_t ptr;

	unsigned int D, M, Y;
	unsigned int h, m, s;

	time_t now;

	if ((str == "now") || (str == "&now;")) {
		now = chrono::system_clock::to_time_t(chrono::system_clock::now());
		retval = now;

	} else {
		ptr = str.find('-');
		if (ptr != string::npos) {
			D = atoi(str.substr(0, ptr).c_str());
			str = str.substr(ptr+1);

			ptr = str.find('-');
			if (ptr != string::npos) {
				M = atoi(str.substr(0, ptr).c_str());
				str = str.substr(ptr+1);

				ptr = str.find(' ');
				if (ptr != string::npos) {
					Y = atoi(str.substr(0, ptr).c_str());
					str = str.substr(ptr+1);

					ptr = str.find(':');
					if (ptr != string::npos) {
						h = atoi(str.substr(0, ptr).c_str());
						str = str.substr(ptr+1);

						ptr = str.find(':');
						if (ptr != string::npos) {
							m = atoi(str.substr(0, ptr).c_str());
							str = str.substr(ptr+1);

							s = atoi(str.c_str());

							D = D-1;
							if (M > 1) D += 31;
							if (M > 2) {
								D += 28;
								if ((Y%4) == 0) D += 1;
								if ((Y%100) == 0) D -= 1;
								if ((Y%400) == 0) D += 1;
							};
							if (M > 3) D += 31;
							if (M > 4) D += 30;
							if (M > 5) D += 31;
							if (M > 6) D += 30;
							if (M > 7) D += 31;
							if (M > 8) D += 31;
							if (M > 9) D += 30;
							if (M > 10) D += 31;
							if (M > 11) D += 30;

							retval = s + m*60 + h*3600 + D*86400 + (Y-1970)*31536000 + ((Y-1969)/4)*86400 - ((Y-1901)/100)*86400 + ((Y-1900+299)/400)*86400;
						};
					};
				};
			};
		};
	};

	return retval;
};

/******************************************************************************
 class SbeException
 ******************************************************************************/

SbeException::SbeException(
			const uint ix
			, const map<string,string>& vals
		) {
	this->ix = ix;
	this->vals = vals;
};

string SbeException::getSref() {
	if (ix == PATHNF) return("pathnf");
	if (ix == ENGCONN) return("engconn");

	if (ix == XMLIO_BUFPARSE) return("xmlio.bufparse");
	if (ix == XMLIO_FILEPARSE) return("xmlio.fileparse");

	if (ix == DBS_CONN) return("dbs.conn");
	if (ix == DBS_QUERY) return("dbs.query");
	if (ix == DBS_STMTPREP) return("dbs.stmtprep");
	if (ix == DBS_STMTEXEC) return("dbs.stmtexec");

	if (ix == TXTRD_TKNUNID) return("txtrd.tknunid");
	if (ix == TXTRD_TKNMISPL) return("txtrd.tknmispl");
	if (ix == TXTRD_ENDTKN) return("txtrd.endtkn");
	if (ix == TXTRD_CONTENT) return("txtrd.content");

	if (ix == IEX_FILETYPE) return("iex.filetype");
	if (ix == IEX_VERSION) return("iex.version");
	if (ix == IEX_IOP) return("iex.iop");
	if (ix == IEX_RETR) return("iex.retr");
	if (ix == IEX_IDIREF) return("iex.idiref");
	if (ix == IEX_IREF) return("iex.iref");
	if (ix == IEX_TSREF) return("iex.tsref");
	if (ix == IEX_THSREF) return("iex.thsref");
	if (ix == IEX_THINT) return("iex.thint");
	if (ix == IEX_VSREF) return("iex.vsref");
	if (ix == IEX_FTM) return("iex.ftm");
	if (ix == IEX_IARG) return("iex.iarg");

	return("");
};

string SbeException::getSquawk(
			uint (*getIx)(const string&)
			, string (*getTitle)(const uint, const uint)
			, const uint ixVLocale
		) {
	string retval;

	map<string,string>::iterator it, it2;

	it = vals.find("tid");
	if (it != vals.end()) retval = it->second;

	it = vals.find("object");
	it2 = vals.find("member");

	if (it != vals.end()) retval += it->second;
	if ((it != vals.end()) && (it2 != vals.end())) retval += "::";
	if (it2 != vals.end()) retval += it2->second + "()";
	if ((it != vals.end()) || (it2 != vals.end())) retval += " ";

	retval += getTitle(getIx(getSref()), ixVLocale);
	
	for (it=vals.begin();it!=vals.end();it++) retval = StrMod::replacePlh(retval, it->first, it->second);

	return retval;
};

/******************************************************************************
 namespace StrMod
 ******************************************************************************/

string StrMod::cap(
			const string& s
		) {
	string retval = s;

	if (s.length() > 0) {
		if (retval[0] == '&') {
			if (s.length() > 1) retval[1] = toupper(retval[1]);
		 } else {
			retval[0] = toupper(retval[0]);
		};
	};

	return(retval);
};

string StrMod::uncap(
			const string& s
		) {
	string retval = s;

	if (s.length() > 0) {
		if (retval[0] == '&') {
			if (s.length() > 1) retval[1] = tolower(retval[1]);
		 } else {
			retval[0] = tolower(retval[0]);
		};
	};

	return(retval);
};

string StrMod::lc(
			const string& s
		) {
	string retval = s;
	for (unsigned int i=0;i<s.length();i++) retval[i] = tolower(retval[i]);

	return(retval);
};

string StrMod::uc(
			const string& s
		) {
	string retval = s;
	for (unsigned int i=0;i<s.length();i++) retval[i] = toupper(retval[i]);

	return(retval);
};

string StrMod::spcex(
			const string& s
		) {
	string retval = s;

	while (retval.length() > 0) if (retval[0] == ' ') retval = retval.substr(1); else break;
	if (retval.length() > 0) while (retval[retval.length()-1] == ' ') retval = retval.substr(0, retval.length()-1);

	return(retval);
};

string StrMod::esc(
			const string& s
		) {
	string retval = s;

	size_t ptr = 0;

	ptr = retval.find('\\');
	while (ptr != string::npos) {
		retval = retval.substr(0, ptr) + "\\" + retval.substr(ptr);
		ptr = retval.find('\\', ptr+2);
	};

	return(retval);
};

string StrMod::dotToUsc(
			const string& s
		) {
	string retval = s;
	for (unsigned int i=0;i<s.length();i++) if ((retval[i] == '-') || (retval[i] == '.')) retval[i] = '_';

	return(retval);
};

string StrMod::uscToCap(
			const string& s
		) {
	string retval = s;

	size_t ptr = 0;
	
	ptr = retval.find('_');
	while (ptr != string::npos) {
		if ((ptr+1) < retval.length()) retval = retval.substr(0, ptr) + cap(retval.substr(ptr+1));
		else retval = retval.substr(0, ptr);

		ptr = retval.find('_', ptr);
	};

	return(retval);
};

string StrMod::boolToString(
			const bool b
		) {
	if (b) return("true");
	else return("false");
};

string StrMod::timetToString(
			const time_t rawtime
		) {
	string retval;

#ifdef _WIN32
	tm exttime;

	gmtime_s(&exttime, &rawtime);

	retval = to_string(exttime.tm_mday);

	switch (exttime.tm_mon) {
#else
	tm* exttime = gmtime(&rawtime);

	retval = to_string(exttime->tm_mday);

	switch (exttime->tm_mon) {
#endif
	case 0:
		retval += " Jan ";
		break;
	case 1:
		retval += " Feb ";
		break;
	case 2:
		retval += " Mar ";
		break;
	case 3:
		retval += " Apr ";
		break;
	case 4:
		retval += " May ";
		break;
	case 5:
		retval += " Jun ";
		break;
	case 6:
		retval += " Jul ";
		break;
	case 7:
		retval += " Aug ";
		break;
	case 8:
		retval += " Sep ";
		break;
	case 9:
		retval += " Oct ";
		break;
	case 10:
		retval += " Nov ";
		break;
	case 11:
		retval += " Dec ";
		break;
	};

#ifdef _WIN32
	retval += to_string(1900 + exttime.tm_year);
#else
	retval += to_string(1900+exttime->tm_year);
#endif

	return(retval);
};

bool StrMod::has(
			const vector<string>& vec
			, const string& str
		) {
	bool found = false;

	for (unsigned int i=0;i<vec.size();i++) {
		if (vec[i] == str) {
			found = true;
			break;
		};
	};

	return found;
};

void StrMod::stringToVector(
			const string& str
			, vector<string>& vec
			, const char sep
		) {
	unsigned int len = str.length();

	unsigned int start = 0;
	bool ignore = false;

	char c;

	vec.resize(0);

	for (unsigned int i=0;i<len;i++) {
		c = str[i];

		if (c == '&') {
			ignore = true;
		} else if (c == sep) {
			if (ignore) {
				ignore = false;
			} else {
				vec.push_back(str.substr(start, i-start));
				start = i+1;
			};
		};
	};
	if (start < len) vec.push_back(str.substr(start));
	else if (start == len) if (len != 0) if (str[start-1] == sep) vec.push_back("");

	for (unsigned int i=0;i<vec.size();i++) vec[i] = spcex(vec[i]);
};

void StrMod::stringToDoublevec(
			const string& str
			, vector<double>& vec
			, const char sep
		) {
	vector<string> _vec;

	stringToVector(str, _vec, sep);

	vec.resize(_vec.size());
	for (unsigned int i=0;i<_vec.size();i++) vec[i] = atof(_vec[i].c_str());
};

void StrMod::vectorToString(
			const vector<string>& vec
			, string& str
			, const char sep
		) {
	str = "";
	for (unsigned int i=0;i<vec.size();i++) str += sep + vec[i];

	if (str.length() > 0) str = str.substr(1);
};

bool StrMod::srefInSrefs(
			const string& srefs
			, const string& sref
		) {
	bool isin = false;
	bool bgnvalid, endvalid;

	size_t srefslen = srefs.length();
	size_t sreflen = sref.length();

	size_t ptr1, ptr2;

	ptr1 = 0;
	ptr2 = srefs.find(sref, ptr1);

	while (ptr2 != string::npos) {
		bgnvalid = false;
		endvalid = false;

		// validate beginning
		if (ptr2 == 0) {
			bgnvalid = true;
		} else {
			if ((srefs[ptr2-1] == ';') || (srefs[ptr2-1] == ' ')) bgnvalid = true;
		};

		// validate ending
		if (bgnvalid) {
			if ((ptr2+sreflen) == srefslen) {
				endvalid = true;
			} else {
				if ((srefs[ptr2+sreflen] == ';') || (srefs[ptr2+sreflen] == ' ')) endvalid = true;
			};
		};

		if (bgnvalid && endvalid) {
			isin = true;
			break;
		} else {
			ptr1 = ptr2+1;
			ptr2 = srefs.find(sref, ptr1);
		};
	};

	return isin;
};

void StrMod::refsToVector(
			const string& refs
			, vector<ubigint>& vec
		) {
	vector<string> strvec;

	stringToVector(refs, strvec);

	vec.resize(0);

	for (unsigned int i=0;i<strvec.size();i++) vec.push_back(atoll(strvec[i].c_str()));
};

string StrMod::replaceChar(
			const string& s
			, const char c
			, const char d
		) {
	string retval = s;

	for (size_t ptr=0;ptr<retval.length();ptr++) if (retval[ptr] == c) retval[ptr] = d;

	return retval;
};

void StrMod::findPlhs(
			const string& s
			, set<string>& plhs
			, const bool add
		) {
	if (!add) plhs.clear();

	size_t ptr1, ptr2;

	ptr1 = s.find('&');
	while (ptr1 != string::npos) {
		ptr2 = s.find(';', ptr1);
		if (ptr2 != string::npos) plhs.insert(s.substr(ptr1+1, ptr2-ptr1-1));
		ptr1 = s.find('&', ptr2);
	};
};

string StrMod::findFirstPlh(
			const string& s
			, size_t start
		) {
	string retval;

	size_t ptr1, ptr2;

	ptr1 = s.find('&', start);
	if (ptr1 != string::npos) {
		ptr2 = s.find(';', ptr1);

		if (ptr2 != string::npos) retval = s.substr(ptr1+1, ptr2-ptr1-1);
	};

	return retval;
};

string StrMod::replacePlh(
			const string& s
			, const string& plh
			, const double val
		) {
	return(replacePlh(s, plh, to_string(val)));
};

string StrMod::replacePlh(
			const string& s
			, const string& plh
			, const string& val
		) {
	string retval = s;

	string s2 = "&" + plh + ";";

	size_t ptr = retval.find(s2);
	while (ptr != string::npos) {
		retval.replace(ptr, s2.length(), val);
		ptr = retval.find(s2);
	};

	return retval;
};

unsigned int StrMod::getCharcnt(
			const string& s
		) {
	unsigned int retval = s.length();

	size_t ptr;

	ptr = s.find("\\u");
	while (ptr != string::npos) {
		retval -= 5;
		ptr = s.find("\\u", ptr+1);
	};

	if (retval > s.length()) retval = s.length();

	return retval;
};

string StrMod::readLine(
			ifstream& infile
			, char* buf
			, const size_t buflen
		) {
	string retval;

	size_t ptr;

	infile.getline(buf, buflen, '\n');
	retval = string(buf);

	if (retval.length() > 0) {
		ptr = retval.rfind('\r');
		if (ptr != string::npos) retval = retval.substr(0, ptr);
	};

	return retval;
};

/******************************************************************************
 class Version
 ******************************************************************************/

Version::Version(
			const string& _s
		) {
	vector<string> ss;
	string s;

	s = StrMod::spcex(_s);
	if (s.length() > 0) if ((s[0] == 'V') || (s[0] == 'v')) s = s.substr(1);

	StrMod::stringToVector(s, ss, '.');

	for (unsigned int i=0;i<ss.size();i++) is.push_back(atoi(ss[i].c_str()));
};

bool Version::defined() const {
	return(is.size() > 0);
};

bool Version::operator<(
			const Version& comp
		) const {
	// I. 3.0 < 3.1
	// II. 5.1 < 5.1.7
	// III. 5.1.7 < 5.2

	for (unsigned int i=0;i<is.size();i++) {
		if (i < comp.is.size()) {
			if (is[i] < comp.is[i]) return true;
			else if (is[i] > comp.is[i]) return false;
		} else return true; // III.
	};
	
	if (is.size() == comp.is.size()) return false; // I. ==
	else return true; // II.
};

bool Version::operator<=(
			const Version& comp
		) const {
	return(!operator>(comp));
};

bool Version::operator>(
			const Version& comp
		) const {
	return(comp.operator<(*this));
};

bool Version::operator>=(
			const Version& comp
		) const {
	return(!operator<(comp));
};

bool Version::operator==(
			const Version& comp
		) const {
	return(!operator!=(comp));
};

bool Version::operator!=(
			const Version& comp
		) const {
	return(operator>(comp) || operator<(comp));
};

string Version::to_string() const {
	string retval = "-";

	for (unsigned int i=0;i<is.size();i++) {
		if (i == 0) retval = std::to_string(is[0]);
		else retval += "." + std::to_string(is[i]);
	};

	return retval;
};
