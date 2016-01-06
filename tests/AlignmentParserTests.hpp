#ifndef TESTS_ALIGNMENTPARSER_H
#define TESTS_ALIGNMENTPARSER_H

#include <string>

#include "parsers/AlignmentParser.hpp"

namespace
{
	using namespace rufus;
	TEST(AlignmentParserTest, ParseA)
	{
		//std::string alignment = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string alignment = "CCCCCCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAAAAAAGGGGGGGGGGGGGGGGGGGGGGGGGTTTTTTTTTTTTTTTTTTTTTTTTT";
		InternalKmer parsedKmer;
		std::cout << "starting" << std::endl;
		bool success = true;
		for (uint32_t i = 0; i < 1000000; ++i)
		{
			success |= AlignmentParser::ParseAlignment(alignment.c_str(), alignment.size(), parsedKmer);
		}
		std::cout << "ending" << std::endl;

		ASSERT_TRUE(success);
		//ASSERT_EQ();
	}
}

#endif //TESTS_ALIGNMENTPARSER_H
