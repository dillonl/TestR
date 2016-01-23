#include "BamAlignmentReader.h"

#include "parsers/AlignmentParser.hpp"
#include "utils/ThreadPool.hpp"

#include <limits>
#include <cstdlib>

namespace rufus
{
	BamAlignmentReader::BamAlignmentReader(const std::string& filePath, const int regionID) :
		m_file_path(filePath),
		m_region_id(regionID)
	{
	}

	BamAlignmentReader::~BamAlignmentReader()
	{
	}

	std::vector< int > BamAlignmentReader::getAllRegionsInBam(const std::string& filePath)
	{
		std::vector< int > regions;
		BamTools::BamReader bamReader;
		if (!bamReader.Open(filePath))
		{
			throw "Unable to open bam file";
		}
		bamReader.LocateIndex();
		for (auto refData : bamReader.GetReferenceData())
		{
			regions.emplace_back(bamReader.GetReferenceID(refData.RefName));
		}
		bamReader.Close();
		return regions;
	}

	void BamAlignmentReader::processAllReadsInRegion(SparseKmerSet::SharedPtr kmerSetPtr)
	{
		ThreadPool tp;
		uint32_t counter = 0;
		std::cout << "path: " << this->m_file_path << " " << INT_MAX << std::endl;
		BamTools::BamReader bamReader;
		if (!bamReader.Open(this->m_file_path))
		{
			throw "Unable to open bam file";
		}
		bamReader.LocateIndex();
		auto referenceData = bamReader.GetReferenceData();
		bamReader.Close();
		int32_t lastPosition = referenceData[this->m_region_id].RefLength;
		uint32_t intervalSize = 1000000;
		uint32_t currentPosition = 0;
		std::vector< std::shared_ptr< std::future< void > > > futureFunctions;
		while (currentPosition < lastPosition)
		{
			uint32_t tmpPosition = currentPosition + intervalSize - 1;
			uint32_t nextPosition = (tmpPosition > lastPosition) ? lastPosition : tmpPosition;
			auto funct = std::bind(&BamAlignmentReader::processReads, this, currentPosition, nextPosition, kmerSetPtr);
			auto futureFunct = tp.enqueue(funct);
			futureFunctions.emplace_back(futureFunct);
			currentPosition += intervalSize;
		}
		for (auto& futureFunct : futureFunctions)
		{
			futureFunct->wait();
		}
		std::cout << "region finished: " << this->m_region_id << std::endl;
	}

	void BamAlignmentReader::processReads(uint32_t startPosition, uint32_t endPosition, SparseKmerSet::SharedPtr kmerSetPtr)
	{
		int seed = rand() % 50000 + 20000;
		// static std::mutex lock;
		uint32_t counter = 0;
		BamTools::BamReader bamReader;
		if (!bamReader.Open(this->m_file_path))
		{
			throw "Unable to open bam file";
		}
		bamReader.LocateIndex();
		bamReader.SetRegion(this->m_region_id, startPosition, this->m_region_id, endPosition);

		auto bamAlignmentPtr = std::make_shared< BamTools::BamAlignment >();
		// std::vector< InternalKmer > internalKmers;
		InternalKmer* kmerCollection = new InternalKmer[100000];
		size_t kmerCount = 0;
		while(bamReader.GetNextAlignment(*bamAlignmentPtr))
		{
			if (bamAlignmentPtr->Position < startPosition) { continue; }
			auto kmersNumber = (bamAlignmentPtr->Length - KMER_SIZE);
			// if (internalKmers.size() < kmersNumber) { internalKmers.resize(kmersNumber); } // resize if necessary
			// if (AlignmentParser::ParseAlignment(bamAlignmentPtr->QueryBases.c_str(), kmersNumber, internalKmers))
			if (AlignmentParser::ParseAlignment(bamAlignmentPtr->QueryBases.c_str(), kmersNumber, kmerCollection + kmerCount))
			{
				kmerCount += kmersNumber;
				if (kmerCount > seed)
				{
					std::lock_guard< std::mutex > guard(m_lock);
					for (auto i = 0; i < kmerCount; ++i)
					{
						counter++;
						kmerSetPtr->addKmer(kmerCollection[i]);
					}
					kmerCount = 0;

				}
				/*
				// lock.lock();
				for (auto i = 0; i < kmersNumber; ++i) // use the actual kmersNumber, in case the size of internalKmers changes, this way you don't accidentally over count
				{
					++counter;
					kmerSetPtr->addKmer(internalKmers[i]);
				}
				// lock.unlock();
				*/
			}

		}
		bamReader.Close();

		std::cout << "total count: " << counter << ", " << this->m_region_id << std::endl;
	}
}
