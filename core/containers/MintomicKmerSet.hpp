#ifndef CORE_CONTAINERS_MINTOMICKMERSET_HPP
#define CORE_CONTAINERS_MINTOMICKMERSET_HPP

#include "KmerHash.hpp"
#include "IKmerSet.h"

#include <memory>
#include <unordered_set>

#include <mintomic/mintomic.h>

namespace rufus
{
	class MintomicKmerSet : public IKmerSet
	{
	public:
        typedef std::shared_ptr< MintomicKmerSet > SharedPtr;
        MintomicKmerSet()
		{
		}

		~MintomicKmerSet()
		{
			// static std::mutex lock;
			// std::lock_guard< std::mutex > guard(lock);
			// std::cout << "set destroyed" << std::endl;
			delete[] m_kmer_set;
			m_total_set_size = 0;
			m_current_set_size = 0;
		}

		//----------------------------------------------
        // from code.google.com/p/smhasher/wiki/MurmurHash3
		inline static uint32_t integerHash(uint64_t h)
		{
			h ^= h >> 16;
			h *= 0x85ebca6b;
			h ^= h >> 13;
			h *= 0xc2b2ae35;
			h ^= h >> 16;
			return h;
		}

		void addKmer(InternalKmer internalKmer) override
		{
			for (uint32_t idx = integerHash(internalKmer);; idx++)
			{
				idx &= m_total_set_size - 1;

				InternalKmer prevKey = mint_compare_exchange_strong_64_relaxed(&m_kmer_set[idx], 0, internalKmer);
				if ((prevKey == 0) || (prevKey == internalKmer))
				{
					mint_store_64_relaxed(&m_kmer_set[idx], internalKmer);
					/*
					mint_store_64_relaxed(&m_entries[idx].value, value);
					return;
					*/
					return;
				}
			}
			/*
			auto iter = m_kmer_set.find(internalKmer);
			if (iter == m_kmer_set.end())
			{
				internalKmer = (internalKmer & 0x0003FFFFFFFFFFFF);
				iter = m_kmer_set.emplace(internalKmer).first;
				// iter = m_kmer_set.find(internalKmer);
			}
			auto iterPtr = const_cast< InternalKmer* >(&(*iter));
			*iterPtr = (internalKmer | ((((*iter) >> KMER_SHIFTER_SIZE) + KMER_COUNT_INC) << KMER_SHIFTER_SIZE));
			*/
		}

		uint64_t getKmerCount(InternalKmer internalKmer) override
		{
			/*
			auto iter = m_kmer_set.find(internalKmer);
			return (iter != m_kmer_set.end()) ? ((*iter) >> KMER_SHIFTER_SIZE) : 0;
			*/
			return 0;
		}

		size_t getSetSize() override
		{
			return m_current_set_size;
		}

		void addAllKmersToPassedInSet(IKmerSet::SharedPtr kmerSetPtr) override
		{
		}

		void resize(uint64_t size) override
		{
			m_kmer_set = new mint_atomic64_t[size];
			memset(m_kmer_set, 0, sizeof(mint_atomic64_t) * size);
			m_total_set_size = size;
			m_current_set_size = size;
		}

	private:
		// std::unordered_set< InternalKmer, KmerHash, KmerKeyEqual > m_kmer_set;
		mint_atomic64_t* m_kmer_set;
		size_t m_total_set_size;
		size_t m_current_set_size;

	};
}

#endif // CORE_CONTAINERS_MINTOMICKMERSET_HPP
