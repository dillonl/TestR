#ifndef RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP
#define RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP

#include "SparseKmerSet.hpp"
#include "utils/ThreadPool.hpp"

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include <concurrentqueue.h>

#include <boost/noncopyable.hpp>

namespace rufus
{
	class KmerSetManager : private boost::noncopyable, public std::enable_shared_from_this< KmerSetManager >
	{
	public:
		typedef std::shared_ptr< KmerSetManager > SharedPtr;
		KmerSetManager(ThreadPool::SharedPtr threadPoolPtr) :
			m_kmer_queue(0),
			m_stopped(false),
			m_thread_pool(threadPoolPtr)
		{
			m_kmer_set = std::make_shared< SparseKmerSet >();
			init();
			m_thread_pool->registerStoppingFunction([&]()
													{
														shared_from_this()->stop();
													});
		}

		~KmerSetManager()
		{
			if (!m_stopped)
			{
				stop();
			}
		}

		inline void addKmer(InternalKmer internalKmer)
		{
			if (!m_stopped)
			{
				m_kmer_queue.enqueue(internalKmer);
				this->m_condition.notify_one();
			}
		}

		void stop()
		{
			// std::cout << "stopping kmer 1" << std::endl;
			// std::unique_lock< std::mutex > lock(this->m_queue_lock);
			if (m_stopped) { return; }
			// std::cout << "stopping kmer 2 " << std::endl;
			m_stopped = true;
			this->m_condition.notify_one();
		}

		SparseKmerSet::SharedPtr getKmerSet()
		{
			return m_kmer_set;
		}

	private:
		void init()
		{
			start();
		}

		void start()
		{
			m_thread_pool->enqueue([this]()
			{
				auto internalKmer = 0;
				for (;;)
				{
					std::unique_lock< std::mutex > lock(this->m_queue_lock);
					this->m_condition.wait(lock, [this] { return this->m_stopped || (this->m_kmer_queue.size_approx() > 0); });
					if (this->m_stopped && (this->m_kmer_queue.size_approx() == 0)) { return; }
					while (this->m_kmer_queue.size_approx() > 0)
					{
						if (m_kmer_queue.try_dequeue(internalKmer))
						{
							m_kmer_set->addKmer(internalKmer);
						}
					}
				}
			});
		}

		// boost::lockfree::queue< InternalKmer > m_kmer_queue;
		moodycamel::ConcurrentQueue< InternalKmer > m_kmer_queue;

		std::mutex m_queue_lock;
		std::condition_variable m_condition;
		std::atomic< bool > m_stopped;

		SparseKmerSet::SharedPtr m_kmer_set;
		ThreadPool::SharedPtr m_thread_pool;

	};
}

#endif // RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP
