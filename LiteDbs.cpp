/**
  * \file LiteDbs.h
  * database access code globals for SQLite (implementation)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 10 Aug 2014
  */

#include "LiteDbs.h"

/******************************************************************************
 class LiteTable
 ******************************************************************************/

LiteTable::LiteTable() {
};

LiteTable::~LiteTable() {
};

void LiteTable::init(
			sqlite3* _dbs
		) {
	dbs = _dbs;
	initStatements();
};

void LiteTable::initStatements() {
};

sqlite3_stmt* LiteTable::createStatement(
			const string stmtSQL
		) {
	int res;
	sqlite3_stmt* result = NULL;

	res = sqlite3_prepare_v2(dbs, stmtSQL.c_str(), -1, &result, NULL);
	if (res != SQLITE_OK) throw SbeException(SbeException::DBS_STMTPREP, {{"dbms","LiteTable::createStatement()"}, {"sql",stmtSQL}});

	return(result);
};

void LiteTable::begin() {
	if (sqlite3_exec(dbs, "BEGIN", NULL, NULL, NULL) != SQLITE_OK) throw SbeException(SbeException::DBS_QUERY, {{"dbms","LiteTable::begin()"}, {"sql","BEGIN"}});
};

bool LiteTable::commit() {
	if (sqlite3_exec(dbs, "COMMIT", NULL, NULL, NULL) != SQLITE_OK) {
		rollback();
		return false;
	};

	return true;
};

void LiteTable::rollback() {
	if (sqlite3_exec(dbs, "ROLLBACK", NULL, NULL, NULL) != SQLITE_OK) throw SbeException(SbeException::DBS_QUERY, {{"dbms","LiteTable::rollback()"}, {"sql","ROLLBACK"}});
};

bool LiteTable::loadUbigintByStmt(
			sqlite3_stmt* stmt
			, ubigint& val
		) {
	int res;

	bool retval = false;

	res = sqlite3_step(stmt);
	if (res == SQLITE_ROW) {
		val = sqlite3_column_int64(stmt, 0);

		res = sqlite3_step(stmt);
		if (res == SQLITE_DONE) retval = true;
	};

	sqlite3_reset(stmt);

	return retval;
};

bool LiteTable::loadUintByStmt(
			sqlite3_stmt* stmt
			, uint& val
		) {
	int res;

	bool retval = false;

	res = sqlite3_step(stmt);
	if (res == SQLITE_ROW) {
		val = sqlite3_column_int(stmt, 0);

		res = sqlite3_step(stmt);
		if (res == SQLITE_DONE) retval = true;
	};

	sqlite3_reset(stmt);

	return retval;
};

bool LiteTable::loadStringByStmt(
			sqlite3_stmt* stmt
			, string& val
		) {
	int res;

	bool retval = false;

	res = sqlite3_step(stmt);
	if (res == SQLITE_ROW) {
		val.assign((const char*) sqlite3_column_text(stmt, 0));

		res = sqlite3_step(stmt);
		if (res == SQLITE_DONE) retval = true;
	};

	sqlite3_reset(stmt);

	return retval;
};

bool LiteTable::loadRefByStmt(
			sqlite3_stmt* stmt
			, ubigint& ref
		) {
	int res;

	bool retval = false;

	res = sqlite3_step(stmt);
	if (res == SQLITE_ROW) {
		ref = sqlite3_column_int64(stmt, 0);

		res = sqlite3_step(stmt);
		if (res == SQLITE_DONE) retval = true;
	};

	sqlite3_reset(stmt);

	return retval;
};

ubigint LiteTable::loadRefsByStmt(
			sqlite3_stmt* stmt
			, const bool append
			, vector<ubigint>& refs
		) {
	int res; ubigint numread = 0;

	if (!append) refs.resize(0);

	res = sqlite3_step(stmt);
	while (res == SQLITE_ROW) {
		refs.push_back(sqlite3_column_int64(stmt, 0));
		numread++;

		res = sqlite3_step(stmt);
	};

	sqlite3_reset(stmt);

	return(numread);
};
