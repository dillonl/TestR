#ifndef CORE_HASHERS_KMERHASH_HPP
#define CORE_HASHERS_KMERHASH_HPP

#include "utils/Types.h"

namespace rufus
{
	class KmerHash
	{
	public:
		size_t operator()(const InternalKmer& kmer) const
		{
			std::cout << "hashed: " << (kmer & 0x0003FFFFFFFFFFFF) << std::endl;
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

#endif // CORE_HASHERS_KMERHASH_HPP
