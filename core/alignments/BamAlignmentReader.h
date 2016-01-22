#ifndef RUFUS_ALIGNMENTS_BAMALIGNMENTREADER_HPP
#define RUFUS_ALIGNMENTS_BAMALIGNMENTREADER_HPP

#include "api/BamReader.h"
#include "api/BamAlignment.h"

#include "IAlignmentReader.h"
#include "containers/KmerSetManager.hpp"

#include <boost/noncopyable.hpp>

namespace rufus
{
	class BamAlignmentReader : private boost::noncopyable
	{
	public:
        typedef std::shared_ptr< BamAlignmentReader > SharedPtr;
        BamAlignmentReader(const std::string& filePath, const int regionID);
		~BamAlignmentReader();

		void processAllReadsInRegion(KmerSetManager::SharedPtr kmerSetManager);

		static std::vector< int > getAllRegionsInBam(const std::string& filePath);

	private:
		void processReads(uint32_t startPosition, uint32_t endPosition, KmerSetManager::SharedPtr kmerSetManager);

		std::string m_file_path;
		int m_region_id;
	};
}

#endif //GRAPHITE_BAMALIGNMENTREADER_HPP
