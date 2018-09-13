/**
  * \file Mt.cpp
  * methods for POSIX threading functionality (implementation)
  * \author Alexander Wirthmüller
  * \date created: 17 Sep 2015
  * \date modified: 22 Aug 2018
  */

#include "Mt.h"

/******************************************************************************
 class Cond
 ******************************************************************************/

Cond::Cond(
			const string& sref
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	res = pthread_mutex_init(&m, &attr);
	if (res != 0) Mt::logError(res, "error initializing mutex", sref + ".m", srefObject, srefMember);

	pthread_mutexattr_destroy(&attr);

	if (res == 0) {
		res = pthread_cond_init(&c, NULL);
		if (res != 0) Mt::logError(res, "error initializing condition", sref, srefObject, srefMember);
	};

	this->sref = sref;

	if (res == 0) Mt::logDebug("successfully initialized condition", sref, srefObject, srefMember);
};

Cond::~Cond() {
	int res;
	
	res = pthread_cond_destroy(&c);
	if (res != 0) Mt::logError(res, "error destroying condition", sref);

	if (res == 0) {
		while (true) {
			res = pthread_mutex_destroy(&m);

			if (res == 0) break;
			else if (res != EBUSY) {
				Mt::logError(res, "error destroying mutex", sref + ".m");
				break;
			};

			res = pthread_mutex_unlock(&m);
			if (res != 0) {
				Mt::logError(res, "error unlocking mutex", sref + ".m");
				break;
			};
		};
	};

	if (res == 0) Mt::logDebug("successfully destroyed condition", sref);
};

void Cond::lockMutex(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	Mt::logDebug("attempting to lock mutex", sref + ".m", srefObject, srefMember);

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);
	else Mt::logDebug("successfully locked mutex", sref + ".m", srefObject, srefMember);
};

void Cond::unlockMutex(
			const string& srefObject
			, const string& srefMember
		) {
	int res;
	
	res = pthread_mutex_unlock(&m);
	if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	else Mt::logDebug("successfully unlocked mutex", sref + ".m", srefObject, srefMember);
};

void Cond::signal(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	if (res == 0) {
		res = pthread_cond_signal(&c);
		if (res != 0) Mt::logError(res, "error signalling condition", sref, srefObject, srefMember);
		else Mt::logDebug("successfully signalled condition", sref, srefObject, srefMember);
	};

	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};
};

void Cond::broadcast(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	if (res == 0) {
		res = pthread_cond_broadcast(&c);
		if (res != 0) Mt::logError(res, "error broadcasting condition", sref, srefObject, srefMember);
		else Mt::logDebug("successfully broadcadt condition", sref, srefObject, srefMember);
	};

	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};
};

void Cond::wait(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	Mt::logDebug("waiting for condition", sref, srefObject, srefMember);

	res = pthread_cond_wait(&c, &m);
	if (res != 0) Mt::logError(res, "error waiting for condition", sref, srefObject, srefMember);
	else Mt::logDebug("done waiting for condition", sref, srefObject, srefMember);
};

bool Cond::timedwait(
			const unsigned int dt
			, const string& srefObject
			, const string& srefMember
		) {
	int res;

	timeval now;
	timespec then;

	if (dt == 0) {
		wait(srefObject, srefMember);
		return true;

	} else {
		gettimeofday(&now, NULL);
		then.tv_sec = now.tv_sec + (dt/1000000);
		if ((now.tv_usec + (dt%1000000)) < 1000000) {
			then.tv_nsec = 1000*(now.tv_usec + (dt%1000000));
		} else {
			then.tv_sec++;
			then.tv_nsec = 1000*(now.tv_usec + (dt%1000000) - 1000000);
		};

		Mt::logDebug("waiting for condition", sref, srefObject, srefMember);

		res = pthread_cond_timedwait(&c, &m, &then);
		if ((res != 0) && (res != ETIMEDOUT)) {
			Mt::logError(res, "error waiting for condition", sref, srefObject, srefMember);
			return false;
		} else if (res == ETIMEDOUT) {
			Mt::logDebug("timed out waiting for condition", sref, srefObject, srefMember);
			return false;
		} else {
			Mt::logDebug("done waiting for condition", sref, srefObject, srefMember);
			return true;
		};
	};
};

/******************************************************************************
 namespace Mt
 ******************************************************************************/

string Mt::getTid() {
#ifdef __APPLE__
	uint64_t tid;
	pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
	uint64_t tid = syscall(SYS_gettid);
#endif

	return("[tid " + to_string(tid) + "] ");
};

void Mt::logDebug(
			const string& what
			, const string& srefCondMutex
			, const string& srefObject
			, const string& srefMember
		) {
	// ex. [tid 1234] JobXyz::test()[1] successfully locked mutex mcNewdata
	if (ixVVerbose < VecVVerbose::ALL) return;

	string s = getTid();

	if (srefObject.length() > 0) s += srefObject;
	if ((srefObject.length() > 0) && (srefMember.length() > 0)) s += "::";
	if (srefMember.length() > 0) s += srefMember + "()";
	if ((srefObject.length() > 0) || (srefMember.length() > 0)) s += " ";
	s += what;
	if (srefCondMutex.length() > 0) s += " " + srefCondMutex;
	
	cout << s << endl;
};

void Mt::logError(
			const int res
			, const string& err
			, const string& srefCondMutex
			, const string& srefObject
			, const string& srefMember
		) {
	// ex. JobXyz::test() error waiting for condition cNewdata (18: not possible)
	if (ixVVerbose < VecVVerbose::ERROR) return;

	string s = getTid();

	if (srefObject.length() > 0) s += srefObject;
	if ((srefObject.length() > 0) && (srefMember.length() > 0)) s += "::";
	if (srefMember.length() > 0) s += srefMember + "()";
	if ((srefObject.length() > 0) || (srefMember.length() > 0)) s += " ";
	s += err;
	if (srefCondMutex.length() > 0) s += " " + srefCondMutex;
	s += " (" + to_string(res) + ": " + string(strerror(res)) + ")";

	cout << s << endl;
};

uint Mt::ixVVerbose = Mt::VecVVerbose::OFF;

/******************************************************************************
 class Mutex
 ******************************************************************************/

Mutex::Mutex(
			const string& sref
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	res = pthread_mutex_init(&m, &attr);
	if (res != 0) Mt::logError(res, "error initializing mutex", sref, srefObject, srefMember);

	pthread_mutexattr_destroy(&attr);

	this->sref = sref;

	if (res == 0) Mt::logDebug("successfully initialized mutex", sref, srefObject, srefMember);
};

Mutex::~Mutex() {
	int res;

	while (true) {
		res = pthread_mutex_destroy(&m);

		if (res == 0) break;
		else if (res != EBUSY) {
			Mt::logError(res, "error destroying mutex", sref);
			break;
		};

		res = pthread_mutex_unlock(&m);
		if (res != 0) {
			Mt::logError(res, "error unlocking mutex", sref);
			break;
		};
	};

	if (res == 0) Mt::logDebug("successfully destroyed mutex", sref);
};

void Mutex::lock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	Mt::logDebug("attempting to lock mutex", sref, srefObject, srefMember);

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref, srefObject, srefMember);
	else Mt::logDebug("successfully locked mutex", sref, srefObject, srefMember);
};

bool Mutex::trylock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	Mt::logDebug("trying to lock mutex", sref, srefObject, srefMember);

	res = pthread_mutex_trylock(&m);
	if ((res != EBUSY) && (res != 0)) {
		Mt::logError(res, "error try-locking mutex", sref, srefObject, srefMember);
		return false;
	} else if (res == EBUSY) {
		Mt::logDebug("failed try-locking busy mutex", sref, srefObject, srefMember);
		return false;
	} else {
		Mt::logDebug("successfully try-locked mutex", sref, srefObject, srefMember);
		return true;
	};
};

void Mutex::unlock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;
	
	res = pthread_mutex_unlock(&m);
	if (res != 0) Mt::logError(res, "error unlocking mutex", sref, srefObject, srefMember);
	else Mt::logDebug("successfully unlocked mutex", sref, srefObject, srefMember);
};

/******************************************************************************
 class Rwmutex
 ******************************************************************************/

Rwmutex::Rwmutex(
			const string& sref
			, const string& srefObject
			, const string& srefMember
		) {
	int res;
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	res = pthread_mutex_init(&m, &attr);
	if (res != 0) Mt::logError(res, "error initializing mutex", sref + ".m", srefObject, srefMember);

	pthread_mutexattr_destroy(&attr);

	if (res == 0) {
		res = pthread_cond_init(&c, NULL);
		if (res != 0) Mt::logError(res, "error initializing condition", sref + ".c", srefObject, srefMember);
	};

	r = 0;

	tidW = 0;
	w = 0;

	this->sref = sref;

	if (res == 0) Mt::logDebug("successfully initialized r/w mutex", sref, srefObject, srefMember);
};

Rwmutex::~Rwmutex() {
	int res;
	
	res = pthread_cond_destroy(&c);
	if (res != 0) Mt::logError(res, "error destroying condition", sref + ".c");

	if (res == 0) {
		while (true) {
			res = pthread_mutex_destroy(&m);

			if (res == 0) break;
			else if (res != EBUSY) {
				Mt::logError(res, "error destroying mutex", sref + ".m");
				break;
			};

			res = pthread_mutex_unlock(&m);
			if (res != 0) {
				Mt::logError(res, "error unlocking mutex", sref + ".m");
				break;
			};
		};
	};

	if (res == 0) Mt::logDebug("successfully destroyed r/w mutex", sref);
};

void Rwmutex::rlock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	bool haswlock = false;

	Mt::logDebug("attempting to lock r/w mutex for read operation", sref, srefObject, srefMember);

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	if (res == 0) {
		if (w > 0) {
#ifdef __APPLE__
			uint64_t tid;
			pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
			uint64_t tid = syscall(SYS_gettid);
#endif

			haswlock = (tidW == tid);
		};

		if (!haswlock) {
			while (w > 0) res = pthread_cond_wait(&c, &m);
			if (res != 0) Mt::logError(res, "error waiting for condition", sref + ".c", srefObject, srefMember);
		};
	};

	if (!haswlock) r++;
	
	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};

	if (res == 0) Mt::logDebug("successfully locked r/w mutex for read operation", sref, srefObject, srefMember);
};

bool Rwmutex::rtrylock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	bool haswlock = false;

	Mt::logDebug("trying to lock r/w mutex for read operation", sref, srefObject, srefMember);

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	if (res == 0) {
		if (w > 0) {
#ifdef __APPLE__
			uint64_t tid;
			pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
			uint64_t tid = syscall(SYS_gettid);
#endif

			haswlock = (tidW == tid);

			if (!haswlock) {
				res = pthread_mutex_unlock(&m);
				if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
				else Mt::logDebug("failed try-locking busy r/w mutex for read operation", sref, srefObject, srefMember);
				
				return false;
			};
		};
	};

	if (!haswlock) r++;
	
	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};

	if (res == 0) Mt::logDebug("successfully try-locked r/w mutex for read operation", sref, srefObject, srefMember);
	
	return true;
};

void Rwmutex::runlock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	if (w == 0) {
		r--;

		if (res == 0) {
			if (r == 0) {
				res = pthread_cond_signal(&c);
				if (res != 0) Mt::logError(res, "error signalling condition", sref + ".c", srefObject, srefMember);
			};
		};
	};

	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};

	if (res == 0) Mt::logDebug("successfully unlocked r/w mutex from read operation", sref, srefObject, srefMember);
};

void Rwmutex::wlock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

#ifdef __APPLE__
	uint64_t tid;
	pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
	uint64_t tid = syscall(SYS_gettid);
#endif

	Mt::logDebug("attempting to lock r/w mutex for write operation", sref, srefObject, srefMember);

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	if (res == 0) {
		if (tidW != tid) {
			while ((w > 0) || (r > 0)) res = pthread_cond_wait(&c, &m);
			if (res != 0) Mt::logError(res, "error waiting for condition", sref + ".c", srefObject, srefMember);
		};
	};

	tidW = tid;
	w++;
	
	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};

	if (res == 0) Mt::logDebug("successfully locked r/w mutex for write operation", sref, srefObject, srefMember);
};

bool Rwmutex::wtrylock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

#ifdef __APPLE__
	uint64_t tid;
	pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
	uint64_t tid = syscall(SYS_gettid);
#endif

	Mt::logDebug("trying to lock r/w mutex for write operation", sref, srefObject, srefMember);

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	if (res == 0) {
		if ((tidW != tid) || (r > 0)) {
			res = pthread_mutex_unlock(&m);
			if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
			else Mt::logDebug("failed try-locking busy r/w mutex for write operation", sref, srefObject, srefMember);
			
			return false;
		};
	};

	tidW = tid;
	w++;

	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};

	if (res == 0) Mt::logDebug("successfully try-locked r/w mutex for write operation", sref, srefObject, srefMember);
	
	return true;
};

void Rwmutex::wunlock(
			const string& srefObject
			, const string& srefMember
		) {
	int res;

	res = pthread_mutex_lock(&m);
	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember);

	w--;
	if (w == 0) tidW = 0;

	if (res == 0) {
		if (w == 0) {
			res = pthread_cond_broadcast(&c);
			if (res != 0) Mt::logError(res, "error broadcasting condition", sref + ".c", srefObject, srefMember);
		};
	};

	if (res == 0) {
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember);
	};

	if (res == 0) Mt::logDebug("successfully unlocked r/w mutex from write operation", sref, srefObject, srefMember);
};
