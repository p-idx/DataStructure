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
		// runs를 2의 n제곱개로 패딩
		int k = 1;
		while ((runs.size() - 1) / k != 0) k *= 2;

		while (runs.size() != k)
			runs.push_back(std::vector<T>());

		// k개(2의 제곱)의 리프노드가 있는 포화 이진트리 배열 구성
		treeArr = new T[2 * k]; // 리프가 k 개면 2 * k - 1 노드 필요, [0] 은 비워둠
		
		// 리프노드 채우기
		for (int i = 0; i < k; i++)
		{
			if (runs[i].size() == 0)
				treeArr[k + i] = max;
			else
				treeArr[k + i] = runs[i][0];
		}
		// 토너먼트 구성
		
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

		// 이제 루트 뽑고 승자트리 재구성 반복 모든 런이 끝날때까지 = max 값이 올라올때까지?

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