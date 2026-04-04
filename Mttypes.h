/**
	* \file Mttypes.h
	* POSIX threading types and data types with thread safety (declarations)
	* \copyright (C) 2018-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 28 Dec 2018
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

namespace Sbecore {
	/**
		* Cond
		*/
	class Cond {

	public:
		Cond(const std::string& sref = "", const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "", const bool nomon = false);
		~Cond();

	private:
	#ifdef POSIXNOTCPP11
		pthread_mutex_t m;
		pthread_cond_t c;
	#else
		std::recursive_mutex m;
		std::condition_variable_any c;
	#endif

		std::string sref;
		bool nomon;

	public:
		void lockMutex(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		void unlockMutex(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");

		void signal(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		void broadcast(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");

		void wait(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		bool timedwait(const unsigned int dt, const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
	};

	/**
		* MtMon
		*/
	namespace MtMon {
		/**
			* VecVLevel
			*/
		namespace VecVLevel {
			const uint VOID = 1;
			const uint ERROR = 2;
			const uint ALL = 3;
		};

		extern pthread_mutex_t mAccess;
		extern uint ixVLevel;
		extern std::fstream monfile;
		extern double t0;

		bool isRunning();

		double getDt();

		void start(const std::string& Version, const uint ixVLevel, const std::string& monpath, const double t0 = 0.0);
		void stop(const bool skiplock = false);

		std::string getTid();

		void debug(const std::string& what, const std::string& srefCondMutex = "", const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		void error(const int res, const std::string& err, const std::string& srefCondMutex = "", const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
	};

	/**
		* Mutex
		*/
	class Mutex {

	public:
		Mutex(const std::string& sref = "", const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "", const bool nomon = false);
		~Mutex();

	private:
	#ifdef POSIXNOTCPP11
		pthread_mutex_t m;
	#else
		std::recursive_mutex m;
	#endif

		std::string sref;
		bool nomon;

	public:
		void lock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		bool trylock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		void unlock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
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
		Rwmutex(const std::string& sref = "", const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "", const bool nomon = false);
		~Rwmutex();

	private:
	#ifdef POSIXNOTCPP11
		pthread_mutex_t m;
		pthread_cond_t c;
	#else
		std::recursive_mutex m;
		std::condition_variable_any c;
	#endif

		unsigned int r;

	#ifdef POSIXNOTCPP11
		uint64_t tidW;
	#else
		std::thread::id tidW;
	#endif
		unsigned int w;

		std::string sref;
		bool nomon;

	public:
		void rlock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		bool rtrylock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		void runlock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");

		void wlock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		bool wtrylock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
		void wunlock(const std::string& srefObject = "", const std::string& srefMember = "", const std::string& args = "");
	};

	/**
		* Refseq
		*/
	class Refseq {

	public:
		Refseq(const std::string& sref = "");

	public:
		std::string sref;

		Mutex mAccess;
		ubigint ref;

	public:
		ubigint getNewRef();
	};

	/**
		* Scr
		*/
	namespace Scr {
		extern Rwmutex rwm;
		extern std::map<ubigint,std::string> scr;
		extern std::map<std::string,ubigint> descr;

		std::string scramble(const ubigint ref);
		ubigint descramble(const std::string& scrRef);
		std::string random();
	};
};
#endif
