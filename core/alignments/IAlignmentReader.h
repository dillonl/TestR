#ifndef RUFUS_ALIGNMENTS_IALIGNMENTREADER_HPP
#define RUFUS_ALIGNMENTS_IALIGNMENTREADER_HPP

#include "containers/KmerSetManager.hpp"

#include <memory>

#include <boost/noncopyable.hpp>

namespace rufus
{
	class IAlignmentReader : private boost::noncopyable
	{
	public:
		typedef std::shared_ptr< IAlignmentReader > SharedPtr;
		IAlignmentReader() {}
		virtual ~IAlignmentReader() {}

		virtual void processAllReadsInRegion(KmerSetManager::SharedPtr kmerSetManager) = 0;
	};
}

#endif //GRAPHITE_IALIGNMENTREADER_HPP
