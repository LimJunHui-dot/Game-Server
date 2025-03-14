#pragma once

// �𸮾� ���������� ���������� ����Ʈ �����͸� ������.

/* --------------------
: RefCountable
:
----------------------*/

// RefCounting.h�� ���� ī������ ���� ��ü�� ������ �����ϴ� ���

// �ֻ��� ��ü�� ���� ���۷��� ī���� ����� �־���.
// ���������� ����� Ŭ�������� �� ���� Ŭ������ ��ӹ޾� ����.
// �츮�� ������ �츮�� ���۷��� ī���� �����͸� ��������.(TSharedPtr)
// �� Ŭ������ ��ӹ޴� Ŭ������ ���� ī������ ���� ��ü�� ������ ������ �� �ִ�.

class RefCountable
{
public:
	// ������, �ʱ� ���� ī��Ʈ�� 1�� ����
	RefCountable() : _refCount(1){}
	// �Ҹ���, ���� ī��Ʈ�� 0�� �Ǹ� ��ü�� ����
	virtual ~RefCountable(){}

	// ���� ���� ī��Ʈ�� ��ȯ�Ѵ�.
	int GetRefCount() { return _refCount; }

	// AddRef�� �������� �����ϰ� ����.
	int AddRef() { return ++_refCount; }
	int ReleaseRef()
	{
		int32 refCount = --_refCount;
		if (refCount == 0)
		{
			delete this;
		}

		return refCount;
	}
protected:
	// ��ü�� ���� ī��Ʈ�� �����ϴ� ����
	atomic<int32>  _refCount;
};

/* --------------------
: SharedPtr
:
----------------------*/

// ���� ReleaseRef�� AddRef�� �� ���� ����
// SharedPtr�̶�� ��ü�� ���� ������ ��� ���ٰ�
// ī������ �˾Ƽ� ���ִ� ���� �ٽ� ����Ʈ
template<typename T>
class TSharedPtr
{
public:
	TSharedPtr(){}
	// �����ϴ� ��ü�� ����Ű�� ������
	TSharedPtr(T* ptr) { Set(ptr); }

	// ����
	TSharedPtr(const TSharedPtr& rhs) { Set(rhs._ptr); }
	// �̵�
	TSharedPtr(TSharedPtr&& rhs) { _ptr = rhs._ptr; rhs._ptr = nullptr; }
	// ��� ���� ����
	template<typename U>
	TSharedPtr(const TSharedPtr<U>& rhs) { Set(static_cast<T>(rhs._ptr)); }
	
	// �Ҹ���, �����ϴ� ��ü�� ���� ī��Ʈ�� ����
	~TSharedPtr() { Release(); }

public:
	// ���� ������
	TSharedPtr& operator=(const TSharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}

		return *this;
	}

	// �̵� ������
	TSharedPtr& operator=(TSharedPtr&& rhs)
	{
		Release();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}

	bool               operator==(const TSharedPtr& rhs) const { return _ptr == rhs._ptr; }
	bool               operator==(T* ptr) const { return _ptr == ptr; }
	bool               operator!=(const TSharedPtr& rhs) const { return _ptr != rhs._ptr; }
	bool               operator!=(T* ptr) const { return _ptr != ptr; }
	bool               operator<(const TSharedPtr& rhs) const { return _ptr < rhs._ptr; }
	T*                 operator*() { return _ptr; }
	const T*           operator*() const { return _ptr; }
	operator T*        () const { return _ptr; }
	T*                 operator->() { return _ptr; }
	const T*           operator->() const { return _ptr; }

	bool IsNull() { return _ptr == nullptr; }

private:
	// �����͸� �����ϰ�, ���� ī��Ʈ�� ����
	inline void Set(T* ptr)
	{
		_ptr = ptr;
		if (ptr)
			ptr->AddRef();
	}

	inline void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}

private:
	T* _ptr = nullptr;

};