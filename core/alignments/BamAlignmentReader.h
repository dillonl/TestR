#ifndef RUFUS_ALIGNMENTS_BAMALIGNMENTREADER_HPP
#define RUFUS_ALIGNMENTS_BAMALIGNMENTREADER_HPP

#include "api/BamReader.h"
#include "api/BamAlignment.h"

#include "IAlignmentReader.h"
#include "utils/Noncopyable.hpp"

namespace rufus
{
	class BamAlignmentReader : private Noncopyable
	{
	public:
        typedef std::shared_ptr< BamAlignmentReader > SharedPtr;
        BamAlignmentReader(const std::string& filePath, const int regionID, KmerSetManager::SharedPtr kmerSetManager);
		~BamAlignmentReader();

		void processAllReadsInRegion(KmerSetManager::SharedPtr kmerSetManager);

	private:
		std::string m_file_path;
		int m_region_id;
		KmerSetManager::SharedPtr m_kmer_set_manager;
        BamTools::BamReader m_bam_reader;
	};
}

#endif //GRAPHITE_BAMALIGNMENTREADER_HPP
