#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <Windows.h>
#include <future>
#include "CoreMacro.h"
#include "ThreadManager.h"
#include "RefCounting.h" 

using KnightRef = TSharedPtr<class Knight>;
using InventoryRef = TSharedPtr<class Inventory>;

class Knight : public RefCountable
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}
	
	~Knight()
	{
		cout << "~Knight()" << endl;
	}
	
	void SetTarget(KnightRef target)
	{
		_target = target;
	}


	// A Knight와 B Knight를 서로 주시함
	// 카운트가 순환이 되어 가지고 절대로 양쪽이 다 해제가 x
	KnightRef _target = nullptr;
	InventoryRef _inventory = nullptr;
};

// 서로 포인터가 엮어 있음
// 포인터가 절대로 해제가 되지 않음
class Inventory : public RefCountable
{
public:
	Inventory(KnightRef knight) : _knight(knight)
	{

	}

	KnightRef _knight;
};

int main()
{
	// 1) 이미 만들어진 클래스 대상으로 사용 불가.
	// 2) 순환(Cycle) 문제

	KnightRef k1(new Knight());
	k1->ReleaseRef();

	k1->_inventory = new Inventory(k1);

	// 생포인터를 사용하는 것(부하도 안걸림)
	// 데이터를 복사하는 것을 막아놓음.
	// unique_ptr(const unique_ptr&) = delete;
	// unique_ptr& operator=(const unique_ptr&) = delete;
	// 객체가 사라질 때 알아서 소멸자에서 삭제
	//unique_ptr<Knight> k2 = make_unique<Knight>();

	// 그냥 복사를 하는 것이 아니라 이동해서 소유권을 포기하고 
	// K3에 넘겨라
	//unique_ptr<Knight> k3 = std::move(k2);
	
	// 상대방에 대한 레퍼런스 카운트를 늘리는 방식으로 동작
	// 얘를 복사하는 순간 늘리기 때문에 항상 문제가 발생.
	// unique_ptr
	// shared_ptr
	// weak_ptr
	// Weak_ptr 포인터를 사용하면 순환 문제를 해결
	// [Knight][RefCountingBlock]

	// [T*][RefCountBlocking*]
	shared_ptr<Knight> spr(new Knight());
	// [T*][RefCountBlocking*]
	//KnightRef k2(new Knight());
	//k2->ReleaseRef();

	//k1->SetTarget(k2);
	// 서로 주시하고 있다보니까 레퍼런스가 0이 되지 않아 
	// 소멸자가 호출되지 않음. 
	// => 절대로 소멸되지 않아 서로 꼬여 해제가 x 
	//k2->SetTarget(k1);

	// 해결책
	// => 깨끗하게 서로 사이클이 일어나서 서로 참조하는 문제를 없애기 위해 많은 노력을 함.
	// 버그가 발생하는 것은 뭔가 컴포넌트 패턴에서 어떤 클래스가 다른 클래스를 
	// 포함하고 있는 관계에서 자주 발생한다.
	//k1->SetTarget(nullptr);
	//k2->SetTarget(nullptr);

	// 소멸자가 호출되는 시기
	// 첫 번째 나이트가 두 번째 애를 바라보고 있는데 
	// 첫번째 애를 nullptr로 밀어주면서 해제시켜줌.
	// 레퍼런스 카운팅을 줄이다 보면 k1을 기억하는 애가
	// 아무도 없다 보니까 소멸이 되기 시작함.
	// 자연스럽게 KnightRef _target = nullptr;이 들고 있던
	// 모든 맴버 변수들에 대한 소멸자도 같이 호출.
	// 자연스럽게 TSharedPtr 소멸자가 호출되면서 릴리즈함
	// 상대방에 대한 레퍼런스 카운트도 자연스럽게 포기
	//k1 = nullptr;
	//k2 = nullptr;
}