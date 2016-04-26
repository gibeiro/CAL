#include "cg1.h"
#include <fstream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include "graphviewer.h"
#include <unistd.h>

cg1::cg1(const char* nodes_file,
		const char* roads_file,
		const char* subroads_file,
		const char* landmarks_file,
		const char* clients_file):available_vehicles(1){

	graph = new Graph<unsigned int>();

	readNodes(nodes_file);
	vector<road> roads = readRoads(roads_file);
	readSubroads(subroads_file, roads);
	readLandmarks(landmarks_file);
	readClients(clients_file);

	graph->dijkstraShortestPath(airport.node);

	gv = new GraphViewer(1024,768,1);
	gv->createWindow(1024, 768);

	addNodes();
	addEdges();

	vector<unsigned int> v;
	v.push_back(0);
	v.push_back(5);
	v.push_back(10);
	v.push_back(11);
	v.push_back(6);
	v.push_back(7);
	v.push_back(2);
	v.push_back(1);
	v.push_back(0);


	displayPath(graph->getPath(0,24));
	getchar();
	//sleep(100);

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

	printf("Reading nodes from %s\n",file);

	char tmp1[128];
	char *tmp2;
	unsigned int id;
	double x,y;
	double x_rad,y_rad;


	while(!stream.eof()){

		stream.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		id = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		x = atof(tmp2);

		tmp2 = strtok(NULL,";");
		y = atof(tmp2);

		tmp2 = strtok(NULL,";");
		x_rad = atof(tmp2);

		tmp2 = strtok(NULL,";");
		y_rad = atof(tmp2);

		graph->addVertex(id,x,y,x_rad,y_rad);

	}

	printf("%d nodes successfully added!\n\n",graph->getVertexSet().size());

	return 1;

}

vector<road> cg1::readRoads(const char* file){

	vector<road> roads;

	ifstream stream;
	stream.open(file);

	if(!stream.is_open()){
		printf("Invalid file: %s\n",file);
		return roads;
	}

	printf("Reading roads from %s\n",file);

	char tmp1[256];
	char *tmp2;
	char *name;
	unsigned int id;

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

	printf("%d roads successfully added!\n\n",roads.size());

	return roads;

}

bool cg1::readSubroads(const char* file, vector<road> &roads){

	ifstream stream;
	stream.open(file);

	if(!stream.is_open()){
		printf("Invalid file: %s\n",file);
		return 0;
	}

	printf("Reading subroads from %s\n",file);

	char tmp1[256];
	char *tmp2;
	unsigned int id1, id2, id3;
	unsigned int previous_id = UINT_MAX;
	std::vector<road>::iterator it;

	while(!stream.eof()){

		stream.getline(tmp1,128);
		//printf("%s\n",tmp1);


		tmp2 = strtok(tmp1,";");
		id1 = atoi(tmp2);
		//printf("Road: %lu\n", id1);


		if(id1 > previous_id)
			roads.erase(it);

		previous_id = id1;

		tmp2 = strtok(NULL,";");
		id2 = atoi(tmp2);
		//printf("Src: %lu\n", id2);

		tmp2 = strtok(NULL,";");
		id3 = atoi(tmp2);
		//printf("Dest: %lu\n", id1);

		it = find(roads.begin(),roads.end(),id1);

		if(it == roads.end())
			continue;

		graph->addEdge(id2,id3,it->name,it->id,it->isBothWays);

	}

	printf("%u subroads successfully added!\n\n",graph->nrEdges());

	roads.clear();

	return 1;

}

bool cg1::readLandmarks(const char* file){

	ifstream stream;
	stream.open(file);

	printf("Reading landmarks from %s\n",file);

	if(!stream.is_open()){
		printf("Invalid file: %s\n",file);
		return 0;
	}

	landmark tmp;
	char tmp1[128];
	char *tmp2;
	bool fst_it = 1;

	while(!stream.eof()){

		stream.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		tmp.name = std::string(tmp2);

		tmp2 = strtok(NULL,";");
		tmp.node = (unsigned int)atoi(tmp2);

		printf("Landmark: %s\nNode: %d\n\n",tmp.name.c_str(),tmp.node);


		if(fst_it){
			fst_it = 0;
			airport = tmp;
		}
		else
			landmarks.push_back(tmp);

	}

	printf("Airport set to %s\n%u landmarks successfully added!\n\n",airport.name.c_str(),landmarks.size());

	return 1;

}

bool cg1::readClients(const char* file){

	ifstream stream;
	stream.open(file);

	if(!stream.is_open()){
		printf("Invalid file: %s\n",file);
		return 0;
	}

	printf("Reading clients from %s\n",file);

	client tmp;
	char tmp1[128];
	char *tmp2;
	vector<landmark>::const_iterator it;

	while(!stream.eof()){

		stream.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		if(tmp2 == NULL)
			continue;
		tmp.name = std::string(tmp2);

		tmp2 = strtok(NULL,";");
		it = find(landmarks.begin(),landmarks.end(), tmp2);
		if(it != landmarks.end())
			tmp.destination = *it;
		else
			continue;

		tmp2 = strtok(NULL,";");
		tmp.NIF = atol(tmp2);

		tmp2 = strtok(NULL,"h");
		tmp.arrival.h = atoi(tmp2);

		tmp2 = strtok(NULL,"\n");
		tmp.arrival.m = atoi(tmp2);

		clients.push_back(tmp);

	}

	printf("%u clients successfully added!\n\n",clients.size());

	return 1;
}

void cg1::distrClients(){



	/*
	if(available_vehicles == 0 || clients.size() == 0)
		return;

	sort(clients.begin(),clients.end());

	if(vehicles.size() != available_vehicles)
		for(size_t i = 0; i < available_vehicles; i++){
			vehicles.push_back(vehicle());
			vehicles[i].arrival = null_time();
		}

	vehicles[0].passangers.push_back(clients[0]);

	time wa = null_time();
	time wb = null_time();
	time current = clients[0].arrival;
	client c1,c2;
	vehicle tmp;

	size_t v_index = 0;

	c1 = clients[0];

	for(size_t i = 1; i < clients.size();){

		c2 = clients[i];

		wa = c1.arrival - c2.arrival;
		wb = c1.arrival + calcTime(tmp) - c2.arrival;

		if(c1.arrival + calcTime(tmp) < c2.arrival){
			available_vehicles++;
			vehicles.push_back(vehicle());
			vehicles[++v_index].passangers.push_back(c2);
			c1 = c2;
			i++;
			continue;
		}

		if(wa < wb){
			vehicles[v_index].passangers.push_back(c2);
			i++;
			continue;
		}

		else{
			if(++v_index == available_vehicles)
				return;
			else
				vehicles[v_index].passangers.push_back(c2);
			c1 = c2;
			i++;
		}

	}
	 */
	/*
	vector<client>::iterator it = clients.begin() + 1;

	while(clients.empty()){

		if(size_t n = frstAvailableVehicle(current) < vehicles.size())
			tmp = vehicles[n];
		else{
			current = (*++it).arrival;
			continue;
		}

		c2 = *it;

		wa = c1.arrival - c2.arrival;
		wb = c1.arrival + calcTime(tmp) - c2.arrival;

		if(c1.arrival + calcTime(tmp) < c2.arrival){
			displayPath();
			clients.erase(clients.begin());
			vehicles[v_index].passangers.push_back(c2);
			i--;
			c1 = c2;
		}

		if(wa < wb){
			vehicles[v_index].passangers.push_back(c2);
		}

		else{

		}


	}
	 */
}

time cg1::calcTime(vehicle &c){

	time tmp;

	for(size_t i = 0; i < c.passangers.size(); i++){

	}

	return tmp;

};

time operator+(const time &a, const time &b){
	time tmp;
	int m = a.m + b.m;
	int h = a.h + b.h;
	while(m > 59){
		m %= 60;
		h++;
	}
	tmp.h = h;
	tmp.m = m;
	return tmp;
}

time operator-(const time &a, const time &b){
	time tmp;
	int m = abs(a.m - b.m);
	int h = abs(a.h - b.h);
	tmp.h = h;
	tmp.m = m;
	return tmp;
}

bool operator<(const time &a, const time &b){
	if(a.h < b.h)
		return 1;
	else if(a.h == b.h){
		if(a.m < b.m)
			return 1;
		else return 0;
	}
	else
		return 0;
}

time null_time(){
	time tmp;
	tmp.h = 0;
	tmp.m = 0;
	return tmp;
}

void cg1::addNodes(){
	vector<Vertex<unsigned int>* > v = graph->getVertexSet();
	vector<Vertex<unsigned int>* >::const_iterator it1 = v.begin();
	vector<landmark >::const_iterator it2 = landmarks.begin();

	while(it1 != v.end()){
		gv->addNode((*it1)->getInfo());
		it1++;
	}

	gv->setVertexColor(airport.node,MAGENTA);
	gv->setVertexLabel(airport.node,airport.name);

	while(it2 != landmarks.end()){
		gv->setVertexColor(it2->node,RED);
		gv->setVertexLabel(it2->node,it2->name);
		it2++;
	}




}

void cg1::addEdges(){

	vector<Vertex<unsigned int>* > v1 = graph->getVertexSet();
	vector<Edge<unsigned int> > v2;
	vector<Vertex<unsigned int>* >::const_iterator it1 = v1.begin();
	vector<Edge<unsigned int> >::const_iterator it2;


	while(it1 != v1.end()){

		v2 = (*it1)->getEdges();
		it2 = v2.begin();

		while(it2 != v2.end()){


			gv->addEdge(it2->getId(),(*it1)->getInfo(),it2->getDestInfo(),EdgeType::DIRECTED);

			it2++;
		}
		it1++;
	}

	gv->rearrange();

}


void cg1::displayPath(const vector<unsigned int> &v){
	gv->setVertexColor(v[0],BLUE);
	for(size_t i = 1; i < v.size(); i++){
		gv->setVertexColor(v[i],BLUE);
		gv->setEdgeColor(graph->getEdge(v[i-1],v[i]), BLUE);
		gv->setEdgeThickness(graph->getEdge(v[i-1],v[i]),5);
	}
}
