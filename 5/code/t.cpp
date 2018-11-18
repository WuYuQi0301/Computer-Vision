#include <iostream>
using namespace std;

void sort(int& a, int& b, int& c)
{
	if(a > b)
	{
		int temp = a;
		a = b;
		b = temp; 
	}
	if(a > c)
	{
		int temp = a;
		a = c;
		c = temp;
	}
	if(b > c)
	{
		int temp = b;
		b = c;
		c = temp;
	}
}
int main(int argc, char const *argv[])
{
	int n1 = 100, n2 = 4, n3 = -1;
	sort(n1, n2, n3);
	cout << n1 <<n2 <<n3 << endl;
	return 0;
}