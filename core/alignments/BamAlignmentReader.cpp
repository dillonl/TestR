#include "BamAlignmentReader.h"

#include "parsers/AlignmentParser.hpp"

#include <limits>

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

	void BamAlignmentReader::processAllReadsInRegion(KmerSetManager::SharedPtr kmerSetManager)
	{
		uint32_t counter = 0;
		std::cout << "path: " << this->m_file_path << " " << INT_MAX << std::endl;
		if (!this->m_bam_reader.Open(this->m_file_path))
		{
			throw "Unable to open bam file";
		}
		this->m_bam_reader.LocateIndex();
		std::cout << "path: " << std::endl;
		// this->m_bam_reader.SetRegion(this->m_region_id, 0, this->m_region_id, std::numeric_limits< int >::max());
		auto referenceData = m_bam_reader.GetReferenceData();
		int32_t lastPosition = referenceData[this->m_region_id].RefLength;
		std::cout << "position: " << lastPosition << std::endl;
		this->m_bam_reader.SetRegion(this->m_region_id, 0, this->m_region_id, lastPosition);
		std::cout << "region set" << std::endl;

		auto bamAlignmentPtr = std::make_shared< BamTools::BamAlignment >();
		std::vector< InternalKmer > internalKmers;
		/*
		while(this->m_bam_reader.GetNextAlignment(*bamAlignmentPtr))
		{
			// std::cout << "alignment gotten!" << std::endl;
			if (bamAlignmentPtr->RefID != this->m_region_id) { break; }
			auto kmersNumber = (bamAlignmentPtr->Length - KMER_SIZE);
			if (internalKmers.size() < kmersNumber) { internalKmers.resize(kmersNumber); } // resize if necessary
			if (AlignmentParser::ParseAlignment(bamAlignmentPtr->QueryBases.c_str(), kmersNumber, internalKmers))
			{
				// std::cout << "alignment parsed!" << std::endl;
				for (auto i = 0; i < kmersNumber; ++i) // use the actual kmersNumber, in case the size of internalKmers changes, this way you don't accidentally over count
				{
					++counter;
					// std::cout << "adding kmer!" << std::endl;
					kmerSetManager->addKmer(internalKmers[i]);
					// std::cout << "kmer added!" << std::endl;
				}
				// if (counter > 500) { break; }
			}
		}
		*/
		this->m_bam_reader.Close();
		std::cout << "total count: " << counter << std::endl;
	}
}
