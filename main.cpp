#include <iostream>
#include "directory.hpp"
using namespace std;

int main( int argc, char **argv )
{
	if( argc != 2 ){
		cerr << "Uso:\n\t" << *argv << " <path>\n";
		return -1;
	}

	try{

		Directory d( argv[1] );

		cout << "Name: " << d.getName()
			<< "\nDirectory: " << boolalpha << ((iNode)d).isDirectory()
			<< "\nParent: " << d.getParent()
			<< "\nAbsolute: " << d.getAbsolutePath() << endl;

		cout << "Children:\n";
		for( auto &i : d.getChildren() ){
				cout << i.getAbsolutePath() << '\n';
		}
	}
	catch( std::invalid_argument e ){
		cerr << e.what() << endl;
	}
	catch( std::runtime_error e ){
		cerr << e.what() << endl;
	}

	return 0;
}
