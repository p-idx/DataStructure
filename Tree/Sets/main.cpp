#include <iostream>

class Sets
{
public:
	Sets(int numberOfElements) : n(numberOfElements)
	{
		if (n < 2) throw "n must > 1";
		parent = new int[n];
		for (int i = 0; i < n; i++) parent[i] = -1;
	}

	void Union(int i, int j)
	{
		int temp = parent[i] + parent[j];
		if (parent[i] > parent[j])
		{
			parent[i] = j;
			parent[j] = temp;
		}
		else
		{
			parent[j] = i;
			parent[i] = temp;
		}
	}

	int Find(int i)
	{
		int r;
		for (r = i; parent[r] >= 0; r = parent[r]);

		while (i != r)
		{
			int s = parent[i];
			parent[i] = r;
			i = s;
		}
		return r;
	}

public:
	int* parent;
	int n;
};

int main()
{
	Sets s = Sets(12);
	s.Union(0, 4);
	s.Union(3, 1);
	s.Union(6, 10);
	s.Union(8, 9);

	//std::cout << s.Find(8);

	s.Union(7, 4);
	s.Union(6, 8);
	s.Union(3, 5);
	s.Union(2, 11);

	s.Union(0,s.Find(11));

	std::cout << s.Find(5);

}