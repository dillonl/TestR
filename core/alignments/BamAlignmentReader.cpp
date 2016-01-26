#include "BamAlignmentReader.h"

#include "containers/KmerSet.hpp"
#include "parsers/AlignmentParser.hpp"
#include "utils/ThreadPool.hpp"

#include <limits>
#include <cstdlib>

namespace rufus
{
	BamAlignmentReader::BamAlignmentReader(const std::string& filePath) :
		m_file_path(filePath)
	{
	}

	BamAlignmentReader::~BamAlignmentReader()
	{
	}

	std::vector< BamAlignmentReader::BamRegion::SharedPtr > BamAlignmentReader::getAllSpacedOutRegions()
	{
		BamTools::BamReader bamReader;
		if (!bamReader.Open(this->m_file_path))
		{
			throw "Unable to open bam file";
		}
		bamReader.LocateIndex();

		// get all region ids
		std::vector< int > regionIDs;
		for (auto refData : bamReader.GetReferenceData())
		{
			regionIDs.emplace_back(bamReader.GetReferenceID(refData.RefName));
		}

		auto referenceData = bamReader.GetReferenceData();
		// get the region pointers
		std::vector< BamRegion::SharedPtr > regionPtrs;
		uint32_t intervalSize = 1000000;
		for (auto regionID : regionIDs)
		{
			int32_t regionLastPosition = referenceData[regionID].RefLength;
			uint32_t currentPosition = 0;
			while (currentPosition < regionLastPosition)
			{
				uint32_t endPosition = ((currentPosition + intervalSize) > regionLastPosition) ? regionLastPosition : currentPosition + intervalSize;
				if (endPosition != regionLastPosition && endPosition + 10000 > regionLastPosition) { endPosition = regionLastPosition; } // so we don't end up with small regions near the end of the bamregion
				auto bamRegionPtr = std::make_shared< BamRegion >(regionID, currentPosition, endPosition);
				regionPtrs.emplace_back(bamRegionPtr);
				currentPosition += endPosition;
			}
		}

		bamReader.Close();
		return regionPtrs;
	}

	void BamAlignmentReader::processAllReadsInBam()
	{
		ThreadPool tp;
		auto spacedOutRegions = getAllSpacedOutRegions();
		std::vector< std::shared_ptr< std::future< IKmerSet::SharedPtr > > > futureFunctions;
		for (auto regionPtr : spacedOutRegions)
		{
			// regionPtr->print();
			auto funct = std::bind(&BamAlignmentReader::processReads, this, regionPtr);
			auto futureFunct = tp.enqueue(funct);
			futureFunctions.emplace_back(futureFunct);
		}
		for (auto& futureFunct : futureFunctions)
		{
			futureFunct->wait();
		}
	}

	IKmerSet::SharedPtr BamAlignmentReader::processReads(BamRegion::SharedPtr bamRegionPtr)
	{
		// static std::mutex lock;
		// std::lock_guard< std::mutex > guard(lock);
		// std::cout << "locked" << std::endl;

		// SparseKmerSet::SharedPtr kmerSetPtr = std::make_shared< SparseKmerSet >();
		KmerSet::SharedPtr kmerSetPtr = std::make_shared< KmerSet >();
		// int seed = 99900; //rand() % 50000 + 20000;
		uint32_t counter = 0;
		BamTools::BamReader bamReader;
		if (!bamReader.Open(this->m_file_path))
		{
			throw "Unable to open bam file";
		}
		bamReader.LocateIndex();
		bamReader.SetRegion(bamRegionPtr->getRegionID(), bamRegionPtr->getStartPosition(), bamRegionPtr->getRegionID(), bamRegionPtr->getEndPosition());

		auto bamAlignmentPtr = std::make_shared< BamTools::BamAlignment >();
		std::vector< InternalKmer > internalKmers(100);
		// InternalKmer kmerCollection[10000];
		// InternalKmer* kmerCollection = new InternalKmer(100000);
		size_t kmerCount = 0;
		// std::cout << "getting alignment" << std::endl;

		while(bamReader.GetNextAlignment(*bamAlignmentPtr))
		{
			if (bamAlignmentPtr->Position < bamRegionPtr->getStartPosition()) { continue; }
			auto kmersNumber = (bamAlignmentPtr->Length - KMER_SIZE);
			if (kmersNumber > internalKmers.size()) { internalKmers.resize(kmersNumber); }
			if (AlignmentParser::ParseAlignment(bamAlignmentPtr->QueryBases.c_str(), kmersNumber, internalKmers))
			// if (AlignmentParser::ParseAlignment(bamAlignmentPtr->QueryBases.c_str(), kmersNumber, kmerCollection + kmerCount))
			{
				for (auto i = 0; i < kmersNumber; ++i)
				{
					kmerSetPtr->addKmer(internalKmers[i]);
                    ++counter;
				}
				/*
				kmerCount += kmersNumber;
				if (kmerCount > seed)
				{
					for (auto i = 0; i < kmerCount; ++i)
					{
						counter++;
						// kmerSetPtr->addKmer(kmerCollection[i]);
						kmerSetPtr->addKmer(internalKmers[i]);
					}
					kmerCount = 0;
				}
				*/
			}
		}
		bamReader.Close();

		std::cout << "total count: " << counter << " ";
        bamRegionPtr->print();
		return kmerSetPtr;
	}
}
