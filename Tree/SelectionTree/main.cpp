#include <iostream>
#include <vector>
#include <deque>

template <typename T>
class WTree
{
	T* treeArr;
	std::vector<std::vector<T>> runs;

public:
	WTree()
	{}
	
	void InsertSortedList(const std::vector<T>& run)
	{
		runs.push_back(run);
	}

	std::vector<T> Merge(const T& max)
	{
		// runs�� 2�� n�������� �е�
		int k = 1;
		while ((runs.size() - 1) / k != 0) k *= 2;

		while (runs.size() != k)
			runs.push_back(std::vector<T>());

		// k��(2�� ����)�� ������尡 �ִ� ��ȭ ����Ʈ�� �迭 ����
		treeArr = new T[2 * k]; // ������ k ���� 2 * k - 1 ��� �ʿ�, [0] �� �����
		
		// ������� ä���
		for (int i = 0; i < k; i++)
		{
			if (runs[i].size() == 0)
				treeArr[k + i] = max;
			else
				treeArr[k + i] = runs[i][0];
		}
		// ��ʸ�Ʈ ����
		
		int s = k * 2;
		while (k > 1)
		{
			for (int i = 0; i < k / 2; i++)
			{
				treeArr[(k + (i * 2)) / 2] = 
					treeArr[k + (i * 2)] < treeArr[k + 1 + (i * 2)] ? 
					treeArr[k + (i * 2)] : treeArr[k + 1 + (i * 2)];
			}
			k /= 2;
		}

		for (int i = 1; i < s; i++)
			std::cout << treeArr[i] << std::endl;

		// ���� ��Ʈ �̰� ����Ʈ�� �籸�� �ݺ� ��� ���� ���������� = max ���� �ö�ö�����?

		return std::vector<T>();
	}
};

int main()
{
	WTree<int> wt;

	wt.InsertSortedList(std::vector<int>{14, 29, 57, 68});

	wt.InsertSortedList(std::vector<int>{20, 26});

	wt.InsertSortedList(std::vector<int>{22, 25, 50});

	wt.InsertSortedList(std::vector<int>{11, 24, 28});

	wt.Merge(99999);
	return 0;
}