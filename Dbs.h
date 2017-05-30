/**
  * \file Dbs.h
  * database access code globals (declarations)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 10 Aug 2014
  */

#ifndef SBECORE_DBS_H
#define SBECORE_DBS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#include <sbecore/Types.h>

/**
  * VecDbsVDbstype
  */
namespace VecDbsVDbstype {
	const uint MY = 1;
	const uint PG = 2;
	const uint LITE = 3;

	uint getIx(const string& sref);
	string getSref(const uint ix);
};

/**
  * DbsException
  */
struct DbsException {
	string err;
	
	DbsException(
				string err
			) {
		this->err = err;
	};
};
#endif
