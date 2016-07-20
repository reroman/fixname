#include <iostream>
#include <unistd.h>
#include "fixname.hpp"
using namespace std;
using namespace fixname;


int main( int argc, char **argv )
{
	char opt;
	Flags flags = 0;
	FixName *fixer;
	Statistics stats;

	while( (opt = getopt( argc, argv, ":adrfivh" )) != -1 ){
		switch( opt ){
		case 'a':
			flags |= FLG_ALL;
			break;
		case 'd':
			flags |= FLG_DIRECTORY;
			break;
		case 'r':
			flags |= FLG_RECURSIVE;
			break;
		case 'f':
			flags |= FLG_FORCE;
			break;
		case 'i':
			flags |= FLG_IGNORE;
			break;
		case 'v':
			flags |= FLG_VERBOSE;
			break;
		case 'h':
			cout << "Uso " << *argv << " [options] %s\n";
			return 0;
		default:
			opterr = 0;
			cerr << "Opción inválida: " << argv[optind] << endl;
			return -1;
		}
	}
	if( optind == argc ){
		cerr << *argv << ": Falta el nombre del archivo o directorio\n";
		return -1;
	}

	try{
		fixer = new FixName( argv[optind], flags );
	}
	catch( invalid_argument e ){
		cerr << e.what() << endl;
		return -1;
	}
	catch( runtime_error e ){
		cerr << e.what() << endl;
		return -1;
	}

	stats = fixer->fix();

	cout << "\nArchivos Ok: " << stats.filesOk
		<< "\nArchivos Renombrados: " << stats.filesRenamed
		<< "\nArchivos Ignorados: " << stats.filesIgnored
		<< "\nArchivos con Error: " << stats.filesError << endl;
	delete fixer;
	return 0;
}
