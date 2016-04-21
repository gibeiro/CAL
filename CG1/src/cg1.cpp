#include "cg1.h"
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <algorithm>

cg1::cg1(const char* nodes_file, const char* roads_file, const char* subroads_file){

	graph = new Graph<int>();
	//interface = new GraphViewer(800,600,0);

	ifstream file;
	file.open(nodes_file);

	char tmp1[128];
	char *tmp2;
	int id,x,y;
	string name;
	vector< road > edges;
	road tmp;

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

		if(strcmp(tmp2,"True") == 0)
			edges.push_back(road(name,1,id));
		else
			edges.push_back(road(name,0,id));

	}

	file.close();

	file.open(subroads_file);

	while(!file.eof()){

		file.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		id = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		x = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		y = atoi(tmp2);

		for(size_t i = 0; i < edges.size(); i++){

			if(edges[i].id < id)
				edges.erase(edges.begin()+i--);

			else if(edges[i].id == id){
				id = sqrt(
						pow(graph->getVertex(x)->x - graph->getVertex(y)->x,2) +
						pow(graph->getVertex(x)->y - graph->getVertex(y)->y,2)
				);
				graph->addEdge(x,y,id,edges[i].name,edges[i].isBothWays,edges[i].id);
			}
		}
	}
}

cg1::~cg1(){
	delete(graph);
}
