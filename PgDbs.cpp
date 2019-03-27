/**
  * \file PgDbs.h
  * database access code globals for PostgreSQL (implementation)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 28 Feb 2019
  */

#include "PgDbs.h"

/******************************************************************************
 class PgTable
 ******************************************************************************/

PgTable::PgTable() {
};

PgTable::~PgTable() {
};

void PgTable::init(
			PGconn* _dbs
		) {
	dbs = _dbs;
	initStatements();
};

void PgTable::initStatements() {
};

/// WILL BECOME OBSOLETE !!!
void PgTable::initStatementsErr(
			PGresult* res
		) {
};

void PgTable::createStatement(
			const string& stmtName
			, const string& query
			, const int nParams
		) {
	PGresult* res;

	res = PQprepare(dbs, stmtName.c_str(), query.c_str(), nParams, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) throw SbeException(SbeException::DBS_STMTPREP, {{"dbms","PgTable::createStatement()"}, {"sql",query}});
};

void PgTable::begin() {
	PGresult* res;

	res = PQexec(dbs, "BEGIN");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) throw SbeException(SbeException::DBS_QUERY, {{"dbms","PgTable::begin()"}, {"sql","BEGIN"}});

	PQclear(res);
};

bool PgTable::commit() {
	PGresult* res;

	res = PQexec(dbs, "COMMIT");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		rollback();
		PQclear(res);
		return false;
	};

	PQclear(res);
	return true;
};

void PgTable::rollback() {
	PGresult* res;

	res = PQexec(dbs, "ROLLBACK");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) throw SbeException(SbeException::DBS_QUERY, {{"dbms","PgTable::rollback()"}, {"sql","ROLLBACK"}});

	PQclear(res);
};

bool PgTable::loadUbigintByStmt(
			const string& srefStmt
			, const unsigned int N
			, const char** vals
			, const int* l
			, const int* f
			, ubigint& val
		) {
	PGresult* res; char* ptr;

	bool retval = false;
	
	res = PQexecPrepared(dbs, srefStmt.c_str(), N, vals, l, f, 0);
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw SbeException(SbeException::DBS_STMTEXEC, {{"dbms","PgTable::loadUbigintByStmt()"}});

	if (PQntuples(res) == 1) {
		ptr = PQgetvalue(res, 0, 0);
		val = atoll(ptr);

		retval = true;
	};

	PQclear(res);

	return retval;
};

bool PgTable::loadUintByStmt(
			const string& srefStmt
			, const unsigned int N
			, const char** vals
			, const int* l
			, const int* f
			, uint& val
		) {
	PGresult* res; char* ptr;

	bool retval = false;
	
	res = PQexecPrepared(dbs, srefStmt.c_str(), N, vals, l, f, 0);
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw SbeException(SbeException::DBS_STMTEXEC, {{"dbms","PgTable::loadUintByStmt()"}});

	if (PQntuples(res) == 1) {
		ptr = PQgetvalue(res, 0, 0);
		val = atol(ptr);

		retval = true;
	};

	PQclear(res);

	return retval;
};

bool PgTable::loadStringByStmt(
			const string& srefStmt
			, const unsigned int N
			, const char** vals
			, const int* l
			, const int* f
			, string& val
		) {
	PGresult* res; char* ptr;

	bool retval = false;
	
	res = PQexecPrepared(dbs, srefStmt.c_str(), N, vals, l, f, 0);
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw SbeException(SbeException::DBS_STMTEXEC, {{"dbms","PgTable::loadStringByStmt()"}});

	if (PQntuples(res) == 1) {
		ptr = PQgetvalue(res, 0, 0);
		val.assign(ptr, PQgetlength(res, 0, 0));

		retval = true;
	};

	PQclear(res);

	return retval;
};

bool PgTable::loadRefByStmt(
			const string& srefStmt
			, const unsigned int N
			, const char** vals
			, const int* l
			, const int* f
			, ubigint& ref
		) {
	PGresult* res; char* ptr;

	bool retval = false;
	
	res = PQexecPrepared(dbs, srefStmt.c_str(), N, vals, l, f, 0);
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw SbeException(SbeException::DBS_STMTEXEC, {{"dbms","PgTable::loadRefByStmt()"}});

	if (PQntuples(res) == 1) {
		ptr = PQgetvalue(res, 0, 0);
		ref = atoll(ptr);

		retval = true;
	};

	PQclear(res);

	return retval;
};

ubigint PgTable::loadRefsByStmt(
			const string& srefStmt
			, const unsigned int N
			, const char** vals
			, const int* l
			, const int* f
			, const bool append
			, vector<ubigint>& refs
		) {
	PGresult* res;

	ubigint numrow; ubigint numread = 0; char* ptr;
	ubigint ref;

	res = PQexecPrepared(dbs, srefStmt.c_str(), N, vals, l, f, 0);
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw SbeException(SbeException::DBS_STMTEXEC, {{"dbms","PgTable::loadRefsByStmt()"}});

	if (!append) refs.resize(0);

	numrow = PQntuples(res);

	if (numrow > 0) {
		refs.reserve(refs.size() + numrow);

		while (numread < numrow) {
			ptr = PQgetvalue(res, numread, 0);
			ref = atoll(ptr);

			refs.push_back(ref);

			numread++;
		};
	};

	PQclear(res);

	return numread;
};

ubigint PgTable::htonl64(
			const ubigint& inval
		) {
	ubigint retval;

	char* bp = (char*) &(inval);
	uint u1, u2;
	memcpy((void*) &u1, (void*) bp, 4);
	memcpy((void*) &u2, (void*) &(bp[4]), 4);
	u1 = htonl(u1);
	u2 = htonl(u2);
	bp = (char*) &retval;
	if (u1 == htonl(u1)) {
		memcpy((void*) bp, (void*) &u1, 4);
		memcpy((void*) &(bp[4]), (void*) &u2, 4);
	} else {
		memcpy((void*) bp, (void*) &u2, 4);
		memcpy((void*) &(bp[4]), (void*) &u1, 4);
	};

	return retval;
};

ubigint PgTable::ntohl64(
			const ubigint& inval
		) {
	ubigint retval;

	char* bp = (char*) &(inval);
	uint u1, u2;
	memcpy((void*) &u1, (void*) bp, 4);
	memcpy((void*) &u2, (void*) &(bp[4]), 4);
	u1 = ntohl(u1);
	u2 = ntohl(u2);
	bp = (char*) &retval;
	if (u1 == htonl(u1)) {
		memcpy((void*) bp, (void*) &u1, 4);
		memcpy((void*) &(bp[4]), (void*) &u2, 4);
	} else {
		memcpy((void*) bp, (void*) &u2, 4);
		memcpy((void*) &(bp[4]), (void*) &u1, 4);
	};

	return retval;
};
