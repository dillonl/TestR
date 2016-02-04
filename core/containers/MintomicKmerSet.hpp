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
			mint_store_64_relaxed(&m_current_set_size, 0);
		}

		//----------------------------------------------
        // from code.google.com/p/smhasher/wiki/MurmurHash3
		inline static uint32_t integerHash(uint64_t h)
		{
			h ^= h >> 33;
			h *= 0xff51afd7ed558ccd;
			h ^= h >> 33;
			h *= 0xc4ceb9fe1a85ec53;
			h ^= h >> 33;
			return h;
		}

		void addKmer(InternalKmer key) override
		{
			/*
			for (uint32_t idx = integerHash(internalKmer);; idx++)
			{
				idx &= m_total_set_size - 1;
				std::cout << "idx: " << idx << std::endl;

				InternalKmer prevKey = mint_compare_exchange_strong_64_relaxed(&m_kmer_set[idx], 0, internalKmer);
				if ((prevKey == 0) || (prevKey == internalKmer))
				{
					// mint_store_64_relaxed(&m_kmer_set[idx], internalKmer);
					// mint_store_64_relaxed(&m_current_set_size, mint_load_64_relaxed(&m_current_set_size) + 1);
					return;
				}
			}
			*/

			for (uint64_t idx = integerHash(key);; ++idx)
			{
				// idx &= m_arraySize - 1;
				idx &= m_total_set_size - 1;
				if (idx >= m_total_set_size)
				{
					std::cout << "idx too large: " << idx << std::endl;
				}

				// Load the key that was there.
				uint64_t probedKey = mint_load_64_relaxed(&m_kmer_set[idx]);
				if (probedKey != key)
				{
					// The entry was either free, or contains another key.
					if (probedKey != 0)
					{
						std::cout << "freed  memory or contains key: " << probedKey << std::endl;
						continue;           // Usually, it contains another key. Keep probing.
					}
					// The entry was free. Now let's try to take it using a CAS.
					uint64_t prevKey = mint_compare_exchange_strong_64_relaxed(&m_kmer_set[idx], 0, key);
					if ((prevKey != 0) && (prevKey != key))
					{
						std::cout << "Stolen!!!" << std::endl;
						continue;       // Another thread just stole it from underneath us.
					}

					// Either we just added the key, or another thread did.
					// Store the value in this array entry.
					mint_store_64_relaxed(&m_kmer_set[idx], key);
					mint_store_64_relaxed(&m_current_set_size, mint_load_64_relaxed(&m_current_set_size) + 1);
				}
				return;
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
			return mint_load_64_relaxed(&m_current_set_size);
		}

		void addAllKmersToPassedInSet(IKmerSet::SharedPtr kmerSetPtr) override
		{
		}

		void resize(uint64_t size) override
		{
			m_kmer_set = new mint_atomic64_t[size];
		    memset(m_kmer_set, 0, sizeof(mint_atomic64_t) * size);
			m_total_set_size = size;
			mint_store_64_relaxed(&m_current_set_size, 0);
			// m_current_set_size = size;
		}

	private:
		// std::unordered_set< InternalKmer, KmerHash, KmerKeyEqual > m_kmer_set;
		mint_atomic64_t* m_kmer_set;
		size_t m_total_set_size;
		mint_atomic64_t m_current_set_size;

	};
}

#endif // CORE_CONTAINERS_MINTOMICKMERSET_HPP
