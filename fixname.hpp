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

	class FixName
	{
	public:
		FixName( std::string path, Flags flag )
			throw( std::invalid_argument, std::runtime_error );
		~FixName();

		Statistics fix();

	private:
		void scanDir( Directory &d );
		std::string getValidName( std::string fileName );
		bool promptFile( std::string fileName );
		void fixiNode( iNode &node );
		void verbose( std::string text, bool error = false );

		iNode *mainNode;
		Flags flags;
		Statistics stats;
	};

} // end of namespace



#endif
