#ifndef INODE_HPP
#define INODE_HPP

#include <string>
#include <stdexcept>

#define DIR_ROOT "/"
#define CHAR_SEPARATOR '/'

class iNode
{
public:
	iNode( std::string path )
		throw( std::runtime_error );

	bool isDirectory() const;
	std::string getName() const;
	std::string getParent() const;
	std::string getAbsolutePath() const;
	std::string getRelativePath() const;
	
	bool rename( std::string newName, bool overwrite = false )
		throw( std::runtime_error );

	static bool exists( std::string path );

private:
	void settingPathsHelper( std::string path );

	std::string name;
	std::string parent;
	bool directory;
};

#endif
