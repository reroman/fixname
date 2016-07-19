#include "inode.hpp"
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using std::string;
using std::runtime_error;
using std::invalid_argument;

extern int errno;

iNode::iNode( std::string path ) 
	throw( runtime_error )
{
	size_t n;
	struct stat fileInfo;
	char c_path[PATH_MAX];

	if( path == DIR_ROOT ){
		name = DIR_ROOT;
		parent = "";
		directory = true;
	}
	else{
		if( stat( path.c_str(), &fileInfo ) < 0 )
			throw runtime_error( path + ": " + string(strerror(errno)) );
		
		getcwd( c_path, PATH_MAX );

		if( S_ISDIR(fileInfo.st_mode) ){
			directory = true;
			if( *path.rbegin() == CHAR_SEPARATOR )
				path.pop_back();
			else if( path == "." )
				path = string( c_path );
		}
		else 
			directory = false;

		n = path.find_last_of( CHAR_SEPARATOR );
		if( n != string::npos ){
			name = path.substr( n + 1 );
			path.erase( n + 1 );
		}
		else
			name = path;
		
		if( path[0] == CHAR_SEPARATOR )
			parent = path.substr( 0, n );
		else{
			parent = string( c_path ) + path;
			realpath( parent.c_str(), c_path );
			parent = string( c_path );
		}
	}
}

bool iNode::isDirectory() const
{
	return directory;
}

std::string iNode::getName() const
{
	return name;
}

std::string iNode::getParent() const
{
	return parent;
}

std::string iNode::getAbsolutePath() const
{
	if( name == DIR_ROOT )
		return name;
	else if( parent == DIR_ROOT )
		return parent + name;
	return parent + DIR_ROOT + name;
}

	bool iNode::rename( std::string newName, bool overwrite ) 
throw( runtime_error )
{
	char auxPath1[PATH_MAX];
	char auxPath2[PATH_MAX];

	if( !exists(newName) || overwrite ){
		if( !::rename( getAbsolutePath().c_str(), newName.c_str() ) ){
			realpath( newName.c_str(), auxPath1 );
			strcpy( auxPath2, auxPath1 );
			name = std::string( basename(auxPath1) );
			parent = std::string( dirname(auxPath2) );
			return true;
		}
		else
			throw runtime_error( "rename(): " + string( strerror(errno) ) );
	}
	return false;
}

bool iNode::exists( std::string path )
{
	return access( path.c_str(), F_OK ) == 0;
}
