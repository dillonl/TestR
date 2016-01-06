#ifndef TESTS_ALIGNMENTPARSER_H
#define TESTS_ALIGNMENTPARSER_H

#include <string>

#include <boost/lockfree/queue.hpp>

#include "parsers/AlignmentParser.hpp"
#include "utils/ThreadPool.hpp"

namespace
{
	using namespace rufus;
	TEST(AlignmentParserTest, ParseA)
	{
		//std::string alignment = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string alignment = "CCCCCCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAAAAAAGGGGGGGGGGGGGGGGGGGGGGGGGTTTTTTTTTTTTTTTTTTTTTTTTT";
		InternalKmer parsedKmer;

		std::unordered_map< InternalKmer, char > map;
		ThreadPool::Instance()->setThreadCount(5);
		std::cout << "starting" << std::endl;
		bool success = true;
		for (uint32_t i = 0; i < 100000000; ++i)
		{
			ThreadPool::Instance()->enqueue(std::bind(&AlignmentParser::ParseAlignment< 25 >, alignment.c_str(), alignment.size()));
			//success |= AlignmentParser::ParseAlignment(alignment.c_str(), alignment.size(), parsedKmer);
		}
		ThreadPool::Instance()->joinAll();
		std::cout << "ending" << std::endl;
		ASSERT_TRUE(success);
		//ASSERT_EQ();
	}
}

#endif //TESTS_ALIGNMENTPARSER_H
