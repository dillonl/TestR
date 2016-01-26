#ifndef TESTS_BAMALIGNMENTREADERTESTS_HPP
#define TESTS_BAMALIGNMENTREADERTESTS_HPP

#include "config/DataPaths.h"
#include "alignments/BamAlignmentReader.h"
#include "utils/ThreadPool.hpp"
#include "containers/KmerSet.hpp"

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

		// SparseKmerSet::SharedPtr kmerSetPtr = std::make_shared< SparseKmerSet >();
		// KmerSet::SharedPtr kmerSetPtr = std::make_shared< KmerSet >();
		// for (int i = 0; i < 1000000; ++i)
		// for (int i = 0; i < 1000000000; ++i)
		// {
			// for (int j = 0; j < 1000; ++j)
			// {
				// kmerSetPtr->addKmer(i);
			// }
		// }
	}

	TEST(BamAlignmentReaderTest, GetAllAlignmentsInRegion)
	{
		/*
		std::string path = TEST_BAM_FILE;
		BamAlignmentReader::SharedPtr readerPtr = std::make_shared< BamAlignmentReader >(path);
		readerPtr->processAllReadsInBam();
		*/
	}

	TEST(BamAlignmentReaderTest, GetAllAlignmentsInRegion2)
	{
		std::string path = "/uufs/chpc.utah.edu/common/home/marth-d1/data/WashU_serialTumors/bams/B0.bam";
		BamAlignmentReader::SharedPtr readerPtr = std::make_shared< BamAlignmentReader >(path);
		readerPtr->processAllReadsInBam();
	}
}


#endif //TESTS_BAMALIGNMENTREADERTESTS_HPP
