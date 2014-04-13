#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include<fstream>
using namespace std;

namespace ProcessScheduling
{
	class Configuration
	{
	public:
		string processFile;
		unsigned IODelay;
		unsigned contextSwitchDelay;
		int ctssQueues;
		bool debug;

		Configuration(istream& is);

		friend ostream& operator<<(ostream& os, Configuration& config);
	};
}
#endif