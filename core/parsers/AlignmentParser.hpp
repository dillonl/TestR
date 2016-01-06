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
		static inline bool ParseAlignment(const char* alignment, size_t alignmentLength)
		{
			InternalKmer internalKmer = 0; // we must guarentee internalKmer is zero
			/*
			auto charParser = [&](const char* alignmentCharacter, InternalKmer& kmer, uint64_t kmerBitOffset) -> bool
			{


				// uint64_t a = (alignmentCharacter[kmerBitOffset] >> 1);
				//uint64_t b = ((alignmentCharacter[kmerBitOffset] >> 1) & 3) << (kmerBitOffset * 2);
				uint64_t shifter = (kmerBitOffset * 2);
				if ((alignmentCharacter[kmerBitOffset]) & (1 >> (3))) { return false; } // if this is a non basepair char
				kmer |= ((alignmentCharacter[kmerBitOffset] >> shifter1) & shifter3) << shifter;

				// std::cout << "shifting by: " << kmerBitOffset * 2 << std::endl;
				// std::cout << "test1: " << alignmentCharacter[kmerBitOffset] << " " << std::bitset< 64 >(alignmentCharacter[kmerBitOffset]) << std::endl;
				//std::cout << "test2: " << alignmentCharacter[kmerBitOffset] << " " << std::bitset< 8 >(a) << std::endl;
				// std::cout << "test3: " << alignmentCharacter[kmerBitOffset] << " " << std::bitset< 64 >(kmer) << std::endl;
				// std::cout << std::endl;
				return true;
			};
			*/
			std::vector< InternalKmer > kmers;
			auto kmerIterations = (alignmentLength - KMER_SIZE);
			for (auto i = 0; i <= kmerIterations; ++i)
			{
				bool validKmer = true;
				// validKmer &= unroller(charParser, i, uint_< KMER_SIZE - 1 >(),  alignment, internalKmer);
				validKmer &= unroller(i, uint_< KMER_SIZE - 1 >(),  alignment, internalKmer);
				kmers.push_back(internalKmer);
				// queue.push(internalKmer);
				// std::cout << validKmer << " " << "----" << std::endl;
				if (!validKmer) { return false;	}
			}
			return true;
		}

	private:
		static const uint64_t shifter1 = 1; // these need to be 64 bit ints
		static const uint64_t shifter3 = 3;

		AlignmentParser() = delete;
		~AlignmentParser() = delete;

		template <size_t N> struct uint_{ };

		// template <size_t N, typename Lambda, typename IterT>
		// static inline bool unroller(const Lambda& f, const IterT& iter, uint_<N>, const char* alignment, InternalKmer& internalKmer)
		template <size_t N, typename IterT>
		static inline bool unroller(const IterT& iter, uint_<N>, const char* alignment, InternalKmer& internalKmer)
		{
			// unroller(f, iter, uint_<N-1>(), alignment, internalKmer);
			unroller(iter, uint_<N-1>(), alignment, internalKmer);
			// return f(alignment, internalKmer, iter + N);
			return parse(alignment, internalKmer, iter + N);
		}

		// template <typename Lambda, typename IterT>
		// static inline bool unroller(const Lambda& f, const IterT& iter, uint_<0>, const char* alignment, InternalKmer& internalKmer)
		template <typename IterT>
		static inline bool unroller(const IterT& iter, uint_<0>, const char* alignment, InternalKmer& internalKmer)
		{
			// return f(alignment, internalKmer, iter + 0);
			return parse(alignment, internalKmer, iter + 0);
		}

		static inline bool parse(const char* alignmentCharacter, InternalKmer& kmer, uint64_t kmerBitOffset)
		{
			uint64_t shifter = (kmerBitOffset * 2);
			if ((alignmentCharacter[kmerBitOffset]) & (1 >> (3))) { return false; } // if this is a non basepair char
			kmer |= ((alignmentCharacter[kmerBitOffset] >> shifter1) & shifter3) << shifter;
			return true;
		}
	};
}

#endif //CORE_PARSERS_ALIGNMENTPARSER_H
