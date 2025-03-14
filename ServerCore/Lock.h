#pragma once

// ǥ�� mutex ���� ��쿡�� ��������� ���� �� ����

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
		// �ִ�� ��ٷ��� ƽ
		ACQUIRE_TIMEOUT_TICK  = 10000,
		MAX_SPIN_COUNT = 5000,
		// ���� 16��Ʈ���� �̾ƿ���
		WRITE_THREAD_MASK = 0xFFFF'0000,
		// ���� 16��Ʈ �̾ƿ���
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
