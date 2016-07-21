#include "inode.hpp"
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
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
	setterPathsHelper( path );
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

std::string iNode::getRelativePath() const
{
	char c_cwd[PATH_MAX];
	string cwd( getcwd( c_cwd, PATH_MAX ) );
	string absPath = getAbsolutePath();
	string result;
	size_t n;

	if( absPath[0] != '/' )
		return "";
	if( *absPath.rbegin() == '/' )
		absPath.pop_back();
	
	if( cwd == absPath )
		return ".";

	if( cwd == "/" )
		return absPath.substr( 1, string::npos );

	while( absPath.find( cwd ) == string::npos && cwd != ""  ){
		result.append( "../" );
		n = cwd.find_last_of( '/' );
		cwd.erase( n );
	}
	if( cwd == "" )
		result.append( absPath, 1, string::npos );
	else
		result.append( absPath, cwd.length() + 1, string::npos );
	return result;
}	

bool iNode::rename( std::string newName, bool overwrite ) 
	throw( runtime_error )
{
	if( !exists(newName) || overwrite ){
		if( !::rename( getAbsolutePath().c_str(), newName.c_str() ) ){
			setterPathsHelper( newName );
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

void iNode::setterPathsHelper( std::string path )
{
	size_t n;
	struct stat fileInfo;
	char c_path[PATH_MAX];

	if( lstat( path.c_str(), &fileInfo ) < 0 )
		throw runtime_error( path + ": " + string(strerror(errno)) );

	getcwd( c_path, PATH_MAX );
	
	if( path == DIR_ROOT ){
		name = DIR_ROOT;
		parent = "";
		directory = true;
	}
	else if( S_ISLNK(fileInfo.st_mode) ){
		n = path.find_last_of( CHAR_SEPARATOR );
		if( n == string::npos ){
			name = path;
			parent = string( c_path );
		}
		else{
			name = path.substr( n + 1 );
			path.erase( n );
			if( path == "" )
				parent = DIR_ROOT;
			else if( path[0] == CHAR_SEPARATOR )
				parent = path;
			else
				parent = string( c_path ) + "/" + path;
			realpath( parent.c_str(), c_path );
			parent = string( c_path );
		}
		directory = false;
	}
	else{
		realpath( path.c_str(), c_path );
		path = string( c_path );
		n = path.find_last_of( CHAR_SEPARATOR );
		name = path.substr( n + 1 );
		path.erase( n );
		if( name == "" ){
			name = DIR_ROOT;
			parent = "";
		}
		else if( path == "" )
			parent = DIR_ROOT;
		else
			parent = path;
		directory = static_cast<bool>( S_ISDIR(fileInfo.st_mode) );
	}
}
