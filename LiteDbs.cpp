/**
	* \file LiteDbs.h
	* database access code globals for SQLite (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 1 Jan 2009
	*/

#include "LiteDbs.h"

using namespace std;

/******************************************************************************
 class LiteTable
 ******************************************************************************/

Sbecore::LiteTable::LiteTable() {
};

Sbecore::LiteTable::~LiteTable() {
};

void Sbecore::LiteTable::init(
			sqlite3* _dbs
		) {
	dbs = _dbs;
	initStatements();
};

void Sbecore::LiteTable::initStatements() {
};

sqlite3_stmt* Sbecore::LiteTable::createStatement(
			const string stmtSQL
		) {
	int res;
	sqlite3_stmt* result = NULL;

	res = sqlite3_prepare_v2(dbs, stmtSQL.c_str(), -1, &result, NULL);
	if (res != SQLITE_OK) throw SbeException(SbeException::DBS_STMTPREP, {{"dbms","LiteTable::createStatement()"}, {"sql",stmtSQL}});

	return(result);
};

void Sbecore::LiteTable::begin() {
	if (sqlite3_exec(dbs, "BEGIN", NULL, NULL, NULL) != SQLITE_OK) throw SbeException(SbeException::DBS_QUERY, {{"dbms","LiteTable::begin()"}, {"sql","BEGIN"}});
};

bool Sbecore::LiteTable::commit() {
	if (sqlite3_exec(dbs, "COMMIT", NULL, NULL, NULL) != SQLITE_OK) {
		rollback();
		return false;
	};

	return true;
};

void Sbecore::LiteTable::rollback() {
	if (sqlite3_exec(dbs, "ROLLBACK", NULL, NULL, NULL) != SQLITE_OK) throw SbeException(SbeException::DBS_QUERY, {{"dbms","LiteTable::rollback()"}, {"sql","ROLLBACK"}});
};

bool Sbecore::LiteTable::loadUintByStmt(
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

bool Sbecore::LiteTable::loadStringByStmt(
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

bool Sbecore::LiteTable::loadRefByStmt(
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

Sbecore::ubigint Sbecore::LiteTable::loadRefsByStmt(
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
