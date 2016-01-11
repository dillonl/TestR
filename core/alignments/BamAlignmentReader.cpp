#include "BamAlignmentReader.h"

#include "parsers/AlignmentParser.hpp"

#include <limits>

namespace rufus
{
	BamAlignmentReader::BamAlignmentReader(const std::string& filePath, const int regionID, KmerSetManager::SharedPtr kmerSetManager) :
		m_file_path(filePath),
		m_region_id(regionID),
		m_kmer_set_manager(kmerSetManager)
	{
	}

	BamAlignmentReader::~BamAlignmentReader()
	{
	}

	void BamAlignmentReader::processAllReadsInRegion(KmerSetManager::SharedPtr kmerSetManager)
	{
		if (!this->m_bam_reader.Open(this->m_file_path))
		{
			throw "Unable to open bam file";
		}
		this->m_bam_reader.LocateIndex();
		this->m_bam_reader.SetRegion(this->m_region_id, 0, this->m_region_id, std::numeric_limits< int >::max());

		auto bamAlignmentPtr = std::make_shared< BamTools::BamAlignment >();
		std::vector< InternalKmer > internalKmers;
		while(this->m_bam_reader.GetNextAlignment(*bamAlignmentPtr))
		{
			if (bamAlignmentPtr->RefID != this->m_region_id) { break; }
			auto kmersNumber = (bamAlignmentPtr->Length - KMER_SIZE);
			if (internalKmers.size() < kmersNumber) { internalKmers.resize(kmersNumber); } // resize if necessary
			if (AlignmentParser::ParseAlignment(bamAlignmentPtr->QueryBases.c_str(), kmersNumber, internalKmers))
			{
				for (auto i = 0; i < kmersNumber; ++i) // use the actual kmersNumber, in case the size of internalKmers changes, this way you don't accidentally over count
				{
					this->m_kmer_set_manager->addKmer(internalKmers[i]);
				}
			}
		}
	}
}
