/**
  * \file Types.h
  * common data types, string manipulation and exception (declarations)
  * \author Alexander Wirthmüller
  * \date created: 10 Aug 2014
  * \date modified: 22 Apr 2020
  */

#ifndef SBECORE_TYPES_H
#define SBECORE_TYPES_H

#ifdef _WIN32
	#include <time.h>
#endif

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <sbecore/config.h>

namespace Sbecore {
	typedef char tinyint;
	typedef unsigned char utinyint;
	typedef short int smallint;
	typedef unsigned short int usmallint;
	typedef unsigned int uint;
	typedef long long int bigint;
	typedef unsigned long long int ubigint;

	/**
		* Arg
		*/
	class Arg {

	public:
		static const ubigint IX = 1;
		static const ubigint REF = 2;
		static const ubigint REFS = 4;
		static const ubigint SREF = 8;
		static const ubigint INTVAL = 16;
		static const ubigint DBLVAL = 32;
		static const ubigint BOOLVAL = 64;
		static const ubigint TXTVAL = 128;
		static const ubigint ALL = 255;

	public:
		Arg(const uint ix = 0, const ubigint ref = 0, const std::vector<ubigint>& refs = {}, const std::string& sref = "", const int intval = 0, const double dblval = 0.0, const bool boolval = false, const std::string& txtval = "", const ubigint mask = 0);

	public:
		ubigint mask;

		uint ix;
		ubigint ref;
		std::vector<ubigint> refs;
		std::string sref;
		int intval;
		double dblval;
		bool boolval;
		std::string txtval;

	public:
		bool operator==(const Arg& comp) const;
		bool operator!=(const Arg& comp) const;
		bool operator<(const Arg& comp) const;

		void clearContent();

		std::string getMaskSrefs() const;
		std::string to_string() const;
	};

	/**
		* Doublemat
		*/
	class Doublemat {

	public:
		Doublemat();
		
	public:
		std::vector<double> vec;
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
		std::vector<float> vec;
		unsigned int M;
		unsigned int N;
	};

	/**
		* Ftm
		*/
	namespace Ftm {
		std::string date(const unsigned int dateval); // is system time / (3600*24)
		std::string time(const int timeval); // independent of system time
		std::string timeOfDay(const unsigned int stampval); // is system time
		std::string stamp(const unsigned int stampval); // is system time
		std::string hmsstamp(const unsigned int stampval); // is system time
		std::string usecstamp(const double stampval); // is system time
		unsigned int invdate(const std::string& dateval);
		int invtime(const std::string& timeval);
		unsigned int invstamp(const std::string& stampval);
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
		SbeException(const uint ix, const std::map<std::string,std::string>& vals);

	public:
		uint ix;
		std::map<std::string,std::string> vals;

	public:
		std::string getSref();
		std::string getSquawk(uint (*getIx)(const std::string&), std::string (*getTitle)(const uint, const uint), const uint ixVLocale);
	};

	/**
		* StrMod
		*/
	namespace StrMod {
		std::string cap(const std::string& s);
		std::string uncap(const std::string& s);
		std::string lc(const std::string& s);
		std::string uc(const std::string& s);
		std::string spcex(const std::string& s);
		std::string esc(const std::string& s);
		std::string dotToUsc(const std::string& s);
		std::string uscToCap(const std::string& s);
		std::string boolToString(const bool b);
		std::string timetToString(const time_t rawtime);
		bool has(const std::vector<std::string>& vec, const std::string& str);
		void stringToVector(const std::string& str, std::vector<std::string>& vec, const char sep = ';');
		void stringToDoublevec(const std::string& str, std::vector<double>& vec, const char sep = ';');
		void vectorToString(const std::vector<std::string>& vec, std::string& str, const char sep = ';');
		bool srefInSrefs(const std::string& srefs, const std::string& sref);
		void refsToVector(const std::string& refs, std::vector<ubigint>& vec);
		std::string replaceChar(const std::string& s, const char c, const char d);
		void findPlhs(const std::string& s, std::set<std::string>& plhs, const bool add = false);
		std::string findFirstPlh(const std::string& s, size_t start);
		std::string replacePlh(const std::string& s, const std::string& plh, const double val);
		std::string replacePlh(const std::string& s, const std::string& plh, const std::string& val);
		unsigned int getCharcnt(const std::string& s);
		std::string readLine(std::ifstream& infile, char* buf, size_t buflen);
	};

	/**
		* Version
		*/
	class Version {

	public:
		Version(const std::string& _s = "");

	public:
		std::vector<unsigned int> is;

	public:
		bool defined() const;
		bool operator<(const Version& comp) const;
		bool operator<=(const Version& comp) const;
		bool operator>(const Version& comp) const;
		bool operator>=(const Version& comp) const;
		bool operator==(const Version& comp) const;
		bool operator!=(const Version& comp) const;

		std::string to_string() const;
	};
};

#endif
