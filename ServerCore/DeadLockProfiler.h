#pragma once  
#include <stack>  
#include <map>  
#include <vector>  
#include <unordered_map> // 추가된 include
#include <set> // 추가된 include
#include <mutex> // 추가된 include

using namespace std; // 추가된 using 선언
using int32 = int; // 추가된 typedef

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
   // 락이 실행되는 것을 스택으로 추적
   stack<int32>                        _lockStack;  
   map<int32, set<int32>>                _lockHistory;  
   
   mutex _lock; // 수정된 타입

private:  
	// cycle 체크를 할 때마다 매번 초기화
   vector<int32>    _discoveredOrder; // 노드가 발견된 순서를 기록하는 배열  
   int32            _discoveredCount = 0; // 노드가 발견된 순서  
   vector<bool>     _finished; // Dfs(i)가 종료 되었는지 여부  
   vector<int32>    _parent;  
};
