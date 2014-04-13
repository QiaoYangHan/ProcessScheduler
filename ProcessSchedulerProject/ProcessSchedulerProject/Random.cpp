// Task
// get the resource file "scheduling.txt"
// the processes need to be scheduled are on the this file
// run two algorithm on these processes and produce output

#include"Random.h"
#include<iostream>
#include<limits>

namespace ProcessScheduling
{
	Random::Random(istream& is)
	{
		unsigned i;
		while(is >> i)
			random.push_back(i);
		sentinel = 0;
	}

	unsigned Random::next()
	{
		unsigned value = random[sentinel];
		++sentinel;

		// Cycle to the front
		if (sentinel == random.size())
			sentinel = 0;

		return value;
	}

	double Random::getProbability()
	{
		double probability = (double)next() / INT_MAX;
		return probability;
	}

	void Random::reset()
	{
		sentinel = 0;
	}

}

