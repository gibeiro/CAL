#include "cg1.h"
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>

cg1::cg1(const char* nodes_file, const char* roads_file, const char* subroads_file){

	graph = new Graph<int>();
	//interface = new GraphViewer(800,600,0);

	ifstream file;
	file.open(nodes_file);

	char tmp1[128];
	char *tmp2;
	int id,x,y;
	string name;
	vector<Edge<int>> edges;

	while(!file.eof()){

		file.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		id = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		x = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		y = atoi(tmp2);

		graph->addVertex(Vertex<int>(id,x,y));

	}

	file.close();

	file.open(roads_file);

	while(!file.eof()){

			file.getline(tmp1,128);

			tmp2 = strtok(tmp1,";");
			id = atoi(tmp2);

			tmp2 = strtok(NULL,";");
			name = tmp2;

			tmp2 = strtok(NULL,";");

			if(strcmp(tmp2,"True") == 0){
				graph
			}

			//edges.pushback(Vertex);
			graph->addVertex(id);

		}

	file.close();

}

cg1::~cg1(){
	delete(graph);
}
