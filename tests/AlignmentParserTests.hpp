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

	TEST(AlignmentParserTest, TestInsertionSpeed)
	{
		bool success = true;
		KmerSet kSet;
		static std::atomic< uint64_t > counter(0);
		// std::unordered_set< uint64_t > kSet;
		for (uint64_t i = 0; i < 50000000; ++i)
		{
			kSet.addKmer((InternalKmer)counter++);
		}
		ASSERT_TRUE(success); //(34588 ms total)
	}

	TEST(AlignmentParserTest, TestInsertionSpeed)
	{
		bool success = true;
		KmerSetManager kSetManager;
		static std::atomic< uint64_t > counter(0);
		// auto funct = std::bind(&KmerSetManager::addKmer, &kSetManager, std::placeholders::_1);
		auto funct = [&] ()
		{
			for (uint64_t i = 0; i < 2500000; ++i)
			{
				kSetManager.addKmer((InternalKmer)counter++);
			}
			std::cout << "finished one loop" << std::endl;
		};
		for (int i = 0; i < 5; ++i)
		{
			ThreadPool::Instance()->enqueue(funct);
			ThreadPool::Instance()->enqueue(funct);
			ThreadPool::Instance()->enqueue(funct);
			ThreadPool::Instance()->enqueue(funct);
		}
		kSetManager.stop();
		ThreadPool::Instance()->joinAll();
		auto kSet = kSetManager.getKmerSet();
		std::cout << "kmer count: " << kSet->getSetSize() << std::endl;
		std::cout << "total count: " << counter << std::endl;
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
