/**
  * \file MyDbs.h
  * database access code globals for MySQL (declarations)
  * \author Alexander Wirthmüller
  * \date created: 1 Jan 2009
  * \date modified: 4 Nov 2016
  */

#ifndef SBECORE_MYDBS_H
#define SBECORE_MYDBS_H

#include <mysql.h>

#include <sbecore/Dbs.h>

/**
  * MyTable
  */
class MyTable {

public:
	MyTable();
	virtual ~MyTable();

public:
	void init(MYSQL* _dbs);
	virtual void initStatements();

public:
	MYSQL* dbs;

public:
	MYSQL_BIND bindTinyint(tinyint* t, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindUtinyint(utinyint* t, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindSmallint(smallint* s, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindUsmallint(usmallint* s, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindInt(int* i, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindUint(uint* i, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindBigint(bigint* b, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindUbigint(ubigint* b, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindCstring(char* str, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindDouble(double* d, unsigned long* length, my_bool* is_null, my_bool* error);
	MYSQL_BIND bindCstring0(unsigned long* length, my_bool* is_null, my_bool* error);

	MYSQL_STMT* createStatement(const string stmtSQL, my_bool getlength = false);

protected:
	void begin();
	bool commit();
	void rollback();

	bool loadUintBySQL(const string& sqlstr, uint& val);
	bool loadStringBySQL(const string& sqlstr, string& val);

	bool loadRefBySQL(const string& sqlstr, ubigint& ref);
	ubigint loadRefsBySQL(const string& sqlstr, const bool append, vector<ubigint>& refs);
};
#endif
