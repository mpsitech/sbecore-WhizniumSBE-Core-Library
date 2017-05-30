/**
  * \file PgDbs.h
  * database access code globals for PostgreSQL (implementation)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 10 Aug 2014
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

void PgTable::initStatementsErr(
			PGresult* res
		) {
	throw DbsException("DbsException / PgSQL: unable to initialize statement\n" + string(PQresultErrorMessage(res)) + "\n");
};

void PgTable::begin() {
	PGresult* res;

	res = PQexec(dbs, "BEGIN");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) throw DbsException("DbsException / PgSQL: failed to begin transaction\n");

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
	if (PQresultStatus(res) != PGRES_COMMAND_OK) throw DbsException("DbsException / PgSQL: failed to roll back transaction\n");

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
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw DbsException("DbsException / PgSQL: error executing statement! (" + srefStmt + ")\n");

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
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw DbsException("DbsException / PgSQL: error executing statement! (" + srefStmt + ")\n");

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
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw DbsException("DbsException / PgSQL: error executing statement! (" + srefStmt + ")\n");

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
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw DbsException("DbsException / PgSQL: error executing statement! (" + srefStmt + ")\n");

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
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) throw DbsException("DbsException / PgSQL: error executing statement! (" + srefStmt + ")\n");

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
