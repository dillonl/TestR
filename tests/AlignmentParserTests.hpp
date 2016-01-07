#ifndef TESTS_ALIGNMENTPARSER_H
#define TESTS_ALIGNMENTPARSER_H

#include <string>
#include <unordered_set>

#include "containers/KmerSet.hpp"
#include "containers/KmerSetManager.hpp"
#include "parsers/AlignmentParser.hpp"
#include "utils/ThreadPool.hpp"

namespace
{
	using namespace rufus;

	/*
	TEST(AlignmentParserTest, TestInsertionSpeed)
	{
		bool success = true;
		KmerSet kSet;
		// std::unordered_set< uint64_t > kSet;
		for (uint64_t i = 0; i < 500000000; ++i)
		{
			kSet.addKmer((InternalKmer)i);
		}
		ASSERT_TRUE(success); //(34588 ms total)
	}
	*/

	TEST(AlignmentParserTest, TestInsertionSpeed)
	{
		bool success = true;
		KmerSetManager kSetManager;
		// auto funct = std::bind(&KmerSetManager::addKmer, &kSetManager, std::placeholders::_1);
		auto funct = [&] ()
		{
			for (uint64_t i = 0; i < 25000000; ++i)
			{
				kSetManager.addKmer((InternalKmer)i);
				// ASSERT_TRUE(kSet.getKmerCount((InternalKmer)i) == 10);
			}
		};
		for (int i = 0; i < 5; ++i)
		{
			ThreadPool::Instance()->enqueue(funct);
			ThreadPool::Instance()->enqueue(funct);
			ThreadPool::Instance()->enqueue(funct);
			ThreadPool::Instance()->enqueue(funct);
		}
		ThreadPool::Instance()->joinAll();
		ASSERT_TRUE(success); //(72470 ms total)
	}

	/*
	TEST(AlignmentParserTest, ParseA)
	{
		//std::string alignment = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string alignment = "CCCCCCCCCCCCCCCCCCCCCCCCCA"; //AAAAAAAAAAAAAAAAAAAAAAAAGGGGGGGGGGGGGGGGGGGGGGGGGTTTTTTTTTTTTTTTTTTTTTTTTT";
		InternalKmer parsedKmer;

		std::unordered_set< InternalKmer, KmerHash, KmerKeyEqual > kSet;
		ThreadPool::Instance()->setThreadCount(5);
		std::cout << "starting" << std::endl;
		bool success = true;
		for (uint32_t i = 0; i < 1; ++i)
		{
			ThreadPool::Instance()->enqueue(std::bind(&AlignmentParser::ParseAlignment< 25 >, alignment.c_str(), alignment.size(), kSet));
			//success |= AlignmentParser::ParseAlignment(alignment.c_str(), alignment.size(), parsedKmer);
		}
		ThreadPool::Instance()->joinAll();
		std::cout << "ending" << std::endl;
		ASSERT_TRUE(success);
		//ASSERT_EQ();
	}
	*/

	/*
	TEST(AlignmentParserTest, HashTest)
	{
		//std::string alignment = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		// std::string alignment = "CCCCCCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAAAAAAGGGGGGGGGGGGGGGGGGGGGGGGGTTTTTTTTTTTTTTTTTTTTTTTTT";
		std::string alignment = "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";
		InternalKmer parsedKmer;

		std::unordered_set< InternalKmer, KmerHash, KmerKeyEqual > kSet;
		ThreadPool::Instance()->setThreadCount(1);
		std::cout << "starting" << std::endl;
		bool success = true;
		for (uint32_t i = 0; i < 2; ++i)
		{
			ThreadPool::Instance()->enqueue(std::bind(&AlignmentParser::ParseAlignment, alignment.c_str(), alignment.size(), &kSet));
			//success |= AlignmentParser::ParseAlignment(alignment.c_str(), alignment.size(), parsedKmer);
		}
		ThreadPool::Instance()->joinAll();
		std::cout << "ending: " << kSet.size() << std::endl;
		ASSERT_TRUE(success);
		//ASSERT_EQ();
	}
	*/
}

#endif //TESTS_ALIGNMENTPARSER_H
