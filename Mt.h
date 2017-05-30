/**
  * \file Mt.h
  * methods for POSIX threading functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 17 Sep 2015
  * \date modified: 1 May 2017
  */

#ifndef SBECORE_MT_H
#define SBECORE_MT_H

#include <string.h>
#include <sys/time.h>

#ifdef _WIN32
	#include <pthread.h>
#endif

#include <iostream>
#include <string>

using namespace std;

/**
  * Cond
  */
namespace Cond {
	int init(pthread_cond_t* cond, const string& srefCond = "", const string& srefObject = "", const string& srefMember = "");
	int destroy(pthread_cond_t* cond, const string& srefCond = "", const string& srefObject = "", const string& srefMember = "");
	int signal(pthread_cond_t* cond, pthread_mutex_t* mutex, const string& srefCond = "", const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");
	int broadcast(pthread_cond_t* cond, pthread_mutex_t* mutex, const string& srefCond = "", const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");
	int wait(pthread_cond_t* cond, pthread_mutex_t* mutex, const string& srefCond = "", const string& srefObject = "", const string& srefMember = "");
	int timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const unsigned int dt, const string& srefCond = "", const string& srefObject = "", const string& srefMember = "");

	void dumpError(const int res, const string& err, const string& srefCond = "", const string& srefObject = "", const string& srefMember = "");
};

/**
  * Mutex
  */
namespace Mutex {
	int init(pthread_mutex_t* mutex, const bool recursive, const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");
	int destroy(pthread_mutex_t* mutex, const bool recursive, const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");
	int lock(pthread_mutex_t* mutex, const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");
	int trylock(pthread_mutex_t* mutex, const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");
	int unlock(pthread_mutex_t* mutex, const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");

	void dumpError(const int res, const string& err, const string& srefMutex = "", const string& srefObject = "", const string& srefMember = "");
};

#endif
