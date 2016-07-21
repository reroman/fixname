#include <iostream>
#include <list>
#include <getopt.h>
#include "fixname.hpp"
using namespace std;
using namespace fixname;


int main( int argc, char **argv )
{
	char opt;
	opterr = 0;
	Flags flags = 0;
	list<FixName*> fixers;
	Statistics stats;
	const struct option args[] = {
		{ "all", no_argument, NULL, 'a' },
		{ "directory", no_argument, NULL, 'd' },
		{ "recursive", no_argument, NULL, 'r' },
		{ "force", no_argument, NULL, 'f' },
		{ "ignore", no_argument, NULL, 'i' },
		{ "verbose", no_argument, NULL, 'v' },
		{ "help", no_argument, NULL, 'h' },
		{ 0, 0, 0, 0 }
	};

	while( (opt = getopt_long( argc, argv, "adrfivh", args, NULL )) != -1 ){
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
				cout << "Elimina espacios y caracteres problemáticos en los nombres de archivos y/o carpetas.\n"
					 "\nModo de empleo: " << *argv << " [OPCIONES] FICHERO...\n"
					 "  o bien: " << *argv << " [OPCIONES] DIRECTORIO...\n"
					 "\nOpciones:\n"
					 "  -a, --all\t\tprocesa entradas que comienzan con .\n"
					 "  -d, --directory\tprocesa los archivos contenidos en DIRECTORIO\n"
					 "  -r, --recursive\tprocesa los subdirectorios recursivamente. Requiere\n"
					 "\t\t\tla opción -d\n"
					 "  -f, --force\t\tno pregunta antes de sobreescribir\n"
					 "  -i, --ignore\t\tignora la sobreescritura de archivos\n"
					 "  -v, --verbose\t\tda detalles de lo que va procesando\n"
					 "  -h, --help\t\tmuestra este mensaje de ayuda y finaliza\n"
					 "\nErrores y sugerencias: Ricardo Roman <reroman4@gmail.com>" << endl;
				return 0;
			default:
				cerr << "Opción inválida: " << argv[optind-1] << endl;
				return -1;
		}
	}
	if( optind == argc ){
		cerr << *argv << ": Falta el nombre del archivo o directorio\n";
		return -1;
	}

	try{
		for( ; optind < argc ; optind++ )
			fixers.push_back( new FixName( argv[optind], flags ) );
	}
	catch( invalid_argument e ){
		cerr << e.what() << endl;
		for( auto &i : fixers )
			delete i;
		return -1;
	}
	catch( runtime_error e ){
		cerr << e.what() << endl;
		for( auto &i : fixers )
			delete i;
		return -1;
	}

	for( auto &i : fixers ){
		i->fix();
		delete i;
	}
	stats = FixName::getStatistics();
	cout << "\nArchivos Ok: " << setColor( to_string(stats.filesOk), COLOR_CYAN)
		<< "\nArchivos Renombrados: " << setColor( to_string(stats.filesRenamed), COLOR_GREEN )
		<< "\nArchivos Ignorados: " << setColor( to_string(stats.filesIgnored), COLOR_YELLOW)
		<< "\nArchivos con Error: " << setColor( to_string(stats.filesError), COLOR_RED )
		<< endl;
	return 0;
}
