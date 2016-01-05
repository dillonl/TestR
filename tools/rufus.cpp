#include "utils/Parameters.h"

#include <iostream>

#include <boost/filesystem.hpp>

int main(int argc, char** argv)
{
	rufus::Parameters params;
	params.parseRUFUS(argc, argv);

	if (params.showHelp() || !params.validateRequired())
	{
	    params.printHelp();
		exit(0);
	}

	return 0;
}
