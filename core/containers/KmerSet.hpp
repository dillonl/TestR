#ifndef CORE_CONTAINERS_KMERSET_HPP
#define CORE_CONTAINERS_KMERSET_HPP

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

	class KmerSet : private boost::noncopyable
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
