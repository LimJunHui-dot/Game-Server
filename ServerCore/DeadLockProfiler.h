#pragma once  
#include <stack>  
#include <map>  
#include <vector>  
#include <unordered_map> // �߰��� include
#include <set> // �߰��� include
#include <mutex> // �߰��� include

using namespace std; // �߰��� using ����
using int32 = int; // �߰��� typedef

/*--------------------  
   DeadLockProfiler  
---------------------*/  

class DeadLockProfiler  
{  
public:  

   void PushLock(const char* name);  
   void PopLock(const char* name);  
   void CheckCycle();  

private:  
   void Dfs(int32 index);  

private:  
   unordered_map<const char*, int32>    _nameToId;  
   unordered_map<int32, const char*>    _idToName;
   // ���� ����Ǵ� ���� �������� ����
   stack<int32>                        _lockStack;  
   map<int32, set<int32>>                _lockHistory;  
   
   mutex _lock; // ������ Ÿ��

private:  
	// cycle üũ�� �� ������ �Ź� �ʱ�ȭ
   vector<int32>    _discoveredOrder; // ��尡 �߰ߵ� ������ ����ϴ� �迭  
   int32            _discoveredCount = 0; // ��尡 �߰ߵ� ����  
   vector<bool>     _finished; // Dfs(i)�� ���� �Ǿ����� ����  
   vector<int32>    _parent;  
};
