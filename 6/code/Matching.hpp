#ifndef MATCHING_HPP
#define MATCHING_HPP

#include <vector>
using namespace std;

class Matching
{
public:
	Matching();
	~Matching();

	vector<pair<int, int> > MakePair(double threshold);
private:

};


#endif