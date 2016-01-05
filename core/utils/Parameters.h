#ifndef RUFUS_CORE_PARAMETERS_H
#define RUFUS_CORE_PARAMETERS_H

#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/program_options.hpp>

namespace rufus
{
	class Parameters : private boost::noncopyable
	{
	public:
		Parameters();
		~Parameters();

		void parseRUFUS(int argc, char** argv);
		bool showHelp();
		void printHelp();
		bool validateRequired();

	private:
		std::shared_ptr< boost::program_options::options_description > m_options_description_ptr;
		boost::program_options::variables_map m_variables_map;
	};
}

#endif //RUFUS_CORE_PARAMETERS_H
