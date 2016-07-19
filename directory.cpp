#include "directory.hpp"
#include <cerrno>
#include <cstring>
#include <dirent.h>

using std::string;
using std::invalid_argument;
using std::list;

Directory::Directory( std::string path ) 
	throw( std::runtime_error, std::invalid_argument )
	: iNode( path )
{
	if( !iNode::isDirectory() )
		throw invalid_argument( string(path) + ": " + 
				string(strerror( ENOTDIR )) );
}

std::list<iNode> Directory::getChildren( bool all ) const
{
	DIR *dirp = opendir( getAbsolutePath().c_str() );
	struct dirent *node;
	list<iNode> children;
	string parent = getAbsolutePath();


	while( (node = readdir(dirp)) ){
		if( strcmp( node->d_name, "." ) && strcmp( node->d_name, ".." ) )
			if( node->d_name[0] != '.' || all ){
				if( parent == "/" )
					children.push_back( iNode(parent +
								string(node->d_name)) );
				else
					children.push_back( iNode(parent + "/" +
								string(node->d_name)) );
			}
	}

	closedir( dirp );
	return children;
}

bool Directory::isDirectory( std::string path )
{
	DIR *dirp = opendir( path.c_str() );

	if( !dirp )
		return false;
	closedir( dirp );
	return true;
}
