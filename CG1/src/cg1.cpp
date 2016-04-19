#include "cg1.h"
#include <fstream>
#include <string>
#include <stdlib.h>

cg1::cg1(const char* nodes_file, const char* roads_file, const char* subroads_file){

	graph = new Graph<int>();
	//interface = new GraphViewer(800,600,0);

	ifstream file;
	file.open(nodes_file);

	string tmp;
	int id,x,y;
	Vertex<int> node();

	while(!file.eof()){

		getline(file,tmp);

		//id = atoi( tmp.find() );

	}

}

cg1::~cg1(){
	delete(graph);
}
