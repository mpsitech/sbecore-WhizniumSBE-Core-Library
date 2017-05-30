/**
  * \file Mt.cpp
  * methods for POSIX threading functionality (implementation)
  * \author Alexander Wirthmüller
  * \date created: 17 Sep 2015
  * \date modified: 1 May 2017
  */

#include "Mt.h"

/******************************************************************************
 namespace Cond
 ******************************************************************************/

int Cond::init(
			pthread_cond_t* cond
			, const string& srefCond
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	
	res = pthread_cond_init(cond, NULL);
	if (res != 0) dumpError(res, "error initializing condition", srefCond, srefObject, srefMember);

	return res;
};

int Cond::destroy(
			pthread_cond_t* cond
			, const string& srefCond
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	
	res = pthread_cond_destroy(cond);
	if (res != 0) dumpError(res, "error destroying condition", srefCond, srefObject, srefMember);

	return res;
};

int Cond::signal(
			pthread_cond_t* cond
			, pthread_mutex_t* mutex
			, const string& srefCond
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = Mutex::lock(mutex, srefMutex, srefObject, srefMember);

	if (res == 0) {
		res = pthread_cond_signal(cond);
		if (res != 0) dumpError(res, "error signalling condition", srefCond, srefObject, srefMember);
	};

	if (res == 0) res = Mutex::unlock(mutex, srefMutex, srefObject, srefMember);

	return res;
};

int Cond::broadcast(
			pthread_cond_t* cond
			, pthread_mutex_t* mutex
			, const string& srefCond
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = Mutex::lock(mutex, srefMutex, srefObject, srefMember);

	if (res == 0) {
		res = pthread_cond_broadcast(cond);
		if (res != 0) dumpError(res, "error broadcasting condition", srefCond, srefObject, srefMember);
	};

	if (res == 0) res = Mutex::unlock(mutex, srefMutex, srefObject, srefMember);

	return res;
};

int Cond::wait(
			pthread_cond_t* cond
			, pthread_mutex_t* mutex
			, const string& srefCond
			, const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = pthread_cond_wait(cond, mutex);
	if (res != 0) dumpError(res, "error waiting for condition", srefCond, srefObject, srefMember);

	return res;
};

int Cond::timedwait(
			pthread_cond_t* cond
			, pthread_mutex_t* mutex
			, const unsigned int dt
			, const string& srefCond
			, const string& srefObject
			, const string& srefMember
		) {
	int res;

	timeval now;
	timespec then;

	if (dt == 0) res = wait(cond, mutex, srefCond, srefObject, srefMember);
	else {
		gettimeofday(&now, NULL);
		then.tv_sec = now.tv_sec + (dt/1000000);
		if ((now.tv_usec + (dt%1000000)) < 1000000) {
			then.tv_nsec = 1000*(now.tv_usec + (dt%1000000));
		} else {
			then.tv_sec++;
			then.tv_nsec = 1000*(now.tv_usec + (dt%1000000) - 1000000);
		};

		res = pthread_cond_timedwait(cond, mutex, &then);
		if ((res != 0) && (res != ETIMEDOUT)) dumpError(res, "error waiting for condition", srefCond, srefObject, srefMember);
	};

	return res;
};

void Cond::dumpError(
			const int res
			, const string& err
			, const string& srefCond
			, const string& srefObject
			, const string& srefMember
		) {
	if (srefObject.length() > 0) cout << srefObject;
	if ((srefObject.length() > 0) && (srefMember.length() > 0)) cout << "::";
	if (srefMember.length() > 0) cout << srefMember << "()";
	if ((srefObject.length() > 0) || (srefMember.length() > 0)) cout << " ";
	cout << err;
	if (srefCond.length() > 0) cout << " " << srefCond;
	cout << " (" << res << ": " << strerror(res) << ")" << endl;
};

/******************************************************************************
 namespace Mutex
 ******************************************************************************/

int Mutex::init(
			pthread_mutex_t* mutex
			, const bool recursive
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	pthread_mutexattr_t attr;

	if (recursive) {
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

		res = pthread_mutex_init(mutex, &attr);
		if (res != 0) dumpError(res, "error initializing mutex", srefMutex, srefObject, srefMember);

		pthread_mutexattr_destroy(&attr);

	} else {
		res = pthread_mutex_init(mutex, NULL);
		if (res != 0) dumpError(res, "error initializing mutex", srefMutex, srefObject, srefMember);
	};

	return res;
};

int Mutex::destroy(
			pthread_mutex_t* mutex
			, const bool recursive
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	int res;

	while (true) {
		res = pthread_mutex_destroy(mutex);
		if (res == 0) break;

		if ( (res != 0) && (!recursive || (recursive && (res != EBUSY))) ) {
			dumpError(res, "error destroying mutex", srefMutex, srefObject, srefMember);
			break;
		};

		if (unlock(mutex, srefMutex, srefObject, srefMember) != 0) break;
	};

	return res;
};

int Mutex::lock(
			pthread_mutex_t* mutex
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	
	res = pthread_mutex_lock(mutex);
	if (res != 0) dumpError(res, "error locking mutex", srefMutex, srefObject, srefMember);

	return res;
};

int Mutex::trylock(
			pthread_mutex_t* mutex
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = pthread_mutex_trylock(mutex);
	if ((res != EBUSY) && (res != 0)) dumpError(res, "error try-locking mutex", srefMutex, srefObject, srefMember);

	return res;
};

int Mutex::unlock(
			pthread_mutex_t* mutex
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	
	res = pthread_mutex_unlock(mutex);
	if (res != 0) dumpError(res, "error unlocking mutex", srefMutex, srefObject, srefMember);

	return res;
};

void Mutex::dumpError(
			const int res
			, const string& err
			, const string& srefMutex
			, const string& srefObject
			, const string& srefMember
		) {
	if (srefObject.length() > 0) cout << srefObject;
	if ((srefObject.length() > 0) && (srefMember.length() > 0)) cout << "::";
	if (srefMember.length() > 0) cout << srefMember << "()";
	if ((srefObject.length() > 0) || (srefMember.length() > 0)) cout << " ";
	cout << err;
	if (srefMutex.length() > 0) cout << " " << srefMutex;
	cout << " (" << res << ": " << strerror(res) << ")" << endl;
};
