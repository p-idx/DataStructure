#include <iostream>

/*
* 원형 더블 연결리스트
* 각종 리스트 연산 구현
* -
* -
* -
* -
* -
* 이터레이터 구현, STL 함수들 호환
* * 가용공간 리스트 적용, 빈번한 삽입 삭제 효율성
* * 가용공간은 클래스 스태틱 변수로 관리, 클래스 활용 공부
*/



template <typename T>
class LinkedList
{
	template <typename U>
	class Node {
	public:
		U data;
		Node<U>* left, *right;
	};

private:
	Node<T>* head;
	size_t len;

public:
	LinkedList() : len(0)
	{
		head = new Node<T>; // dummy
		head->left = head->right = head;
	}
	// 복사, 대입, 소멸

	bool isEmpty() const { return (head->left == head->right) ? true : false; }

	LinkedList& insert(T data, const size_t index)
	{
		if (index > len || index < 0) throw "insert: index out of range.";

		Node<T>* newNode = new Node<T>;
		newNode->data = data;
		

		// 삽입 인덱스 앞까지
		
		// 인덱스가 레프트에 가까우면 레프트부터

		Node<T>* cur = head;
		for (size_t i = 0; i != index; i++)
			cur = cur->right;

		cur->right->left = newNode;
		newNode->right = cur->right;
		newNode->left = cur;
		cur->right = newNode;

		len++;

		return *this;
	}
	
	T& at(const size_t index) const
	{
		if (isEmpty()) throw "at: list is empty.";
		if (index >= len || index < 0) throw "at: index out of range.";

		Node<T>* cur = head->right;
		for (size_t i = 0; i != index; i++)
			cur = cur->right;
		
		return cur->data;
	}

	size_t size() const { return len; }

	friend std::ostream& operator<<(std::ostream& os, const LinkedList& ll)
	{
		os << ll.at(0);
		for (int i = 1; i < ll.size(); i++)
			os << ' ' << ll.at(i);
		return os;
	}
};

int main()
{
	LinkedList<int> ll;
	ll.insert(2, 0).insert(3, 0);
	std::cout << ll;
	return 0;
}