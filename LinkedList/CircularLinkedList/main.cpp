#include <iostream>

#include <chrono>
#include <algorithm>
#include <vector>
#include <iterator>

#define AVAIL 1
/*
* 원형 더블 연결리스트
* - insert, erase, at, [], size 연산 지원
* - 인덱싱은 파이썬 처럼 음수도 가능
* - stack처럼 활용시 top 함수 기능은 [-1]
* - push_back, push_front, flush(가용공간 비우기용) 구현 예정
* - concat, reverse, sort(합병) 구현 예정
*
* 탐색은 들어온 index와 현재 size의 차이를 비교해 시계방향, 반시계방향 순회를 결정함
* 양방향 이터레이터 상속해 구현, STL 함수들 호환(양방향 반복자만)
* 
* * 가용공간 리스트 적용, 빈번한 삽입 삭제 효율성
* * 가용공간은 클래스 스태틱 멤버변수로 관리
* * 가용공간은 런타임 시 더이상 존재하는 연결리스트 객체가 없을 때 모두 반납함
*/


template <typename T>
class LinkedList
{
	template <typename U>
	class Node {
		friend class iterator;
	public:
		U data;
		Node<U>* left, *right;
	};

	// template <typename U>
	class iterator : public std::iterator<
		std::bidirectional_iterator_tag, // iterator_category
		T,                      // value_type
		T,                      // difference_type
		const T*,               // pointer
		const T&                // reference
	>
	{
	protected:
		Node<T>* cur;
	public:
		explicit iterator(Node<T>* startNode = nullptr) : cur(startNode)
		{}

		T& operator*() const { return cur->data; }
		T* operator->() const { return &(cur->data); }

		iterator& operator++() 
		{ 
			cur = cur->right;
			return *this;
		}

		iterator operator++(int)
		{
			iterator old = *this;
			cur = cur->right;
			return old;
		}

		iterator& operator--()
		{
			cur = cur->left;
			return *this;
		}

		iterator operator--(int)
		{
			iterator old = *this;
			cur = cur->left;
			return old;
		}

		bool operator != (const iterator right) const { return cur != right.cur; }
		bool operator == (const iterator right) const { return cur == right.cur; }
	};

	class reverse_iterator : public iterator
	{
	public:
		explicit reverse_iterator(Node<T>* startNode = nullptr) : iterator(startNode)
		{}

		reverse_iterator& operator++()
		{
			iterator::cur = iterator::cur->left;
			return *this;
		}

		reverse_iterator operator++(int)
		{
			reverse_iterator old = *this;
			iterator::cur = iterator::cur->left;
			return old;
		}

		reverse_iterator& operator--()
		{
			iterator::cur = iterator::cur->right;
			return *this;
		}

		reverse_iterator operator--(int)
		{
			reverse_iterator old = *this;
			iterator::cur = iterator::cur->right;
			return old;
		}
	};
private:
	Node<T>* head;
	size_t len;
	static Node<T>* avail;
	static size_t total_list_num;

	static Node<T>* getNode()
	{
		Node<T>* newNode;
		if (avail)
		{
			newNode = avail;
			avail = avail->right;
		}
		else
			newNode =  new Node<T>;
		return newNode;
	}

	static void retNode(Node<T>*& nodeP)
	{
		nodeP->right = avail;
		avail = nodeP;
		nodeP = nullptr;
	}

	Node<T>* findPt(const int index) const
	{
		Node<T>* cur = head;
		if (index >= (int)len || index < -(int)len)
			throw "List index error.";
		
		if (index >= 0) 
		{
			if ((int)len - index < index + 1)
			{
				for (int i = 0; i != (int)len - index; i++)
					cur = cur->left;
			}
			else
			{
				for (int i = 0; i != index + 1; i++)
					cur = cur->right;
			}
		}
		else
		{
			if (-index < (int)len + index + 1)
			{
				for (int i = 0; i != -index; i++)
					cur = cur->left;
			}
			else
			{
				for (int i = 0; i != (int)len + index + 1; i++)
					cur = cur->right;
			}
		}
		
		return cur;
	}

public:
	
	LinkedList() : len(0)
	{
		head = new Node<T>; // dummy
		head->left = head->right = head;
		total_list_num++;
	}
	// 복사, 대입, 소멸

	LinkedList(const LinkedList& l) : LinkedList()
	{
		for (int i = 0; i < l.size(); i++)
			append(l.at(i));
	}

	~LinkedList()
	{
		Node<T>* cur = head->right, *prev;
		while (cur != head)
		{
			prev = cur;
			cur = cur->right;
#if AVAIL == 0
			delete prev;
#else
			retNode(prev);
#endif
		}
		delete head;

		total_list_num--;
		if (total_list_num == 0)
		{
			cur = avail;
			while (cur)
			{
				prev = cur;
				cur = cur->right;
				delete prev;
			}
			avail = nullptr;
		}
	}

	bool isEmpty() const { return (len == 0) ? true : false; }

	const size_t size() const { return len; }

	LinkedList& append(T data)
	{
#if AVAIL == 0
		Node<T>* newNode = new Node<T>;
#else
		Node<T>* newNode = getNode();
#endif
		newNode->data = data;

		newNode->right = head;
		head->left->right = newNode;
		
		newNode->left = head->left;
		head->left = newNode;

		len++;

		return *this;
	}

	LinkedList& insert(T data, const int index = 0)
	{
		if (index == len)
			append(data);
		else
		{
			Node<T>* next = findPt(index);

#if AVAIL == 0
			Node<T>* newNode = new Node<T>;
#else
			Node<T>* newNode = getNode();
#endif
			newNode->data = data;

			if (index >= 0)
			{
				newNode->right = next;
				next->left->right = newNode;

				newNode->left = next->left;
				next->left = newNode;
			}
			else
			{
				newNode->left = next;
				next->right->left = newNode;

				newNode->right = next->right;
				next->right = newNode;
			}
			len++;
		}
		return *this;
	}

	LinkedList& erase(const int index)
	{
		Node<T>* cur = findPt(index);
		// 지울 인덱스에 도착
		cur->right->left = cur->left;
		cur->left->right = cur->right;

#if AVAIL == 0
		delete cur;
#else
		retNode(cur);
#endif
		len--;

		return *this;
	}
	
	const T& at(const int index) const
	{
		Node<T>* cur = findPt(index);
		return cur->data;
	}

	T& operator[](const int index) const
	{
		Node<T>* cur = findPt(index);
		return cur->data;
	}

	friend std::ostream& operator<<(std::ostream& os, const LinkedList& ll)
	{
		os << ll.at(0);
		for (int i = 1; i < ll.size(); i++)
			os << ' ' << ll.at(i);
		return os;
	}

	iterator begin() { return isEmpty() ? iterator(head) : iterator(head->right); }

	iterator end() { return iterator(head); }

	reverse_iterator rbegin() { return isEmpty() ? reverse_iterator(head) : reverse_iterator(head->left); }

	reverse_iterator rend() { return reverse_iterator(head); }
};

template <typename T>
LinkedList<T>::Node<T>* LinkedList<T>::avail = nullptr;

template <typename T>
size_t LinkedList<T>::total_list_num = 0;


int main()
{
	LinkedList<int> ll;

	for (int i = 0; i < 5; i++)
	{
		ll.insert(i, i);
	}
	//int N = 1000000;
	std::cout << ll;

	// std::fill(ll.begin(), ll.end(), -7);

	std::cout << ll[-2];
	std::cout << std::endl;

	LinkedList<int> ll2;
	for (int i = 0; i < 10; i++)
	{
		ll2.append(0);
	}

	std::cout << ll2;

	std::cout << std::endl;
	std::copy(ll.begin(), ll.end(), ll2.begin());

	std::cout << ll2;
	std::cout << std::endl;
	
	for (int i = - ((int)ll2.size()); i != 0; i++) {
		std::cout << ll2.at(i);
	}

	std::cout << std::endl;
	for (auto node : ll2)
	{
		std::cout << node;
	}

	std::cout << std::endl;
	for(auto iter = ll2.rbegin(); iter != ll2.rend(); iter++)
	{
		std::cout << *iter;
	}
	//auto start = std::chrono::system_clock::now();

	//std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	//std::cout << sec.count() << std::endl;

	return 0;
}