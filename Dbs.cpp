/**
  * \file Dbs.cpp
  * database access code globals (implementation)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 10 Aug 2014
  */

#include "Dbs.h"

/******************************************************************************
 namespace VecDbsVDbstype
 ******************************************************************************/

uint VecDbsVDbstype::getIx(
			const string& sref
		) {
	if (sref.compare("my") == 0) return MY;
	else if (sref.compare("pg") == 0) return PG;
	else if (sref.compare("lite") == 0) return LITE;

	return(0);
};

string VecDbsVDbstype::getSref(
			const uint ix
		) {
	if (ix == MY) return("my");
	else if (ix == PG) return("pg");
	else if (ix == LITE) return("lite");

	return("");
};
