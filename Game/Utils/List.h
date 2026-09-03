//-------------------------------------------------------------------------
//
// 연결 리스트
// 
// **학습용**
// 
// 템플릿은 반드시 수정 후 재컴파일 해야 오류 확인 가능
// 이터레이터는 지역 내에서만 유효 -> 다른 함수로 전달 금지
// 해당 컨테이너 자료형을 *iter로 받아서 사용
// ex) A* ptr = *iter;
//
//-------------------------------------------------------------------------
#pragma once

template<typename T>
class List
{
public:
	struct Node
	{
		T data_;
		Node* prev_;
		Node* next_;

		Node() = default;

		Node(T data)
			:data_(data), prev_(nullptr), next_(nullptr)
		{
		}
	};

	class iterator
	{
		friend class List;
		Node* node_;

	public:
		iterator(Node* node = nullptr)
			:node_(node)
		{
		}

		iterator& operator++()
		{
			node_ = node_->next_;
			return *this;
		}

		const iterator operator++(int)
		{
			const iterator temp(*this);  // this로 수정
			node_ = node_->next_;
			return temp;
		}

		iterator& operator--()
		{
			node_ = node_->prev_;
			return *this;
		}

		const iterator operator--(int)
		{
			const iterator temp(*this);
			node_ = node_->prev_;
			return temp;
		}

		bool operator==(const iterator& rhs) const
		{
			return node_ == rhs.node_;  // 같은 노드 주소인지
		}

		bool operator!=(const iterator& rhs) const
		{
			return !(*this == rhs);
			//return _node != rhs._node;
		}

		T& operator*() const
		{
			return node_->data_;
		}
	};

private:
	Node* head_;
	Node* tail_;

	int size_;

public:
	List()
	{
		head_ = new Node();
		tail_ = new Node();

		head_->next_ = tail_;
		tail_->prev_ = head_;

		size_ = 0;
	}

	~List()
	{
		Node* cur = head_;

		while (cur)
		{
			Node* next = cur->next_;
			delete cur;
			cur = next;
		}
	}

	iterator begin() const
	{
		return iterator(head_->next_);	// 생성자 호출
	}

	iterator end() const
	{
		return iterator(tail_);
	}

	void push_front(T data)
	{
		Node* node = new Node(data);
		Node* nextNode = head_->next_;

		nextNode->prev_ = node;
		node->next_ = nextNode;

		head_->next_ = node;
		node->prev_ = head_;

		size_++;
	}

	void push_back(T data)
	{
		Node* node = new Node(data);
		Node* prevNode = tail_->prev_;

		prevNode->next_ = node;
		node->prev_ = prevNode;

		tail_->prev_ = node;
		node->next_ = tail_;

		size_++;
	}

	void pop_front()
	{
		if (head_->next_ != tail_)
		{
			Node* node = head_->next_;

			node->prev_->next_ = node->next_;
			node->next_->prev_ = node->prev_;

			delete node;

			size_--;
		}
	}

	void pop_back()
	{
		if (tail_->prev_ != head_)
		{
			Node* node = tail_->prev_;

			node->prev_->next_ = node->next_;
			node->next_->prev_ = node->prev_;

			delete node;

			size_--;
		}
	}

	void Clear()
	{
		Node* cur = head_->next_;

		while (cur != tail_)
		{
			Node* next = cur->next_;
			delete cur;
			cur = next;
		}

		head_->next_ = tail_;
		tail_->prev_ = head_;
	}

	int size() const
	{
		return size_;
	}

	bool empty() const
	{
		return !size_;
	}

	iterator erase(iterator it)
	{
		Node* node = it.node_;

		++it;

		node->prev_->next_ = node->next_;
		node->next_->prev_ = node->prev_;
		delete node;

		size_--;

		return it;
	}

	void Remove(T data)
	{
		// 반드시 아래 방식으로 해야함. 이터레이터는 삭제되는 순간 어떻게 동작할지 예측 불가
		// 이터레이터는 지역에서만 사용하고, 인자로 전달하지 말것
		for (iterator it = begin(); it != end();)
		{
			if (data == *it)
				it = erase(it);
			else
				++it;
		}
	}
};
