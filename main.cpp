#include <iostream>
#include "inode.hpp"
using namespace std;

int main( int argc, char **argv )
{
	if( argc != 3 ){
		cerr << "Uso:\n\t" << *argv << " <path>\n";
		return -1;
	}

	try{
		iNode node( argv[1] );
		cout << "Name: " << node.getName()
			<< "\nDirectory: " << boolalpha << node.isDirectory()
			<< "\nParent: " << node.getParent()
			<< "\nAbsolute: " << node.getAbsolutePath() << endl;

		if( node.rename( argv[2] ) )
			cout << "Exitoso" << endl;
		else
			cout << "Falló" << endl;

		cout << "Name: " << node.getName()
			<< "\nDirectory: " << boolalpha << node.isDirectory()
			<< "\nParent: " << node.getParent()
			<< "\nAbsolute: " << node.getAbsolutePath() << endl;


	}
	catch( std::invalid_argument e ){
		cerr << e.what() << endl;
	}
	catch( std::runtime_error e ){
		cerr << e.what() << endl;
	}

	return 0;
}
