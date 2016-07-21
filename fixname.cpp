#include "fixname.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::cerr;
using std::invalid_argument;
using std::runtime_error;
using std::string;



namespace fixname
{
	Statistics FixName::stats = { 0, 0, 0, 0, 0 };

	inline string setColor( string text, string colorAnsi ){
		return colorAnsi + text + COLOR_NORMAL;
	}

	Statistics FixName::getStatistics()
	{
		return stats;
	}

	FixName::FixName( std::string path, Flags flag )
		throw( std::invalid_argument, std::runtime_error )
		: flags( flag )
	{
		if( (flags & FLG_RECURSIVE) && !(flags & FLG_DIRECTORY) )
			throw invalid_argument( "Opción -r requiere opción -d" );

		if( (flags & FLG_IGNORE) && (flags & FLG_FORCE) )
			throw invalid_argument( "Opción -i se contrapone con opción -f" );

		if( flags & FLG_DIRECTORY )
			mainNode = new Directory( path );
		else
			mainNode = new iNode( path );
	}

	FixName::~FixName(){
		delete mainNode;
	}

	Statistics FixName::fix()
	{
		fixiNode( *mainNode );
		if( flags & FLG_DIRECTORY )
			scanDir( static_cast<Directory&>( *mainNode ) );

		return stats;
	}

	void FixName::scanDir( Directory &d )
	{
		for( auto &i : d.getChildren( flags & FLG_ALL ) ){
			fixiNode( i );
			if( i.isDirectory() && (flags & FLG_RECURSIVE) ){
				Directory aux( i.getAbsolutePath() );
				scanDir( aux );
			}
		}
	}

	void FixName::fixiNode( iNode &node )
	{
		string newName = getValidName( node.getName() );
		string newPath;
		string oldPath;
		size_t n;
		bool exists;

		stats.totalFiles++;

		if( newName == "" ){
			verbose( "'" + node.getRelativePath() + "':\n\t" + setColor("Ok", COLOR_CYAN) + "\n" );
			stats.filesOk++;
			return;
		}

		oldPath = newPath = node.getRelativePath();
		n = newPath.find_last_of( CHAR_SEPARATOR );
		newPath.replace( n + 1, string::npos, newName );
		exists = iNode::exists( newPath );

		if( exists && ((flags & FLG_IGNORE)) ){
			stats.filesIgnored++;
			verbose( "'" + node.getRelativePath() + "':\n\t" + setColor("Ignorado", COLOR_YELLOW) + "\n" );
		}
		else if( !exists || flags & FLG_FORCE ){
			try{
				node.rename( newPath, true );
				stats.filesRenamed++;
				verbose( "'" + oldPath + "':\n\t" + setColor("Renombrado", COLOR_GREEN) + ": '" 
						+ newName + "'\n", true );
			}
			catch( runtime_error e ){
				stats.filesError++;
				verbose( "'" + oldPath + "' -> '" + newName + 
						"':\n\t" + setColor("Error", COLOR_RED) + ": " + e.what() + "\n", true );
			}
		}
		else if( promptFile( newPath ) ){
			try{
				node.rename( newPath, true );
				stats.filesRenamed++;
				verbose( "'" + oldPath + "':\n\t" + setColor("Renombrado", COLOR_GREEN) + ": '" 
						+ newName + "'\n", true );
			}
			catch( runtime_error e ){
				stats.filesError++;
				verbose( "'" + oldPath + "' -> '" + newName + 
						"':\n\t" + setColor( "Error", COLOR_RED) + ": " + e.what() + "\n", true );
			}
		}
		else{
			stats.filesIgnored++;
			verbose( "'" + oldPath + "':\n\t" + setColor("Ignorado", COLOR_YELLOW) + "\n" );
		}
	}


	string FixName::getValidName( std::string fileName )
	{
		bool ok = true;
		string fix;
		static const string badChars( "#<$+%>!`&*‘|{?}@" );

		fix.reserve( fileName.length() );
		for( char &c : fileName )
			if( badChars.find_last_of( c ) != string::npos )
				ok = false;
			else if( c == ' ' || c == '/' || c == '\\' ){
				ok = false;
				fix.push_back( '_' );
			}
			else
				fix.push_back( c );

		if( ok )
			return "";
		else
			return fix;
	}

	void FixName::verbose( std::string text, bool error )
	{
		if( flags & FLG_VERBOSE )
			cout << text;
		else if( error )
			cerr << text;
	}

	bool FixName::promptFile( std::string newName )
	{
		string option;

		do{
			cout << "El archivo " << newName << " ya existe.\n"
				<< "¿Desea sobreescribirlo? (S/N) ";
			getline( cin, option );
		}while( option != "S" && option != "s" && option != "N" && option != "n" );

		if( option == "S" || option == "s" )
			return true;
		return false;
	}

} // end of namespace
