#include"Configuration.h"
#include<iostream>
#include<sstream>
#include<string>

namespace ProcessScheduling
{
	Configuration::Configuration(istream& is)
	{
		string identifier;
		string data;

		//read string but halt at '='
		while (getline(is, identifier, '='))
		{
			//read the data element
			if (!getline(is, data))
			{
				cerr << "Unexpected end of output!" << endl;
				return;
			}

			istringstream dataStream;
			dataStream.str(data);

			if (identifier == "ProcessFile")
			{
				processFile = data;
			}
			else if (identifier == "IOdelay")
			{
				dataStream >> IODelay ;
			}
			else if (identifier == "ContextSwitchDelay")
			{
				dataStream >> contextSwitchDelay;
			}
			else if (identifier == "CTSSQueues")
			{
				dataStream >> ctssQueues;
			}
			else if (identifier == "Debug")
			{
				if (data == "true")
					debug = true;
				else if (data == "false")
					debug = false;
				else
					cerr << "Unrecognized value for Debug: " << data << endl;
			}
			else
			{
				cerr << "Unrecognized identifier/value pair: " 
					 << identifier << "/" << data << endl;
			}
		}
	}

	ostream& operator<<(ostream& os, Configuration& config)
	{
		os << "ProcessFile = " << config.processFile << endl
		   << "IOdelay = " << config.IODelay << endl
		   << "ContextSwitchDelay = " << config.contextSwitchDelay << endl
		   << "CTSSQueues = " << config.ctssQueues << endl
		   << "Debug = " << ((config.debug) ? "true" : "false") << endl;
		return os;
	}
}