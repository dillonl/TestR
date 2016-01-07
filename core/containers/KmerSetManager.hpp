#ifndef RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP
#define RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP

#include "KmerSet.hpp"

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include <boost/noncopyable.hpp>
#include <boost/lockfree/queue.hpp>

namespace rufus
{
	class KmerSetManager : private boost::noncopyable
	{
	public:
		typedef std::shared_ptr< KmerSetManager > SharedPtr;
		KmerSetManager() : m_kmer_queue(0)
		{
		}

		~KmerSetManager()
		{
			stop();
		}

		inline void addKmer(InternalKmer internalKmer)
		{
			m_kmer_queue.push(internalKmer);
			this->m_condition.notify_one();
		}

		void joinAll()
		{
		}

		void stop()
		{

		}

	private:
		void init()
		{
			m_worker_ptr = std::make_shared< std::thread >(&KmerSetManager::start, this);
		}

		void start()
		{
			for (;;)
			{
				std::unique_lock< std::mutex > lock(this->m_queue_lock);
				this->m_condition.wait(lock, [this] { return this->m_stopped || !this->m_kmer_queue.empty(); });
				if (this->m_stopped && this->m_kmer_queue.empty()) { return; }
				while (!this->m_kmer_queue.empty())
				{
					auto internalKmer = 0;
					if ( m_kmer_queue.pop(internalKmer))
					{
						m_kmer_set.addKmer(internalKmer);
					}
				}
			}
		}

		boost::lockfree::queue< InternalKmer > m_kmer_queue;

		std::shared_ptr< std::thread > m_worker_ptr;
		std::mutex m_queue_lock;
		std::condition_variable m_condition;
		bool m_stopped;

		KmerSet m_kmer_set;

	};
}

#endif // RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP
