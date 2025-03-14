#pragma once

// 표준 mutex 같은 경우에는 재귀적으로 락을 못 잡음

/*-------------
:  RW SpinLock
---------------*/

/*----------------
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W: WriteFlag (Exclusive Lock Owner ThreadId)
R: ReadFlag (Shared Lock Count)
------------------*/

// W -> W (O)
// W -> R (O)
// R -> W (X)
class Lock
{
	enum : uint32
	{
		// 최대로 기다려줄 틱
		ACQUIRE_TIMEOUT_TICK  = 10000,
		MAX_SPIN_COUNT = 5000,
		// 상위 16비트만을 뽑아오기
		WRITE_THREAD_MASK = 0xFFFF'0000,
		// 하위 16비트 뽑아오기
		READ_COUNT_MASK = 0X0000'FFFF,
		EMPTY_FLAG = 0X0000'0000
	};

public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/*----------------
:   LockGuards
-----------------*/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};
