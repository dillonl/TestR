#ifndef RUFUS_UTILS_NONCOPYABLE_HPP
#define RUFUS_UTILS_NONCOPYABLE_HPP

namespace rufus
{
	class Noncopyable
	{
	public:
		Noncopyable( Noncopyable const & ) = delete;
		Noncopyable& operator=( Noncopyable const & ) = delete;

		Noncopyable() = default;
	};
}

#endif //RUFUS_UTILS_NONCOPYABLE_HPP
