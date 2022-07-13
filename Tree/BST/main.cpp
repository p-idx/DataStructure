#include <iostream>
#include <utility>
#include <exception>
#include <random>
#include <cmath>

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
	int height;

public:
	BST() : root(nullptr), height(0)
	{}

	int GetHeight()
	{
		SetHeight(0, root);
		return height;
	}

	void SetHeight(int h, TreeNode<std::pair<K,E>>* r)
	{
		if (r)
		{
			h++;
			SetHeight(h, r->left);
			SetHeight(h, r->right);
		}

		if (height < h)
			height = h;
	}

	virtual bool IsEmpty() const
	{
		return root ? false : true;
	}

	TreeNode<std::pair<K, E>>* getNode(TreeNode <std::pair<K, E>>* p, const K& k)
	{
		if (!p) return nullptr;
		else if (k < p->data.first) return getNode(p->left, k);
		else if (k > p->data.first) return getNode(p->right, k);
		return p;
	}

	virtual std::pair<K, E>* Get(const K& k)
	{
		TreeNode<std::pair<K, E>>* c = getNode(root, k);
		if (!c) throw "Key Error";
		return &(c->data);
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

	virtual void Delete(const K& k)
	{
		TreeNode<std::pair<K, E>>* p = root, * pp = nullptr;
		bool isLeftChild = false;
		while (p)
		{
			if (k < p->data.first)
			{
				pp = p;
				p = p->left;
				isLeftChild = true;
			}
			else if (k > p->data.first)
			{
				pp = p;
				p = p->right;
				isLeftChild = false;
			}
			else break;
		}

		if (!p) throw "Key Error";

		// p의 양쪽에 모두 자식이 있을 경우 **왼쪽으로 복사하면서 내려감
		while (p->left != nullptr && p->right != nullptr)
		{
			p->data = p->left->data; // 랭크를 통해 더 좋은 트리로
			pp = p;
			isLeftChild = true;
			p = p->left;
		}
		// p의 자식이 없을 경우
		if (p->left == nullptr && p->right == nullptr)
		{
			if (pp) // 부모가 있을 경우 = 삭제노드가 루트가 아닌 경우
			{
				if (isLeftChild) {
					pp->left = nullptr;
					std::cout << pp->data.first;
				}
				else
					pp->right = nullptr;
			}
			delete p;
		}
		else if (p->left != nullptr && p->right == nullptr) // p의 자식이 왼쪽에만 있을 경우
		{
			if (pp) // 부모가 있을 경우 = 삭제노드가 루트가 아닌 경우
			{
				if (isLeftChild)
					pp->left = p->left;
				else
					pp->right = p->left;
			}
			delete p;
		}
		else if (p->left == nullptr && p->right != nullptr) // p의 자식이 오른쪽에만 있을 경우
		{
			if (pp) // 부모가 있을 경우 = 삭제노드가 루트가 아닌 경우
			{
				if (isLeftChild)
					pp->left = p->right;
				else
					pp->right = p->right;
			}
			delete p;
		}
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
	
	void Split(const K& k, BST<K, E>& small, std::pair<K, E>*& mid, BST<K, E>& big)
	{
		if (!root) { small.root = big.root = nullptr; return; }

		TreeNode<std::pair<K, E>>* sHead = new TreeNode<std::pair<K, E>>,
			* s = sHead,
			* bHead = new TreeNode<std::pair<K, E>>,
			* b = bHead,
			* current = root;

		while (current)
		{
			if (k < current->data.first)
			{
				b->left = current;
				b = current;
				current = current->left;
			}
			else if (k > current->data.first)
			{
				s->right = current;
				s = current;
				current = current->right;
			}
			else // split
			{
				s->right = current->left;
				b->left = current->right;
				small.root = sHead->right;
				delete sHead;
				big.root = bHead->left;
				delete bHead;
				mid = new std::pair<K, E>(current->data.first, current->data.second);
				delete current;
				return;
			}
		}
		s->right = b->left = nullptr;
		small.root = sHead->right;
		delete sHead;
		big.root = bHead->left;
		delete bHead;
		mid = nullptr;
		return;
	}

	E& operator[](const K& k)
	{
		try {
			return Get(k)->second;
		}
		catch (const char*)
		{
			Insert(std::pair<K, E>(k, NULL));
			return Get(k)->second;
		}
	}
};



int main()
{
	using namespace std;
	BST<int, int> bst;
	std::random_device rd;

	int n = 10000;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dis(0, 200000);

	for (int i = 0; i < n; i++)
	{
		bst[dis(gen)] = i;
	}

	std::cout << "res: " << bst.GetHeight() / log2(n);

	return 0;
}