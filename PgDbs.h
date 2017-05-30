/**
  * \file PgDbs.h
  * database access code globals for PostgreSQL (declarations)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 4 Nov 2016
  */

#ifndef SBECORE_PGDBS_H
#define SBECORE_PGDBS_H

#include <libpq-fe.h>

#include <sbecore/Dbs.h>

/**
  * PgTable
  */
class PgTable {

public:
	PgTable();
	virtual ~PgTable();

public:
	void init(PGconn* _dbs);
	virtual void initStatements();
	void initStatementsErr(PGresult* res);

public:
	PGconn* dbs;

protected:
	void begin();
	bool commit();
	void rollback();

	bool loadUbigintByStmt(const string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, ubigint& val); // LEGACY
	bool loadUintByStmt(const string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, uint& val);
	bool loadStringByStmt(const string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, string& val);

	bool loadRefByStmt(const string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, ubigint& ref);
	ubigint loadRefsByStmt(const string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, const bool append, vector<ubigint>& refs);

public:
	ubigint htonl64(const ubigint& inval);
	ubigint ntohl64(const ubigint& inval);
};
#endif
