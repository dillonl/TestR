#ifndef RUFUS_ALIGNMENTS_IALIGNMENTREADER_HPP
#define RUFUS_ALIGNMENTS_IALIGNMENTREADER_HPP

#include "containers/KmerSetManager.hpp"
#include "utils/Noncopyable.hpp"

#include <memory>

namespace rufus
{
	class IAlignmentReader : private Noncopyable
	{
	public:
		typedef std::shared_ptr< IAlignmentReader > SharedPtr;
		IAlignmentReader() {}
		virtual ~IAlignmentReader() {}

		virtual void processAllReadsInRegion(KmerSetManager::SharedPtr kmerSetManager) = 0;
	};
}

#endif //GRAPHITE_IALIGNMENTREADER_HPP
