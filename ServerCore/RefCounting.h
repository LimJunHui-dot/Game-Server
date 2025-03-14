#pragma once

// 언리얼 엔진에서도 마찬가지로 스마트 포인터를 구현함.

/* --------------------
: RefCountable
:
----------------------*/

// RefCounting.h는 참조 카운팅을 통해 객체의 수명을 관리하는 기능

// 최상위 객체를 만들어서 레퍼런스 카운팅 기능을 넣어줌.
// 컨텐츠에서 사용할 클래스들을 이 상위 클래스를 상속받아 만듬.
// 우리가 무조건 우리의 레퍼런스 카운팅 포인터를 관리해줌.(TSharedPtr)
// 이 클래스를 상속받는 클래스는 참조 카운팅을 통해 객체의 수명을 관리할 수 있다.

class RefCountable
{
public:
	// 생성자, 초기 참조 카운트를 1로 설정
	RefCountable() : _refCount(1){}
	// 소멸자, 참조 카운트가 0이 되면 객체를 삭제
	virtual ~RefCountable(){}

	// 현재 참조 카운트를 반환한다.
	int GetRefCount() { return _refCount; }

	// AddRef를 수동으로 관리하고 있음.
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
	// 객체의 참조 카운트를 저장하는 변수
	atomic<int32>  _refCount;
};

/* --------------------
: SharedPtr
:
----------------------*/

// 직접 ReleaseRef와 AddRef를 할 일이 없고
// SharedPtr이라는 객체가 감춰 가지고 대신 해줄것
// 카운팅을 알아서 해주는 것이 핵심 포인트
template<typename T>
class TSharedPtr
{
public:
	TSharedPtr(){}
	// 관리하는 객체를 가리키는 포인터
	TSharedPtr(T* ptr) { Set(ptr); }

	// 복사
	TSharedPtr(const TSharedPtr& rhs) { Set(rhs._ptr); }
	// 이동
	TSharedPtr(TSharedPtr&& rhs) { _ptr = rhs._ptr; rhs._ptr = nullptr; }
	// 상속 관계 복사
	template<typename U>
	TSharedPtr(const TSharedPtr<U>& rhs) { Set(static_cast<T>(rhs._ptr)); }
	
	// 소멸자, 관리하는 객체의 참조 카운트를 감소
	~TSharedPtr() { Release(); }

public:
	// 복사 연산자
	TSharedPtr& operator=(const TSharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}

		return *this;
	}

	// 이동 연산자
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
	// 포인터를 설정하고, 참조 카운트를 증가
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