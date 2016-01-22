#ifndef TESTS_BAMALIGNMENTREADERTESTS_HPP
#define TESTS_BAMALIGNMENTREADERTESTS_HPP

#include "config/DataPaths.h"
#include "alignments/BamAlignmentReader.h"
#include "utils/ThreadPool.hpp"

namespace
{
	using namespace rufus;

	TEST(BamAlignmentReaderTest, GetAllRegions)
	{
		/*
		std::string path = TEST_BAM_FILE;
		auto regions = BamAlignmentReader::getAllRegionsInBam(path);
		for (int i = 0; i < regions.size(); ++i)
		{
			ASSERT_EQ(i, regions[i]);
		}
		*/
	}

	TEST(BamAlignmentReaderTest, GetAllAlignmentsInRegion)
	{

		ThreadPool::SharedPtr threadPool = std::make_shared< ThreadPool >();
		std::string path = TEST_BAM_FILE;
		int regionID = 0;
		KmerSetManager::SharedPtr kmerSetManager = std::make_shared< KmerSetManager >();
        BamAlignmentReader reader(path, regionID);
		reader.processAllReadsInRegion(kmerSetManager);
		threadPool->stop();
		kmerSetManager->stopAndJoin();
	}

	TEST(BamAlignmentReaderTest, GetAllAlignmentsInRegion2)
	{
		ThreadPool::SharedPtr threadPool = std::make_shared< ThreadPool >();
		KmerSetManager::SharedPtr kmerSetManager = std::make_shared< KmerSetManager >();
		std::string path = "/uufs/chpc.utah.edu/common/home/marth-d1/data/WashU_serialTumors/bams/B0.bam";
		std::vector< BamAlignmentReader::SharedPtr > readers;
		auto regions = BamAlignmentReader::getAllRegionsInBam(path);
		for (int i = 0; i < regions.size(); ++i)
		{
			auto thr = [&]()
				{
					BamAlignmentReader::SharedPtr reader = std::make_shared< BamAlignmentReader >(path, regions[i]);
					reader->processAllReadsInRegion(kmerSetManager);
					readers.emplace_back(reader);
				};
			threadPool->enqueue(thr);
		}
		threadPool->stop();
		kmerSetManager->stopAndJoin();
	}
}


#endif //TESTS_BAMALIGNMENTREADERTESTS_HPP
