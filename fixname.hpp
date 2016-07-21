#ifndef FIXNAME_HPP
#define FIXNAME_HPP

#include <string>
#include <stdexcept>
#include "inode.hpp"
#include "directory.hpp"

#define FLG_ALL			 1
#define FLG_DIRECTORY	 2
#define FLG_RECURSIVE	 4
#define FLG_FORCE		 8
#define FLG_IGNORE		 16
#define FLG_VERBOSE		 32

#define COLOR_RED		"\e[31;1m"
#define COLOR_GREEN		"\e[32;1m"
#define COLOR_YELLOW	"\e[33;1m"
#define COLOR_CYAN		"\e[36;1m"
#define COLOR_NORMAL	"\e[0m"

namespace fixname
{
	struct Statistics{
		int filesOk;
		int filesRenamed;
		int filesIgnored;
		int filesError;
		int totalFiles;
	};

	typedef int Flags;

	std::string setColor( std::string text, std::string ansiColor );

	class FixName
	{
	public:
		FixName( std::string path, Flags flag )
			throw( std::invalid_argument, std::runtime_error );
		~FixName();

		Statistics fix();
		static Statistics getStatistics();

	private:
		void scanDir( Directory &d );
		std::string getValidName( std::string fileName );
		bool promptFile( std::string fileName );
		void fixiNode( iNode &node );
		void verbose( std::string text, bool error = false );

		iNode *mainNode;
		Flags flags;
		static Statistics stats;
	};

} // end of namespace



#endif
