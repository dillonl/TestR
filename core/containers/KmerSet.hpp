#ifndef CORE_CONTAINERS_KMERSET_HPP
#define CORE_CONTAINERS_KMERSET_HPP

#include "KmerHash.hpp"
#include "utils/Types.h"
#include "utils/Noncopyable.hpp"

#include <memory>
#include <unordered_set>

namespace rufus
{
	class KmerSet : private Noncopyable
	{
	public:
		typedef std::shared_ptr< KmerSet > SharedPtr;
		KmerSet()
		{
		}

		~KmerSet()
		{
		}

		void addKmer(InternalKmer internalKmer)
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

		uint64_t getKmerCount(InternalKmer internalKmer)
		{
			auto iter = m_kmer_set.find(internalKmer);
			return (iter != m_kmer_set.end()) ? ((*iter) >> KMER_SHIFTER_SIZE) : 0;
		}

		size_t getSetSize()
		{
			return m_kmer_set.size();
		}

	private:
		std::unordered_set< InternalKmer, KmerHash, KmerKeyEqual > m_kmer_set;

	};
}

#endif // CORE_CONTAINERS_KMERSET_HPP
