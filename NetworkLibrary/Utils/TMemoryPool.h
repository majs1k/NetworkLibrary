//---------------------------------------------------------------
// 
// 메모리풀
// 
// 오브젝트풀 / 프리리스트
// 특정 구조체/클래스를 초기 사이즈 할당
// 이후 필요시 계속 노드 추가
// 스택 방식으로 미사용 오브젝트 블록 관리
// 
// 모니터링 용이
// 메모리 누수 - 프로젝트 메인 폴더에 파일로그 출력
// 메모리 침범 - DebugBreak
// 
// 메모리 누수로 메모리 사용량 올라가는게 확인된다면,
// 1. 일일이 플레이하면서 어떤 상황에서 발생하는지 확인
// 2. 최근 추가된 컨텐츠 코드 점검
// 
// 사용법
// CMemoryPool<DATA> MemPool(300, false);
// DATA* pData = MemPool.Alloc();
// pData 사용
// MemPool.Free(pData);
// 
//---------------------------------------------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <fstream>
#include <list>

//#define DEBUG_MEMORYPOOL

template <class T>
class TMemoryPool
{
private:
	struct Node
	{
#ifdef DEBUG_MEMORYPOOL
		void* underflow_;
#endif
		T data_;

#ifdef DEBUG_MEMORYPOOL
		void* overflow_;
#endif
		Node* next_;
	};

	struct AllocInfo
	{
		void* ptr_;
		char fileName_[128];
		int  lineNum_;

		AllocInfo(void* ptr, const char* fileName, int lineNum)
			:ptr_(ptr), lineNum_(lineNum)
		{
			strcpy_s(fileName_, 128, fileName);
		}
	};

	Node* top_;
	int size_;
	int capacity_;
	bool isPlacementNew_;

#ifdef DEBUG_MEMORYPOOL
	std::list<AllocInfo> allocList_;
#endif

public:
	TMemoryPool(int size, bool isPlacementNew = false)
	{
		top_ = nullptr;
		size_ = size;
		capacity_ = size;
		isPlacementNew_ = isPlacementNew;

		for (int i = 0; i < size; i++)
		{
			Node* node = (Node*)malloc(sizeof(Node));

#ifdef DEBUG_MEMORYPOOL
			node->underflow_ = nullptr;
			node->overflow_ = nullptr;
#endif

			if (!isPlacementNew)
			{
				new (&(node->data_)) T();
			}

			node->next_ = top_;
			top_ = node;
		}
	}

	~TMemoryPool()
	{
		Node* cur = top_;

		while (cur)
		{
			Node* next = cur->next_;

			cur->data_.~T();
			Free(cur);

			cur = next;
		}

#ifdef DEBUG_MEMORYPOOL
		if (allocList_.size() == 0)
			return;

		std::ofstream fout("memory_pool_leak.txt");

		for (auto& i : allocList_)
		{
			fout << "[" << i.ptr_ << "]" << " " << i.fileName_ << ": " << i.lineNum_ << std::endl;
		}

		fout.close();
#endif
	}

#ifdef DEBUG_MEMORYPOOL
	T* Alloc(const char* fileName, int lineNum)
	{
		if (size_ <= 0)
		{
			Node* node = (Node*)malloc(sizeof(Node));
			new (&(node->data_)) T();

			node->underflow_ = this;
			node->overflow_ = this;

			capacity_++;

			return &(node->data_);
		}

		allocList_.push_back(AllocInfo((void*)top_, fileName, lineNum));

		if (top_->underflow_ != nullptr || top_->overflow_ != nullptr)
		{
			DebugBreak();
		}

		top_->underflow_ = this;
		top_->overflow_ = this;

		T* value = &(top_->data_);

		if (isPlacementNew_)
			new (value)T();

		top_ = top_->next_;
		size_--;

		return value;
	}
#else
	T* Alloc()
	{
		if (size_ <= 0)
		{
			Node* node = (Node*)malloc(sizeof(Node));
			new (node)T();
			capacity_++;

			return &(node->data_);
		}

		T* value = (T*)top_;

		if (isPlacementNew_)
			new (value)T();

		top_ = top_->next_;
		size_--;

		return value;
	}
#endif

	bool Free(T* value)
	{
		if (isPlacementNew_)
			value->~T();

		Node* node = (Node*)value;

#ifdef DEBUG_MEMORYPOOL
		node = (Node*)((char*)value - sizeof(void*));

		if (size_ >= capacity_)
		{
			DebugBreak();
			return FALSE;
		}

		if (node->underflow_ != this || node->overflow_ != this)
		{
			DebugBreak();
			return FALSE;
		}

		node->underflow_ = nullptr;
		node->overflow_ = nullptr;

		for (auto it = allocList_.begin(); it != allocList_.end();)
		{
			if ((*it).ptr_ == (void*)node)
			{
				it = allocList_.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
#endif

		node->next_ = top_;
		top_ = node;
		size_++;

		return true;
	}

	int	Capacity()
	{
		return capacity_;
	}

	int	Size()
	{
		return size_;
	}
};

#ifdef DEBUG_MEMORYPOOL
#define Alloc()			Alloc(__FILE__, __LINE__)
#endif
