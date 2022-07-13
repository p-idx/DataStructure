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
	TreeNode<T>* left = nullptr;
	TreeNode<T>* right = nullptr;
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
		if (!p) return nullptr;
		else if (k < p->data.first) return Get(p->left, k);
		else if (k > p->data.first) return Get(p->right, k);
		return &(p->data);
	}

	virtual std::pair<K, E>* Get(const K& k)
	{
		return Get(root, k);
	}

	virtual void Insert(const std::pair<K, E>& thePair)
	{
		TreeNode<std::pair<K, E>>* p = root, * pp = 0;
		while (p)
		{
			pp = p;
			if (thePair.first < p->data.first) p = p->left;
			else if (thePair.first > p->data.first) p = p->right;
			else // 이미 키가 있으면 그 값을 변경
			{
				p->data.second = thePair.second;
			}
		}

		p = new TreeNode<std::pair<K, E>>;
		p->data.first = thePair.first;
		p->data.second = thePair.second;
		if (root) // 트리가 공백이 아닐 때
		{
			if (thePair.first < pp->data.first) pp->left = p;
			else
				pp->right = p;
		}
		else
			root = p;
	}

	virtual void Delete(const K&)
	{

	}

	void Inorder()
	{
		std::cout << "Inorder Print" << std::endl;
		Inorder(root);
	}

	void Preorder()
	{
		std::cout << "Preorder Print" << std::endl;
		Preorder(root);
	}

	void Inorder(TreeNode<std::pair<K, E>>* node)
	{
		if (node)
		{
			Inorder(node->left);
			std::cout << "Key: " << node->data.first << "  Val: " << node->data.second << std::endl;
			Inorder(node->right);
		}
	}

	void Preorder(TreeNode<std::pair<K, E>>* node)
	{
		if (node)
		{
			std::cout << "Key: " << node->data.first << "  Val: " << node->data.second << std::endl;
			Preorder(node->left);
			Preorder(node->right);
		}
	}

};

int main()
{
	using namespace std;
	BST<int, int> bst;

	pair<int, int> p;
	p.first = 30;
	p.second = 1;
	bst.Insert(p);
	
	p.first = 5;
	p.second = 1;
	bst.Insert(p);

	p.first = 2;
	p.second = 1;
	bst.Insert(p);

	p.first = 40;
	p.second = 1;
	bst.Insert(p);

	p.first = 80;
	p.second = 1;
	bst.Insert(p);

	p.first = 35;
	p.second = 1;
	bst.Insert(p);


	bst.Inorder();
	bst.Preorder();

	return 0;
}