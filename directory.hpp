#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "inode.hpp"
#include <list>

class Directory : public iNode
{
public:
	Directory( std::string path ) 
		throw( std::runtime_error, std::invalid_argument );

	std::list<iNode> getChildren( bool all = false ) const;

	static bool isDirectory( std::string path );
};

#endif 
