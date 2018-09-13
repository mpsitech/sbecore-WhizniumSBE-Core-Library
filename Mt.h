/**
  * \file Mt.h
  * methods for POSIX threading functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 17 Sep 2015
  * \date modified: 14 Aug 2018
  */

#ifndef SBECORE_MT_H
#define SBECORE_MT_H

#include <string.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef _WIN32
	#include <pthread.h>
#endif

#include <iostream>
#include <string>

using namespace std;

/**
  * Cond
  */
class Cond {

public:
	Cond(const string& sref = "", const string& srefObject = "", const string& srefMember = "");
	~Cond();

private:
	pthread_mutex_t m;
	pthread_cond_t c;

	string sref;

public:
	void lockMutex(const string& srefObject = "", const string& srefMember = "");
	void unlockMutex(const string& srefObject = "", const string& srefMember = "");

	void signal(const string& srefObject = "", const string& srefMember = "");
	void broadcast(const string& srefObject = "", const string& srefMember = "");

	void wait(const string& srefObject = "", const string& srefMember = "");
	bool timedwait(const unsigned int dt, const string& srefObject = "", const string& srefMember = "");
};

/**
  * Mt
  */
namespace Mt {
	/**
		* VecVVerbose
		*/
	namespace VecVVerbose {
		const uint OFF = 1;
		const uint ERROR = 2;
		const uint ALL = 3;
	};

	string getTid();

	void logDebug(const string& what, const string& srefCondMutex = "", const string& srefObject = "", const string& srefMember = "");
	void logError(const int res, const string& err, const string& srefCondMutex = "", const string& srefObject = "", const string& srefMember = "");

	extern uint ixVVerbose;
};

/**
  * Mutex
  */
class Mutex {

public:
	Mutex(const string& sref = "", const string& srefObject = "", const string& srefMember = "");
	~Mutex();

private:
	pthread_mutex_t m;

	string sref;

public:
	void lock(const string& srefObject = "", const string& srefMember = "");
	bool trylock(const string& srefObject = "", const string& srefMember = "");
	void unlock(const string& srefObject = "", const string& srefMember = "");
};

/**
  * Rwmutex
  */
class Rwmutex {
	// write-preferring r/w mutex ; still, writer has to wait for no readers present
	// both readers and writer can lock recursively
	// read lock/unlock requests of the writing thread are non-blocking and otherwise ignored
	// a read lock cannot be bumped up to become a write lock

public:
	Rwmutex(const string& sref = "", const string& srefObject = "", const string& srefMember = "");
	~Rwmutex();

private:
	pthread_mutex_t m;
	pthread_cond_t c;

	unsigned int r;

	uint64_t tidW;
	unsigned int w;

	string sref;

public:
	void rlock(const string& srefObject = "", const string& srefMember = "");
	bool rtrylock(const string& srefObject = "", const string& srefMember = "");
	void runlock(const string& srefObject = "", const string& srefMember = "");

	void wlock(const string& srefObject = "", const string& srefMember = "");
	bool wtrylock(const string& srefObject = "", const string& srefMember = "");
	void wunlock(const string& srefObject = "", const string& srefMember = "");
};

#endif
