#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;



struct number
{
	int num[8];
	double dist;
	bool operator<(const number &a) const {
		return dist < a.dist;
	}
};

double getDist(number target, number num1)
{
	double ans = 0;
	for (int i = 0; i < 8; ++i)
		ans += pow(target.num[i]-num1.num[i],2)
	// ans = sqrt(ans);
	return ans;
}

int main(int argc, char const *argv[])
{
	priority_queue<number> pq;

	number target;
	target.dist = 0;
	for (int i = 0; i < 8; ++i)
		target.num[i] = i;

	number n[8] = ;
	return 0;
}