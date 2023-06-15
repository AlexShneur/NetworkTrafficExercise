#include "netAnalyzer.h"

int main(int argc, const char* argv[]) 
{
	if (argc < 2)
	{
		std::cout << "No input file found." << std::endl;
		return EXIT_FAILURE;
	}

    NetAnalyzer netAn;
    netAn.Analyze(argv[1]);

    return EXIT_SUCCESS;
}
