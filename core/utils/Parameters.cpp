#include "Parameters.h"

namespace rufus
{
	Parameters::Parameters()
	{
	}

	Parameters::~Parameters()
	{
	}

	void Parameters::parseRUFUS(int argc, char** argv)
	{
		m_options_description_ptr = std::make_shared< boost::program_options::options_description >("options");
		/*
		m_options_description_ptr->add_options()
			("help,h","Print help message")
			(",b", boost::program_options::value< std::vector< std::string > >()->required()->multitoken(), "Path to input BAM file[s], separate multiple files by space")
			(",r", boost::program_options::value< std::string >()->required(), "Region information")
			(",v", boost::program_options::value< std::vector< std::string > >()->required()->multitoken(), "Path to input VCF file[s], separate multiple files by space")
			(",o", boost::program_options::value< std::string >()->default_value(""), "Path to output directory [optional - if not provided then prints to std::cout]")
			(",f", boost::program_options::value< std::string >()->required(), "Path to input FASTA file")
			(",p", boost::program_options::value< uint32_t >()->default_value(90), "Smith-Waterman Percent [optional - default is 90]")
			(",m", boost::program_options::value< uint32_t >()->default_value(1), "Smith-Waterman Match Value [optional - default is 1]")
			(",s", boost::program_options::value< uint32_t >()->default_value(4), "Smith-Waterman MisMatch Value [optional - default is 4]")
			(",a", boost::program_options::value< uint32_t >()->default_value(6), "Smith-Waterman Gap Open Value [optional - default is 6]")
			(",e", boost::program_options::value< uint32_t >()->default_value(1), "Smith-Waterman Gap Extension Value [optional - default is 1]")
			(",g", boost::program_options::value< uint32_t >()->default_value(3000), "The size of the graph [optional - default is 3000]")
			(",t", boost::program_options::value< uint32_t >()->default_value(std::thread::hardware_concurrency() * 2), "Thread count [optional - default is number of cores x 2]");
		auto parseCommandLine = boost::program_options::parse_command_line(argc, argv, *m_options_description_ptr);
		boost::program_options::store(parseCommandLine, m_variables_map);
		*/
	}
}
