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
		// runs를 2의 n제곱개로 패딩
		int k = 1;
		while ((runs.size() - 1) / k != 0) k *= 2;

		while (runs.size() != k)
			runs.push_back(std::vector<T>());

		// k개(2의 제곱)의 리프노드가 있는 포화 이진트리 배열 구성, pair의 int는 runs인덱스.
		treeArr = new std::pair<int, T>[2 * k]; // 리프가 k 개면 2 * k - 1 노드 필요, [0] 은 비워둠
		
		// 리프노드 채우기
		for (int i = 0; i < k; i++)
		{
			if (runs[i].size() == 0)
				treeArr[k + i].second = max;
			else
				treeArr[k + i].second = runs[i][0];
			treeArr[k + i].first = i;
		}

		// 토너먼트 구성
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


		// 이제 루트 뽑고 승자트리 재구성 반복 모든 런이 끝날때까지 = max 값이 올라올때까지?
		std::vector<T> ret;
		
		std::vector<int> runs_idx;
		runs_idx.reserve(runs.size());
		for (int i = 0; i < runs.size(); i++)
			runs_idx.push_back(0);

		while (treeArr[1].second != max)
		{
			// 루트 빼서 삽입
			ret.push_back(treeArr[1].second);

			// 리프노드 새로운 것으로 교체
			int idx = treeArr[1].first;
			runs_idx[idx]++;
			if (runs[idx].size() != runs_idx[idx])
				treeArr[kk + idx].second = runs[idx][runs_idx[idx]];
			else
				treeArr[kk + idx].second = max;

			// 토너먼트 재구성
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