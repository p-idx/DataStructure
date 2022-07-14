#include <iostream>
#include <vector>
#include <utility>

template <typename T>
class WTree
{
	std::pair<int, T>* treeArr;
	std::vector<std::vector<T>> runs;

public:
	WTree() : treeArr(nullptr)
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

		// k��(2�� ����)�� ������尡 �ִ� ��ȭ ����Ʈ�� �迭 ����, pair�� int�� runs�ε���.
		treeArr = new std::pair<int, T>[2 * k]; // ������ k ���� 2 * k - 1 ��� �ʿ�, [0] �� �����
		
		// ������� ä���
		for (int i = 0; i < k; i++)
		{
			if (runs[i].size() == 0)
				treeArr[k + i].second = max;
			else
				treeArr[k + i].second = runs[i][0];
			treeArr[k + i].first = i;
		}

		// ��ʸ�Ʈ ����
		int kk = k;
		int s = k * 2;
		while (k > 1)
		{
			for (int i = 0; i < k / 2; i++)
			{
				treeArr[(k + (i * 2)) / 2] =
					treeArr[k + (i * 2)].second < treeArr[k + 1 + (i * 2)].second ?
					treeArr[k + (i * 2)] : treeArr[k + 1 + (i * 2)];
			}
			k /= 2;
		}

		for (int i = 1; i < s; i++)
			std::cout << treeArr[i].first << treeArr[i].second << std::endl;


		// ���� ��Ʈ �̰� ����Ʈ�� �籸�� �ݺ� ��� ���� ���������� = max ���� �ö�ö�����?
		std::vector<T> ret;
		
		std::vector<int> runs_idx;
		runs_idx.reserve(runs.size());
		for (int i = 0; i < runs.size(); i++)
			runs_idx.push_back(0);

		while (treeArr[1].second != max)
		{
			// ��Ʈ ���� ����
			ret.push_back(treeArr[1].second);

			// ������� ���ο� ������ ��ü
			int idx = treeArr[1].first;
			runs_idx[idx]++;
			if (runs[idx].size() != runs_idx[idx])
				treeArr[kk + idx].second = runs[idx][runs_idx[idx]];
			else
				treeArr[kk + idx].second = max;

			// ��ʸ�Ʈ �籸��
			int k = kk;
			while (k > 1)
			{
				for (int i = 0; i < k / 2; i++)
				{
					treeArr[(k + (i * 2)) / 2] =
						treeArr[k + (i * 2)].second < treeArr[k + 1 + (i * 2)].second ?
						treeArr[k + (i * 2)] : treeArr[k + 1 + (i * 2)];
				}
				k /= 2;
			}
		}

		return ret;
	}
};

int main()
{
	WTree<int> wt;

	wt.InsertSortedList(std::vector<int>{14, 29, 57, 68});

	wt.InsertSortedList(std::vector<int>{20, 26});

	wt.InsertSortedList(std::vector<int>{22, 25, 50});

	wt.InsertSortedList(std::vector<int>{11, 24, 28});

	std::vector<int> ret = wt.Merge(99999);
	for (auto& e : ret)
	{
		std::cout << e << ' ';
	}
	
	return 0;
}