/**
  * \file Mttypes.h
  * POSIX threading types and data types with thread safety (declarations)
  * \author Alexander Wirthmüller
  * \date created: 28 Dec 2018
  * \date modified: 28 Dec 2018
  */

#ifndef SBECORE_MTTYPES_H
#define SBECORE_MTTYPES_H

#define POSIXNOTCPP11

#ifdef __CYGWIN__
	#include <pthread.h>
#endif

#include <string.h>

#ifdef __linux__
	#include <sys/syscall.h>
	#include <unistd.h>
#endif
#ifndef _WIN32
	#include <sys/time.h>
#endif
#include <sys/types.h>

//#include <condition_variable>
#include <iostream>
//#include <mutex>
#include <string>
//#include <thread>

#include <sbecore/Types.h>

/**
  * Cond
  */
class Cond {

public:
	Cond(const string& sref = "", const string& srefObject = "", const string& srefMember = "");
	~Cond();

private:
#ifdef POSIXNOTCPP11
	pthread_mutex_t m;
	pthread_cond_t c;
#else
	recursive_mutex m;
 	condition_variable_any c;
#endif

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

	string getTid(const bool textNotBare = false);

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
#ifdef POSIXNOTCPP11
	pthread_mutex_t m;
#else
	recursive_mutex m;
#endif

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
#ifdef POSIXNOTCPP11
	pthread_mutex_t m;
	pthread_cond_t c;
#else
	recursive_mutex m;
 	condition_variable_any c;
#endif

	unsigned int r;

#ifdef POSIXNOTCPP11
	uint64_t tidW;
#else
	thread::id tidW;
#endif
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

/**
	* Refseq
	*/
class Refseq {

public:
	Refseq(const string& sref = "");

public:
	string sref;

	Mutex mAccess;
	ubigint ref;

public:
	ubigint getNewRef();
};

/**
  * Scr
  */
namespace Scr {
	string scramble(const ubigint ref);
	ubigint descramble(const string& scrRef);
	string random();

	extern Rwmutex rwm;
	extern map<ubigint,string> scr;
	extern map<string,ubigint> descr;
};

#endif
