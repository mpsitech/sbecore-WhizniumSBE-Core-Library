/**
  * \file MyDbs.h
  * database access code globals for MySQL (implementation)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 10 Aug 2014
  */

#include "MyDbs.h"

/******************************************************************************
 class MyTable
 ******************************************************************************/

MyTable::MyTable() {
};

MyTable::~MyTable() {
};

void MyTable::init(
			MYSQL* _dbs
		) {
	dbs = _dbs;
	initStatements();
};

void MyTable::initStatements() {
};

MYSQL_BIND MyTable::bindTinyint(
			tinyint* t
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(tinyint);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_TINY;
	result.buffer = t;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = false;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindUtinyint(
			utinyint* t
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(utinyint);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_TINY;
	result.buffer = t;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = true;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindSmallint(
			smallint* s
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(smallint);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_SHORT;
	result.buffer = s;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = false;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindUsmallint(
			usmallint* s
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(usmallint);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_SHORT;
	result.buffer = s;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = true;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindInt(
			int* i
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(int);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_LONG;
	result.buffer = i;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = false;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindUint(
			uint* i
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(uint);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_LONG;
	result.buffer = i;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = true;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindBigint(
			bigint* b
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(bigint);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_LONGLONG;
	result.buffer = b;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = false;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindUbigint(
			ubigint* b
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(ubigint);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_LONGLONG;
	result.buffer = b;
	result.length = length;
	result.is_null = is_null;
	result.is_unsigned = true;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindCstring(
			char* str
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*is_null = false;

	// note that for write to database, this should be MYSQL_TYPE_STRING
	result.buffer_type = MYSQL_TYPE_VAR_STRING;
	result.buffer = str;
	// for read from database
	result.buffer_length = *length;
	// for write to database
	result.length = length;
	result.is_null = is_null;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindCstring0(
			unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*is_null = false;

	result.buffer_type = MYSQL_TYPE_VAR_STRING;
	result.buffer = NULL;
	result.buffer_length = 0;
	result.length = length;
	result.is_null = is_null;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_BIND MyTable::bindDouble(
			double* d
			, unsigned long* length
			, my_bool* is_null
			, my_bool* error
		) {
	MYSQL_BIND result;
	memset(&result, 0, sizeof(MYSQL_BIND));

	*length = sizeof(double);
	*is_null = false;

	result.buffer_type = MYSQL_TYPE_DOUBLE;
	result.buffer = d;
	result.length = length;
	result.is_null = is_null;
#if MYSQL_VERSION_ID  >= 50003
	result.error = error;
#endif

	return(result);
};

MYSQL_STMT* MyTable::createStatement(
			const string stmtSQL
			, my_bool getlength
		) {
	MYSQL_STMT* result = mysql_stmt_init(dbs);
	if (!result) throw DbsException("DbsException / MySQL: unable to initialize statement '" + stmtSQL + "'\n");

	if (mysql_stmt_prepare(result, stmtSQL.c_str(), stmtSQL.length()))
		throw DbsException("DbsException / MySQL: unable to initialize statement '" + stmtSQL + "'\n");
	if (getlength) mysql_stmt_attr_set(result, STMT_ATTR_UPDATE_MAX_LENGTH, &getlength);

	return(result);
};

void MyTable::begin() {
	if (mysql_query(dbs, "BEGIN")) throw DbsException("DbsException / MySQL: failed to begin transaction\n");
};

bool MyTable::commit() {
	if (mysql_query(dbs, "COMMIT")) {
		rollback();
		return false;
	};

	return true;
};

void MyTable::rollback() {
	if (mysql_query(dbs, "ROLLBACK")) throw DbsException("DbsException / MySQL: failed to roll back transaction\n");
};

bool MyTable::loadUintBySQL(
			const string& sqlstr
			, uint& val
		) {
	MYSQL_RES* dbresult; MYSQL_ROW dbrow;

	bool retval = false;

	if (mysql_real_query(dbs, sqlstr.c_str(), sqlstr.length())) throw DbsException("DbsException / MySQL: failed to execute query '" + sqlstr + "'\n");

	dbresult = mysql_store_result(dbs);
	if (!dbresult) throw DbsException("DbsException / MySQL: failed to store result! (loadUintBySQL)\n");

	if (mysql_num_rows(dbresult) == 1) {
		dbrow = mysql_fetch_row(dbresult);
		val = atol((char*) dbrow[0]);

		retval = true;
	};
	
	mysql_free_result(dbresult);

	return retval;
};

bool MyTable::loadStringBySQL(
			const string& sqlstr
			, string& val
		) {
	MYSQL_RES* dbresult; MYSQL_ROW dbrow; unsigned long* dblengths;

	bool retval = false;

	if (mysql_real_query(dbs, sqlstr.c_str(), sqlstr.length())) throw DbsException("DbsException / MySQL: failed to execute query '" + sqlstr + "'\n");

	dbresult = mysql_store_result(dbs);
	if (!dbresult) throw DbsException("DbsException / MySQL: failed to store result! (loadStringBySQL)\n");

	if (mysql_num_rows(dbresult) == 1) {
		dbrow = mysql_fetch_row(dbresult);
		dblengths = mysql_fetch_lengths(dbresult);
		val.assign(dbrow[0], dblengths[0]);

		retval = true;
	};
	
	mysql_free_result(dbresult);

	return retval;
};

bool MyTable::loadRefBySQL(
			const string& sqlstr
			, ubigint& ref
		) {
	MYSQL_RES* dbresult; MYSQL_ROW dbrow;

	bool retval = false;

	if (mysql_real_query(dbs, sqlstr.c_str(), sqlstr.length())) throw DbsException("DbsException / MySQL: failed to execute query '" + sqlstr + "'\n");

	dbresult = mysql_store_result(dbs);
	if (!dbresult) throw DbsException("DbsException / MySQL: failed to store result! (loadRefBySQL)\n");

	if (mysql_num_rows(dbresult) == 1) {
		dbrow = mysql_fetch_row(dbresult);
		ref = atoll((char*) dbrow[0]);

		retval = true;
	};
	
	mysql_free_result(dbresult);

	return retval;
};

ubigint MyTable::loadRefsBySQL(
			const string& sqlstr
			, const bool append
			, vector<ubigint>& refs
		) {
	MYSQL_RES* dbresult; MYSQL_ROW dbrow; ubigint numrow; ubigint numread = 0;

	if (mysql_real_query(dbs, sqlstr.c_str(), sqlstr.length())) throw DbsException("DbsException / MySQL: failed to execute query '" + sqlstr + "'\n");

	dbresult = mysql_store_result(dbs);
	if (!dbresult) throw DbsException("DbsException / MySQL: failed to store result! (loadRefsBySQL)\n");

	numrow = mysql_num_rows(dbresult);
	if (!append) refs.resize(0);

	refs.reserve(refs.size() + numrow);

	while (numread < numrow) {
		dbrow = mysql_fetch_row(dbresult);
		refs.push_back(atoll((char*) dbrow[0]));
		numread++;
	};

	mysql_free_result(dbresult);
	
	return(numread);
};
