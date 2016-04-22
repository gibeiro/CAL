#include "cg1.h"
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include "graphviewer.h"

cg1::cg1(const char* nodes_file, const char* roads_file, const char* subroads_file){

	graph = new Graph<int>();

	readNodes(nodes_file);
	readRoads(roads_file);
	readSubroads(roads_file);

	gv = new GraphViewer(800,600,1);
	gv->createWindow(800, 600);

	addNodes();

}

cg1::~cg1(){
	delete(graph);
}

bool cg1::readNodes(const char* file){

	ifstream stream;
	stream.open(file);

	if(!stream.is_open()){
		printf("Invalid file: %s\n",file);
		return 0;
	}

	char tmp1[128];
	char *tmp2;
	int id,x,y;

	while(!stream.eof()){

		stream.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		id = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		x = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		y = atoi(tmp2);

		graph->addVertex(Vertex<int>(id,x,y));

	}

	printf("Nodes successfully added!\n");

	return 1;

}

bool cg1::readRoads(const char* file){

	ifstream stream;
	stream.open(file);

	if(!stream.is_open()){
		printf("Invalid file: %s\n",file);
		return 0;
	}

	char tmp1[256];
	char *tmp2;
	char *name;
	int id;

	while(!stream.eof()){

		stream.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		id = atoi(tmp2);

		tmp2 = strtok(NULL,";");

		if(tmp2 != NULL){
			if(!strcmp(tmp2,"True")){
				roads.push_back(road("",1,id));
				continue;
			}

			if(!strcmp(tmp2,"False")){
				roads.push_back(road("",0,id));
				continue;
			}
		}


		name = tmp2;

		tmp2 = strtok(NULL,"\n");

		if(tmp2 != NULL){
			if(!strcmp(tmp2,"True"))
				roads.push_back(road(name,1,id));
			else
				roads.push_back(road(name,0,id));
		}
		else
			continue;

	}

	printf("Roads successfully added!\n");

	return 1;

}

bool cg1::readSubroads(const char* file){

	ifstream stream;
	stream.open(file);

	if(!stream.is_open()){
		printf("Invalid file: %s\n",file);
		return 0;
	}

	char tmp1[256];
	char *tmp2;
	int id1, id2, id3;
	double dist;
	int previous_id = -1;
	std::vector<road>::iterator it;
	Vertex<int> *v1;
	Vertex<int> *v2;

	while(!stream.eof()){

		stream.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		id1 = atoi(tmp2);

		if(id1 > previous_id && previous_id != -1)
			roads.erase(it);

		previous_id = id1;

		tmp2 = strtok(NULL,";");
		id2 = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		id3 = atoi(tmp2);

		it = find(roads.begin(),roads.end(),id1);

		if(it == roads.end())
			continue;

		else{

			v1 = graph->getVertex(id2);
			v2 = graph->getVertex(id3);

			if(v1 == NULL || v2 == NULL)
				continue;

			dist = sqrt(
					pow(v1->x - v2->x,2) +
					pow(v1->y - v2->y,2)
			);

		}

		graph->addEdge(
				graph->getVertex(id2)->x,
				graph->getVertex(id2)->y,
				dist,
				it->name,
				it->isBothWays,
				it->id
		);
	}

	printf("Subroads successfully added!\n");

	return 1;
}


bool cg1::addNodes(){

	std::vector<Vertex<int> *>::const_iterator it;
	std::vector<Edge<int> >::const_iterator it2;

	vector<Vertex<int> *> nodes = graph->getVertexSet();
	vector<Edge<int> > tmp;

	it = nodes.begin();

	while(it != nodes.end())
		gv->addNode((*it)->info,(int)(*it)->x,(int)(*it)->y);

	it = nodes.begin();

	while(it != nodes.end()){

		tmp = (*it)->getEdges();

		it2 = tmp.begin();

		while(it2 != tmp.end()){
			gv->addEdge(it2->id, (*it)->info, it2->dest->info, EdgeType::DIRECTED);
			gv->setEdgeWeight(it2->id,it2->weight);
			gv->setVertexLabel(it2->id,it2->name);
		}
	}


	gv->rearrange();

}
