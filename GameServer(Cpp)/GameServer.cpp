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


	// A Knight�� B Knight�� ���� �ֽ���
	// ī��Ʈ�� ��ȯ�� �Ǿ� ������ ����� ������ �� ������ x
	KnightRef _target = nullptr;
	InventoryRef _inventory = nullptr;
};

// ���� �����Ͱ� ���� ����
// �����Ͱ� ����� ������ ���� ����
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
	// 1) �̹� ������� Ŭ���� ������� ��� �Ұ�.
	// 2) ��ȯ(Cycle) ����

	KnightRef k1(new Knight());
	k1->ReleaseRef();

	k1->_inventory = new Inventory(k1);

	// �������͸� ����ϴ� ��(���ϵ� �Ȱɸ�)
	// �����͸� �����ϴ� ���� ���Ƴ���.
	// unique_ptr(const unique_ptr&) = delete;
	// unique_ptr& operator=(const unique_ptr&) = delete;
	// ��ü�� ����� �� �˾Ƽ� �Ҹ��ڿ��� ����
	//unique_ptr<Knight> k2 = make_unique<Knight>();

	// �׳� ���縦 �ϴ� ���� �ƴ϶� �̵��ؼ� �������� �����ϰ� 
	// K3�� �Ѱܶ�
	//unique_ptr<Knight> k3 = std::move(k2);
	
	// ���濡 ���� ���۷��� ī��Ʈ�� �ø��� ������� ����
	// �긦 �����ϴ� ���� �ø��� ������ �׻� ������ �߻�.
	// unique_ptr
	// shared_ptr
	// weak_ptr
	// Weak_ptr �����͸� ����ϸ� ��ȯ ������ �ذ�
	// [Knight][RefCountingBlock]

	// [T*][RefCountBlocking*]
	shared_ptr<Knight> spr(new Knight());
	// [T*][RefCountBlocking*]
	//KnightRef k2(new Knight());
	//k2->ReleaseRef();

	//k1->SetTarget(k2);
	// ���� �ֽ��ϰ� �ִٺ��ϱ� ���۷����� 0�� ���� �ʾ� 
	// �Ҹ��ڰ� ȣ����� ����. 
	// => ����� �Ҹ���� �ʾ� ���� ���� ������ x 
	//k2->SetTarget(k1);

	// �ذ�å
	// => �����ϰ� ���� ����Ŭ�� �Ͼ�� ���� �����ϴ� ������ ���ֱ� ���� ���� ����� ��.
	// ���װ� �߻��ϴ� ���� ���� ������Ʈ ���Ͽ��� � Ŭ������ �ٸ� Ŭ������ 
	// �����ϰ� �ִ� ���迡�� ���� �߻��Ѵ�.
	//k1->SetTarget(nullptr);
	//k2->SetTarget(nullptr);

	// �Ҹ��ڰ� ȣ��Ǵ� �ñ�
	// ù ��° ����Ʈ�� �� ��° �ָ� �ٶ󺸰� �ִµ� 
	// ù��° �ָ� nullptr�� �о��ָ鼭 ����������.
	// ���۷��� ī������ ���̴� ���� k1�� ����ϴ� �ְ�
	// �ƹ��� ���� ���ϱ� �Ҹ��� �Ǳ� ������.
	// �ڿ������� KnightRef _target = nullptr;�� ��� �ִ�
	// ��� �ɹ� �����鿡 ���� �Ҹ��ڵ� ���� ȣ��.
	// �ڿ������� TSharedPtr �Ҹ��ڰ� ȣ��Ǹ鼭 ��������
	// ���濡 ���� ���۷��� ī��Ʈ�� �ڿ������� ����
	//k1 = nullptr;
	//k2 = nullptr;
}