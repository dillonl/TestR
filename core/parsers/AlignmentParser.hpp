#ifndef CORE_PARSERS_ALIGNMENTPARSER_H
#define CORE_PARSERS_ALIGNMENTPARSER_H

#include "utils/Types.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <iostream>
#include <bitset>
#include <unordered_map>

namespace rufus
{

	class AlignmentParser : private boost::noncopyable
	{
	public:
		template< uint32_t KMER_SIZE = 25 >
		static inline bool ParseAlignment(const char* alignment, size_t alignmentLength, std::unordered_set< InternalKmer, KmerHash, KmerKeyEqual >* kSet)
		{
			std::cout << "addr: " << kSet << std::endl;
			auto kmerIterations = (alignmentLength - KMER_SIZE);
			for (auto i = 0; i <= kmerIterations; ++i)
			{
				InternalKmer internalKmer = 0; // we must guarentee internalKmer is zero
				bool validKmer = true;
				validKmer &= unroller(i, uint_< KMER_SIZE - 1 >(),  alignment, internalKmer);

				std::string kmer = std::string(alignment + i, 25);
				auto iter = kSet->find(internalKmer);
				if (iter != kSet->end())
				{
					uint64_t fiftyShift = 50;
					auto iterPtr = const_cast< InternalKmer* >(&(*iter));
					uint64_t count = (((*iter) >> fiftyShift) + std::uint64_t(1));
					*iterPtr = internalKmer | (count << fiftyShift);
					iter = kSet->find(internalKmer);
					std::cout << "found: " << kmer << " " << std::bitset< 64 >(*iter) << " " << std::bitset< 8 >(count) << " " << count << std::endl;
				}
				else
				{
					// std::cout << "k1:    " << kmer << " " << std::bitset< 64 >(internalKmer) << " " << internalKmer << std::endl;
					kSet->emplace(internalKmer);
					iter = kSet->find(internalKmer);
					auto iterPtr = const_cast< InternalKmer* >(&(*iter));
					*iterPtr = internalKmer | 0x0004000000000000;
					// std::cout << "k2:    " << kmer << " " << std::bitset< 64 >(*iter) << std::endl;
					iter = kSet->find(internalKmer);
					std::cout << "k3:    " << kmer << " " << std::bitset< 64 >(*iter) << " " << internalKmer << std::endl;
				}
				if (!validKmer) { return false;	}
			}
			std::cout << "---" << std::endl;
			return true;
		}

	private:
		static const uint64_t shifter1 = 1; // these need to be 64 bit ints
		static const uint64_t shifter3 = 3;

		AlignmentParser() = delete;
		~AlignmentParser() = delete;

		template <size_t N> struct uint_{ };

		template <size_t N, typename IterT>
		static inline bool unroller(const IterT& iter, uint_<N>, const char* alignment, InternalKmer& internalKmer)
		{
			unroller(iter, uint_<N-1>(), alignment, internalKmer);
			// return parse(alignment, internalKmer, iter + N);
			return parse(alignment, internalKmer, N, iter);
		}

		template <typename IterT>
		static inline bool unroller(const IterT& iter, uint_<0>, const char* alignment, InternalKmer& internalKmer)
		{
			// return f(alignment, internalKmer, iter + 0);
			return parse(alignment, internalKmer, 0, iter);
		}

		static inline bool parse(const char* alignmentCharacter, InternalKmer& kmer, uint64_t kmerBitOffset, uint64_t counter)
		{
			uint64_t shifter = (kmerBitOffset * 2);
			// std::cout << "offset: " << alignmentCharacter[kmerBitOffset + counter] << " " << shifter << " " << counter << " " << std::bitset< 8 > (((alignmentCharacter[kmerBitOffset + counter] >> shifter1) & 0x3)) << std::endl;
			if ((alignmentCharacter[kmerBitOffset + counter]) & (1 >> (3))) { return false; } // if this is a non basepair char
			kmer |= (((alignmentCharacter[kmerBitOffset + counter] >> shifter1) & 0x3) & shifter3) << shifter;
			return true;
		}
	};
}

#endif //CORE_PARSERS_ALIGNMENTPARSER_H
