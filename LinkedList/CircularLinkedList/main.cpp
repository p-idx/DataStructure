#include <iostream>

/*
* ���� ���� ���Ḯ��Ʈ
* ���� ����Ʈ ���� ����
* -
* -
* -
* -
* -
* ���ͷ����� ����, STL �Լ��� ȣȯ
* * ������� ����Ʈ ����, ����� ���� ���� ȿ����
* * ��������� Ŭ���� ����ƽ ������ ����, Ŭ���� Ȱ�� ����
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
	// ����, ����, �Ҹ�

	bool isEmpty() const { return (head->left == head->right) ? true : false; }

	LinkedList& insert(T data, const size_t index)
	{
		if (index > len || index < 0) throw "insert: index out of range.";

		Node<T>* newNode = new Node<T>;
		newNode->data = data;
		

		// ���� �ε��� �ձ���
		
		// �ε����� ����Ʈ�� ������ ����Ʈ����

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