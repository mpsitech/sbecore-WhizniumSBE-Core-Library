/**
	* \file PgDbs.h
	* database access code globals for PostgreSQL (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 1 Jan 2009
	*/

#ifndef SBECORE_PGDBS_H
#define SBECORE_PGDBS_H

#ifdef __linux__
	#include <netinet/in.h>
#endif

#include <libpq-fe.h>

#include <sbecore/Dbs.h>

namespace Sbecore {
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

	public:
		PGconn* dbs;

	public:
		void createStatement(const std::string& stmtName, const std::string& query, const int nParams);

	protected:
		void begin();
		bool commit();
		void rollback();

		bool loadUintByStmt(const std::string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, uint& val);
		bool loadStringByStmt(const std::string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, std::string& val);

		bool loadRefByStmt(const std::string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, ubigint& ref);
		ubigint loadRefsByStmt(const std::string& srefStmt, const unsigned int N, const char** vals, const int* l, const int* f, const bool append, std::vector<ubigint>& refs);

	public:
		ubigint htonl64(const ubigint& inval);
		ubigint ntohl64(const ubigint& inval);
	};
};
#endif
