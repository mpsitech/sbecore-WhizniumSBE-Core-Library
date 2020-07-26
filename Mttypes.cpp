/**
  * \file Mttypes.cpp
  * POSIX threading types and data types with thread safety (implementation)
  * \author Alexander Wirthmüller
  * \date created: 28 Dec 2018
  * \date modified: 9 Jul 2020
  */

#include "Mttypes.h"

using namespace std;

/******************************************************************************
 class Cond
 ******************************************************************************/

Sbecore::Cond::Cond(
			const string& sref
			, const string& srefObject
			, const string& srefMember
			, const string& args
		) {
#ifdef POSIXNOTCPP11
	int res;
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	res = pthread_mutex_init(&m, &attr);
	if (res != 0) Mt::logError(res, "error initializing mutex", sref + ".m", srefObject, srefMember, args);

	pthread_mutexattr_destroy(&attr);

	if (res == 0) {
		res = pthread_cond_init(&c, NULL);
		if (res != 0) Mt::logError(res, "error initializing condition", sref, srefObject, srefMember, args);
	};

	this->sref = sref;

	if (res == 0) Mt::logDebug("successfully initialized condition", sref, srefObject, srefMember, args);
#else
	Mt::logDebug("successfully initialized condition", sref, srefObject, srefMember, args);
#endif
};

Sbecore::Cond::~Cond() {
#ifdef POSIXNOTCPP11
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
#else
	Mt::logDebug("successfully destroyed condition", sref);
#endif
};

void Sbecore::Cond::lockMutex(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

	Mt::logDebug("attempting to lock mutex", sref + ".m", srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);
	else Mt::logDebug("successfully locked mutex", sref + ".m", srefObject, srefMember, args);
};

void Sbecore::Cond::unlockMutex(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
#ifdef POSIXNOTCPP11
	int res;

	res = pthread_mutex_unlock(&m);

	if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
	else Mt::logDebug("successfully unlocked mutex", sref + ".m", srefObject, srefMember, args);
#else
	m.unlock();
	Mt::logDebug("successfully unlocked mutex", sref + ".m", srefObject, srefMember, args);
#endif
};

void Sbecore::Cond::signal(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_cond_signal(&c);

		if (res != 0) Mt::logError(res, "error signalling condition", sref, srefObject, srefMember, args);
		else Mt::logDebug("successfully signalled condition", sref, srefObject, srefMember, args);
#else
		c.notify_one();
		Mt::logDebug("successfully signalled condition", sref, srefObject, srefMember, args);
#endif
	};

	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};
};

void Sbecore::Cond::broadcast(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_cond_broadcast(&c);

		if (res != 0) Mt::logError(res, "error broadcasting condition", sref, srefObject, srefMember, args);
		else Mt::logDebug("successfully broadcast condition", sref, srefObject, srefMember, args);
#else
		c.notify_all();
		Mt::logDebug("successfully broadcast condition", sref, srefObject, srefMember, args);
#endif
	};

	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};
};

void Sbecore::Cond::wait(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

	Mt::logDebug("waiting for condition", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_cond_wait(&c, &m);
#else
	try {
		c.wait(m);
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error waiting for condition", sref, srefObject, srefMember, args);
	else Mt::logDebug("done waiting for condition", sref, srefObject, srefMember, args);
};

bool Sbecore::Cond::timedwait(
			const unsigned int dt
			, const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

#ifdef POSIXNOTCPP11
	timeval now;
	timespec then;
#endif

	if (dt == 0) {
		wait(srefObject, srefMember, args);
		return true;

	} else {
#ifdef POSIXNOTCPP11
		gettimeofday(&now, NULL);
		then.tv_sec = now.tv_sec + (dt/1000000);
		if ((now.tv_usec + (dt%1000000)) < 1000000) {
			then.tv_nsec = 1000*(now.tv_usec + (dt%1000000));
		} else {
			then.tv_sec++;
			then.tv_nsec = 1000*(now.tv_usec + (dt%1000000) - 1000000);
		};
#endif

		Mt::logDebug("waiting for condition", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
		res = pthread_cond_timedwait(&c, &m, &then);
		if ((res != 0) && (res != ETIMEDOUT)) {
			Mt::logError(res, "error waiting for condition", sref, srefObject, srefMember, args);
			return false;
		} else if (res == ETIMEDOUT) {
			Mt::logDebug("timed out waiting for condition", sref, srefObject, srefMember, args);
			return false;
		} else {
			Mt::logDebug("done waiting for condition", sref, srefObject, srefMember, args);
			return true;
		};
#else
		try {
			auto res = c.wait_for(m, chrono::microseconds(dt));

			if (res == cv_status::timeout) {
				Mt::logDebug("timed out waiting for condition", sref, srefObject, srefMember, args);
				return false;
			} else {
				Mt::logDebug("done waiting for condition", sref, srefObject, srefMember, args);
				return true;
			};

		} catch (system_error e) {
			res = -1;
			Mt::logError(res, "error waiting for condition", sref, srefObject, srefMember, args);
			return false;
		};
#endif
	};
};

/******************************************************************************
 namespace Mt
 ******************************************************************************/

string Sbecore::Mt::getTid(
			const bool textNotBare
		) {
#ifdef POSIXNOTCPP11
#ifdef __APPLE__
	uint64_t tid;
	pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
	uint64_t tid = syscall(SYS_gettid);
#endif
#ifdef __CYGWIN__
	uint64_t tid = (uint64_t) pthread_self();
#endif

	if (textNotBare) return("[tid " + to_string(tid) + "] ");
	else return to_string(tid);
#else
	ostringstream str;
	
	str << this_thread::get_id();

	if (textNotBare) return("[tid " + str.str() + "] ");
	else return str.str();
#endif
};

void Sbecore::Mt::logDebug(
			const string& what
			, const string& srefCondMutex
			, const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	// ex. [tid 1234] JobXyz::test()[1] successfully locked mutex mcNewdata
	if (ixVVerbose < VecVVerbose::ALL) return;

	string s = getTid(true);

	if (srefObject.length() > 0) s += srefObject;
	if ((srefObject.length() > 0) && (srefMember.length() > 0)) s += "::";
	if (srefMember.length() > 0) s += srefMember + "(" + args + ")";
	if ((srefObject.length() > 0) || (srefMember.length() > 0)) s += " ";
	s += what;
	if (srefCondMutex.length() > 0) s += " " + srefCondMutex;
	
	cout << s << endl;
};

void Sbecore::Mt::logError(
			const int res
			, const string& err
			, const string& srefCondMutex
			, const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	// ex. JobXyz::test() error waiting for condition cNewdata (18: not possible)
	if (ixVVerbose < VecVVerbose::ERROR) return;

	string s = getTid(true);

	if (srefObject.length() > 0) s += srefObject;
	if ((srefObject.length() > 0) && (srefMember.length() > 0)) s += "::";
	if (srefMember.length() > 0) s += srefMember + "(" + args + ")";
	if ((srefObject.length() > 0) || (srefMember.length() > 0)) s += " ";
	s += err;
	if (srefCondMutex.length() > 0) s += " " + srefCondMutex;
#ifdef POSIXNOTCPP11
	s += " (" + to_string(res) + ": " + string(strerror(res)) + ")";
#endif

	cout << s << endl;
};

Sbecore::uint Sbecore::Mt::ixVVerbose = Sbecore::Mt::VecVVerbose::OFF;

/******************************************************************************
 class Mutex
 ******************************************************************************/

Sbecore::Mutex::Mutex(
			const string& sref
			, const string& srefObject
			, const string& srefMember
			, const string& args
		) {
#ifdef POSIXNOTCPP11
	int res;
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	res = pthread_mutex_init(&m, &attr);
	if (res != 0) Mt::logError(res, "error initializing mutex", sref, srefObject, srefMember, args);

	pthread_mutexattr_destroy(&attr);

	this->sref = sref;

	if (res == 0) Mt::logDebug("successfully initialized mutex", sref, srefObject, srefMember, args);
#else
	Mt::logDebug("successfully initialized mutex", sref, srefObject, srefMember, args);
#endif
};

Sbecore::Mutex::~Mutex() {
#ifdef POSIXNOTCPP11
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
#else
	Mt::logDebug("successfully destroyed mutex", sref);
#endif
};

void Sbecore::Mutex::lock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

	Mt::logDebug("attempting to lock mutex", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref, srefObject, srefMember, args);
	else Mt::logDebug("successfully locked mutex", sref, srefObject, srefMember, args);
};

bool Sbecore::Mutex::trylock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

	Mt::logDebug("trying to lock mutex", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_mutex_trylock(&m);
#else
	res = (m.try_lock()) ? 0 : EBUSY;
#endif

	if ((res != EBUSY) && (res != 0)) {
		Mt::logError(res, "error try-locking mutex", sref, srefObject, srefMember, args);
		return false;
	} else if (res == EBUSY) {
		Mt::logDebug("failed try-locking busy mutex", sref, srefObject, srefMember, args);
		return false;
	} else {
		Mt::logDebug("successfully try-locked mutex", sref, srefObject, srefMember, args);
		return true;
	};
};

void Sbecore::Mutex::unlock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
#ifdef POSIXNOTCPP11
	int res;
	
	res = pthread_mutex_unlock(&m);

	if (res != 0) Mt::logError(res, "error unlocking mutex", sref, srefObject, srefMember, args);
	else Mt::logDebug("successfully unlocked mutex", sref, srefObject, srefMember, args);
#else
	m.unlock();
	Mt::logDebug("successfully unlocked mutex", sref, srefObject, srefMember, args);
#endif
};

/******************************************************************************
 class Rwmutex
 ******************************************************************************/

Sbecore::Rwmutex::Rwmutex(
			const string& sref
			, const string& srefObject
			, const string& srefMember
			, const string& args
		) {
#ifdef POSIXNOTCPP11
	int res;
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	res = pthread_mutex_init(&m, &attr);
	if (res != 0) Mt::logError(res, "error initializing mutex", sref + ".m", srefObject, srefMember, args);

	pthread_mutexattr_destroy(&attr);

	if (res == 0) {
		res = pthread_cond_init(&c, NULL);
		if (res != 0) Mt::logError(res, "error initializing condition", sref + ".c", srefObject, srefMember, args);
	};
#endif

	r = 0;

#ifdef POSIXNOTCPP11
	tidW = 0;
#endif
	w = 0;

	this->sref = sref;

#ifdef POSIXNOTCPP11
	if (res == 0) Mt::logDebug("successfully initialized r/w mutex", sref, srefObject, srefMember, args);
#else
	Mt::logDebug("successfully initialized r/w mutex", sref, srefObject, srefMember, args);
#endif
};

Sbecore::Rwmutex::~Rwmutex() {
#ifdef POSIXNOTCPP11
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
#else
	Mt::logDebug("successfully destroyed r/w mutex", sref);
#endif
};

void Sbecore::Rwmutex::rlock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

	bool haswlock = false;

	Mt::logDebug("attempting to lock r/w mutex for read operation", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	if (res == 0) {
		if (w > 0) {
#ifdef POSIXNOTCPP11
#ifdef __APPLE__
			uint64_t tid;
			pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
			uint64_t tid = syscall(SYS_gettid);
#endif
#ifdef __CYGWIN__
			uint64_t tid = (uint64_t) pthread_self();
#endif

			haswlock = (tidW == tid);
#else
			haswlock = (tidW == this_thread::get_id());
#endif
		};

		if (!haswlock) {
			while (w > 0) {
#ifdef POSIXNOTCPP11
				res = pthread_cond_wait(&c, &m);
#else
				try {
					c.wait(m);
					res = 0;
				} catch (system_error e) {
					res = -1;
				};
#endif
			};

			if (res != 0) Mt::logError(res, "error waiting for condition", sref + ".c", srefObject, srefMember, args);
		};
	};

	if (!haswlock) r++;
	
	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};

	if (res == 0) Mt::logDebug("successfully locked r/w mutex for read operation", sref, srefObject, srefMember, args);
};

bool Sbecore::Rwmutex::rtrylock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

	bool haswlock = false;

	Mt::logDebug("trying to lock r/w mutex for read operation", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	if (res == 0) {
		if (w > 0) {
#ifdef POSIXNOTCPP11
#ifdef __APPLE__
			uint64_t tid;
			pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
			uint64_t tid = syscall(SYS_gettid);
#endif
#ifdef __CYGWIN__
			uint64_t tid = (uint64_t) pthread_self();
#endif

			haswlock = (tidW == tid);
#else
			haswlock = (tidW == this_thread::get_id());
#endif

			if (!haswlock) {
#ifdef POSIXNOTCPP11
				res = pthread_mutex_unlock(&m);

				if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
				else Mt::logDebug("failed try-locking busy r/w mutex for read operation", sref, srefObject, srefMember, args);
#else
				m.unlock();
				Mt::logDebug("failed try-locking busy r/w mutex for read operation", sref, srefObject, srefMember, args);
#endif

				return false;
			};
		};
	};

	if (!haswlock) r++;
	
	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};

	if (res == 0) Mt::logDebug("successfully try-locked r/w mutex for read operation", sref, srefObject, srefMember, args);
	
	return true;
};

void Sbecore::Rwmutex::runlock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	if (w == 0) {
		r--;

		if (res == 0) {
			if (r == 0) {
#ifdef POSIXNOTCPP11
				res = pthread_cond_signal(&c);
				if (res != 0) Mt::logError(res, "error signalling condition", sref + ".c", srefObject, srefMember, args);
#else
				c.notify_one();
#endif
			};
		};
	};

	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};

	if (res == 0) Mt::logDebug("successfully unlocked r/w mutex from read operation", sref, srefObject, srefMember, args);
};

void Sbecore::Rwmutex::wlock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

#ifdef POSIXNOTCPP11
#ifdef __APPLE__
	uint64_t tid;
	pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
	uint64_t tid = syscall(SYS_gettid);
#endif
#ifdef __CYGWIN__
	uint64_t tid = (uint64_t) pthread_self();
#endif
#else
	thread::id tid = this_thread::get_id();
#endif

	Mt::logDebug("attempting to lock r/w mutex for write operation", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	if (res == 0) {
		if (tidW != tid) {

			while ((w > 0) || (r > 0)) {
#ifdef POSIXNOTCPP11
				res = pthread_cond_wait(&c, &m);
#else
				try {
					c.wait(m);
					res = 0;
				} catch (system_error e) {
					res = -1;
				};
#endif
			};

			if (res != 0) Mt::logError(res, "error waiting for condition", sref + ".c", srefObject, srefMember, args);
		};
	};

	tidW = tid;
	w++;
	
	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};

	if (res == 0) Mt::logDebug("successfully locked r/w mutex for write operation", sref, srefObject, srefMember, args);
};

bool Sbecore::Rwmutex::wtrylock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

#ifdef POSIXNOTCPP11
#ifdef __APPLE__
	uint64_t tid;
	pthread_threadid_np(NULL, &tid);
#endif
#ifdef __linux__
	uint64_t tid = syscall(SYS_gettid);
#endif
#ifdef __CYGWIN__
	uint64_t tid = (uint64_t) pthread_self();
#endif
#else
	thread::id tid = this_thread::get_id();
#endif

	Mt::logDebug("trying to lock r/w mutex for write operation", sref, srefObject, srefMember, args);

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	if (res == 0) {
		if ((tidW != tid) || (r > 0)) {
#ifdef POSIXNOTCPP11
			res = pthread_mutex_unlock(&m);

			if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
			else Mt::logDebug("failed try-locking busy r/w mutex for write operation", sref, srefObject, srefMember, args);
#else
			m.unlock();
#endif

			return false;
		};
	};

	tidW = tid;
	w++;

	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};

	if (res == 0) Mt::logDebug("successfully try-locked r/w mutex for write operation", sref, srefObject, srefMember, args);
	
	return true;
};

void Sbecore::Rwmutex::wunlock(
			const string& srefObject
			, const string& srefMember
			, const string& args
		) {
	int res;

#ifdef POSIXNOTCPP11
	res = pthread_mutex_lock(&m);
#else
	try {
		m.lock();
		res = 0;
	} catch (system_error e) {
 		res = -1;
	};
#endif

	if (res != 0) Mt::logError(res, "error locking mutex", sref + ".m", srefObject, srefMember, args);

	w--;
#ifdef POSIXNOTCPP11
	if (w == 0) tidW = 0;
#else
	if (w == 0) tidW = thread::id();
#endif

	if (res == 0) {
		if (w == 0) {
#ifdef POSIXNOTCPP11
			res = pthread_cond_broadcast(&c);
			if (res != 0) Mt::logError(res, "error broadcasting condition", sref + ".c", srefObject, srefMember, args);
#else
			c.notify_all();
#endif
		};
	};

	if (res == 0) {
#ifdef POSIXNOTCPP11
		res = pthread_mutex_unlock(&m);
		if (res != 0) Mt::logError(res, "error unlocking mutex", sref + ".m", srefObject, srefMember, args);
#else
		m.unlock();
#endif
	};

	if (res == 0) Mt::logDebug("successfully unlocked r/w mutex from write operation", sref, srefObject, srefMember, args);
};

/******************************************************************************
 class Refseq
 ******************************************************************************/

Sbecore::Refseq::Refseq(
			const string& sref
		) :
			mAccess("mAccess", "Refseq(" + sref + ")", "Refseq")
		{
	this->sref = sref;

	ref = 0;
};

Sbecore::ubigint Sbecore::Refseq::getNewRef() {
	ubigint ref_backup;

	mAccess.lock("Refseq(" + sref + ")", "getNewRef");

	ref++;
	ref_backup = ref;

	mAccess.unlock("Refseq(" + sref + ")", "getNewRef");

	return ref_backup;
};

/******************************************************************************
 namespace Scr
 ******************************************************************************/

string Sbecore::Scr::scramble(
			const ubigint ref
		) {
	string retval;

	if (ref == 0) return("");

	rwm.rlock("Scr", "scramble");

	auto it = scr.find(ref);

	if (it != scr.end()) {
		retval = it->second;

		rwm.runlock("Scr", "scramble[1]");

	} else {
		rwm.runlock("Scr", "scramble[2]");

		rwm.wlock("Scr", "scramble");

		while (true) {
			retval = random();
			if (descr.find(retval) == descr.end()) break;
		};

		scr[ref] = retval;
		descr[retval] = ref;

		rwm.wunlock("Scr", "scramble");
	};

	return retval;
};

Sbecore::ubigint Sbecore::Scr::descramble(
			const string& scrRef
		) {
	ubigint retval = 0;

	rwm.rlock("Scr", "descramble");

	auto it = descr.find(scrRef);
	if (it != descr.end()) retval = it->second;

	rwm.runlock("Scr", "descramble");

	return retval;
};

string Sbecore::Scr::random() {
	string retval;

	int digit;

	// assume advance random seed

	// fetch digits and make sure their ASCII code is in the range 0..9/a..z
	for (unsigned int i = 0; i<16; i++) {
		digit = rand() % 36 + 48;
		if (digit > 57) digit += (97-48-10);

		retval = retval + ((char) digit);
	};

	return retval;
};

Sbecore::Rwmutex Sbecore::Scr::rwm("rwm", "Scr", "Scr");
map<Sbecore::ubigint,string> Sbecore::Scr::scr;
map<string,Sbecore::ubigint> Sbecore::Scr::descr;
