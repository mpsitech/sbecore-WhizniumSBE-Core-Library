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
	if (sref == "my") return MY;
	if (sref == "pg") return PG;
	if (sref == "lite") return LITE;

	return(0);
};

string VecDbsVDbstype::getSref(
			const uint ix
		) {
	if (ix == MY) return("my");
	if (ix == PG) return("pg");
	if (ix == LITE) return("lite");

	return("");
};
