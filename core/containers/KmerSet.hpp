#ifndef CORE_CONTAINERS_KMERSET_HPP
#define CORE_CONTAINERS_KMERSET_HPP

#include "KmerHash.hpp"
#include "IKmerSet.h"

#include <memory>
#include <unordered_set>

namespace rufus
{
	class KmerSet : public IKmerSet
	{
	public:
		typedef std::shared_ptr< KmerSet > SharedPtr;
		KmerSet()
		{
		}

		~KmerSet()
		{
		}

		void addKmer(InternalKmer internalKmer) override
		{
			auto iter = m_kmer_set.find(internalKmer);
			if (iter == m_kmer_set.end())
			{
				internalKmer = (internalKmer & 0x0003FFFFFFFFFFFF);
				m_kmer_set.emplace(internalKmer);
				iter = m_kmer_set.find(internalKmer);
			}
			auto iterPtr = const_cast< InternalKmer* >(&(*iter));
			*iterPtr = (internalKmer | ((((*iter) >> KMER_SHIFTER_SIZE) + KMER_COUNT_INC) << KMER_SHIFTER_SIZE));
		}

		uint64_t getKmerCount(InternalKmer internalKmer) override
		{
			auto iter = m_kmer_set.find(internalKmer);
			return (iter != m_kmer_set.end()) ? ((*iter) >> KMER_SHIFTER_SIZE) : 0;
		}

		size_t getSetSize() override
		{
			return m_kmer_set.size();
		}

	private:
		std::unordered_set< InternalKmer, KmerHash, KmerKeyEqual > m_kmer_set;

	};
}

#endif // CORE_CONTAINERS_KMERSET_HPP
