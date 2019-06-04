/**
  * \file Types.h
  * common data types, string manipulation and exception (declarations)
  * \author Alexander Wirthmüller
  * \date created: 10 Aug 2014
  * \date modified: 2 Jun 2019
  */

#ifndef SBECORE_TYPES_H
#define SBECORE_TYPES_H

typedef char tinyint;
typedef unsigned char utinyint;
typedef short int smallint;
typedef unsigned short int usmallint;
typedef unsigned int uint;
typedef long long int bigint;
typedef unsigned long long int ubigint;

#ifdef _WIN32
	#include <time.h>
#endif

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include <sbecore/config.h>

/**
  * Doublemat
  */
class Doublemat {

public:
	Doublemat();
	
public:
	vector<double> vec;
	unsigned int M;
	unsigned int N;
};

/**
  * Floatmat
  */
class Floatmat {

public:
	Floatmat();
	
public:
	vector<float> vec;
	unsigned int M;
	unsigned int N;
};

/**
  * Ftm
  */
namespace Ftm {
	string date(const unsigned int dateval); // is system time / (3600*24)
	string time(const int timeval); // independent of system time
	string timeOfDay(const unsigned int stampval); // is system time
	string stamp(const unsigned int stampval); // is system time
	string hmsstamp(const unsigned int stampval); // is system time
	string usecstamp(const double stampval); // is system time
	unsigned int invdate(const string& dateval);
	int invtime(const string& timeval);
	unsigned int invstamp(const string& stampval);
};

/**
  * SbeException
  */
class SbeException {

public:
	static const uint PATHNF = 1;
	static const uint ENGCONN = 2;

	static const uint XMLIO_BUFPARSE = 101;
	static const uint XMLIO_FILEPARSE = 102;

	static const uint DBS_CONN = 201;
	static const uint DBS_QUERY = 202;
	static const uint DBS_STMTPREP = 203;
	static const uint DBS_STMTEXEC = 204;

	static const uint TXTRD_TKNUNID = 301;
	static const uint TXTRD_TKNMISPL = 302;
	static const uint TXTRD_ENDTKN = 303;
	static const uint TXTRD_CONTENT = 304;

	static const uint IEX_FILETYPE = 401;
	static const uint IEX_VERSION = 402;
	static const uint IEX_IOP = 403;
	static const uint IEX_RETR = 404;
	static const uint IEX_IDIREF = 405;
	static const uint IEX_IREF = 406;
	static const uint IEX_TSREF = 407;
	static const uint IEX_THSREF = 408;
	static const uint IEX_THINT = 409;
	static const uint IEX_VSREF = 410;
	static const uint IEX_FTM = 411;
	static const uint IEX_IARG = 412;

public:
	SbeException(const uint ix, const map<string,string>& vals);

public:
	uint ix;
	map<string,string> vals;

public:
	string getSref();
	string getSquawk(uint (*getIx)(const string&), string (*getTitle)(const uint, const uint), const uint ixVLocale);
};

/**
  * StrMod
  */
namespace StrMod {
	string cap(const string& s);
	string uncap(const string& s);
	string lc(const string& s);
	string uc(const string& s);
	string spcex(const string& s);
	string esc(const string& s);
	string dotToUsc(const string& s);
	string uscToCap(const string& s);
	string boolToString(const bool b);
	string timetToString(const time_t rawtime);
	bool has(const vector<string>& vec, const string& str);
	void stringToVector(const string& str, vector<string>& vec, const char sep = ';');
	void stringToDoublevec(const string& str, vector<double>& vec, const char sep = ';');
	void vectorToString(const vector<string>& vec, string& str, const char sep = ';');
	bool srefInSrefs(const string& srefs, const string& sref);
	void refsToVector(const string& refs, vector<ubigint>& vec);
	string replaceChar(const string& s, const char c, const char d);
	void findPlhs(const string& s, set<string>& plhs, const bool add = false);
	string findFirstPlh(const string& s, size_t start);
	string replacePlh(const string& s, const string& plh, const double val);
	string replacePlh(const string& s, const string& plh, const string& val);
	unsigned int getCharcnt(const string& s);
	string readLine(ifstream& infile, char* buf, size_t buflen);
};

/**
  * Version
  */
class Version {

public:
	Version(const string& _s = "");

public:
	vector<unsigned int> is;

public:
	bool defined() const;
	bool operator<(const Version& comp) const;
	bool operator<=(const Version& comp) const;
	bool operator>(const Version& comp) const;
	bool operator>=(const Version& comp) const;
	bool operator==(const Version& comp) const;
	bool operator!=(const Version& comp) const;

	string to_string() const;
};

#endif
