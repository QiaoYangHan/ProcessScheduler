#ifndef RANDOM_H
#define RANDOM_H

#include<vector>
#include<fstream>
using namespace std;

namespace ProcessScheduling
{
	class Random 
	{
	private:
		vector<unsigned> random;
		size_t sentinel;
	public:
		// Constructor
		Random(istream& ifs);

		// Get the next random number
		unsigned next();

		// Get Probability
		double getProbability();

		void reset();
	};
}
#endif