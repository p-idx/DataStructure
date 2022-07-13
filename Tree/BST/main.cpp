#include <iostream>
#include <utility>

template <typename K, typename E>
class Dictionary
{
public:
	virtual bool IsEmpty() const = 0;
	virtual std::pair<K, E>* Get(const K&) = 0;
	virtual void Insert(const std::pair<K, E>&) = 0;
	virtual void Delete(const K&) = 0;
};

template <typename K, typename E>
class BST;

template <typename T>
class TreeNode
{
	template <typename K, typename E>
	friend class BST;
protected:
	T data;
	TreeNode<T>* left;
	TreeNode<T>* right;
};

template <typename K, typename E>
class BST : public Dictionary<K, E>
{
private:
	TreeNode<std::pair<K, E>>* root;

public:
	BST() : root(nullptr)
	{}

	virtual bool IsEmpty() const
	{
		return root ? false : true;
	}

	std::pair<K, E>* Get(TreeNode <std::pair<K, E>>* p, const K& k)
	{
		return &(p->data);
	}

	virtual std::pair<K, E>* Get(const K& k)
	{
		return this->Get(this->root, k);
	}

	virtual void Insert(const std::pair<K, E>&)
	{

	}
	virtual void Delete(const K&)
	{

	}
};
int main()
{
	using namespace std;
	BST<int, int> bst;
	cout << "Test" << endl;

	return 0;
}