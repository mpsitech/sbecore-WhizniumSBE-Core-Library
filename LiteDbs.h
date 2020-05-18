/**
  * \file LiteDbs.h
  * database access code globals for SQLite (declarations)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 22 Apr 2020
  */

#ifndef SBECORE_LITEDBS_H
#define SBECORE_LITEDBS_H

#include <sqlite3.h>

#include <sbecore/Dbs.h>

namespace Sbecore {
	/**
		* LiteTable
		*/
	class LiteTable {

	public:
		LiteTable();
		virtual ~LiteTable();

	public:
		void init(sqlite3* _dbs);
		virtual void initStatements();

	public:
		sqlite3* dbs;

	public:
		sqlite3_stmt* createStatement(const std::string stmtSQL);

	protected:
		void begin();
		bool commit();
		void rollback();

		bool loadUintByStmt(sqlite3_stmt* stmt, uint& val);
		bool loadStringByStmt(sqlite3_stmt* stmt, std::string& val);

		bool loadRefByStmt(sqlite3_stmt* stmt, ubigint& ref);
		ubigint loadRefsByStmt(sqlite3_stmt* stmt, const bool append, std::vector<ubigint>& refs);
	};
};
#endif
