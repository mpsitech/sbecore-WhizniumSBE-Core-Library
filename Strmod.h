/**
  * \file Strmod.h
  * methods for string modification (declarations)
  * \author Alexander Wirthmüller
  * \date created: 10 Aug 2014
  * \date modified: 6 Mar 2016
  */

#ifndef SBECORE_STRMOD_H
#define SBECORE_STRMOD_H

#include <cmath>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include <sbecore/Types.h>

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
};
#endif
