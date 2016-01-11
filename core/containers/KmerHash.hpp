#ifndef CORE_CONTAINERS_KMERHASH_HPP
#define CORE_CONTAINERS_KMERHASH_HPP

#include "utils/Types.h"

#include <memory>

#include <boost/noncopyable.hpp>

namespace rufus
{
	class KmerHash
	{
	public:
		size_t operator()(const InternalKmer& kmer) const
			{
				return kmer & 0x0003FFFFFFFFFFFF;
			}
	};

	class KmerKeyEqual
	{
	public:
		bool operator()(const InternalKmer& a, const InternalKmer& b) const
			{
				return (a & 0x0003FFFFFFFFFFFF) == (b & 0x0003FFFFFFFFFFFF);
			}
	};
}
#endif  //CORE_CONTAINERS_KMERHASH_HPP
