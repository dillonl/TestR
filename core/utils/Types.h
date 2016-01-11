#ifndef CORE_UTILS_TYPES_H
#define CORE_UTILS_TYPES_H

#include <cstdint>

namespace rufus
{
	typedef uint64_t InternalKmer;
	typedef uint32_t position;

#define KMER_SIZE 25ULL
#define KMER_SHIFTER_SIZE (KMER_SIZE * 2)
#define KMER_COUNT_INC 1ULL
}

#endif //CORE_UTILS_TYPES_H
