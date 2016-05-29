
#include <fstream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include "graphviewer.h"
#include "cg1.h"
#include "strings.h"

cg1::cg1(const char* nodes_file,
		const char* roads_file,
		const char* subroads_file,
		const char* landmarks_file,
		const char* clients_file,
		unsigned int nr_vehicles)
{

	available_vehicles = nr_vehicles;

	graph = new Graph<unsigned int>();
	current_time = time();


	readNodes(nodes_file);
	roads = readRoads(roads_file);
	vector<road> tmp = roads;
	readSubroads(subroads_file, tmp);
	readLandmarks(landmarks_file);
	readClients(clients_file);
	createVehicles();

	gv = new GraphViewer(1024,768,1);
	gv->createWindow(1024, 768);

	addNodes();
	addEdges();
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

	while(!stream.eof()){

		stream.getline(tmp1,128);

		tmp2 = strtok(tmp1,";");
		id = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		x = (double)atof(tmp2);

		tmp2 = strtok(NULL,";");
		y = (double)atof(tmp2);

		graph->addVertex(id,x,y);

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

	char tmp1[128];
	char *tmp2;
	char *name;
	unsigned int id;

	while(!stream.eof()){

		stream.getline(tmp1,sizeof(tmp1));

		tmp2 = strtok(tmp1,";");
		id = atoi(tmp2);

		tmp2 = strtok(NULL,";");
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


		tmp2 = strtok(tmp1,";");
		id1 = atoi(tmp2);


		if(id1 > previous_id)
			roads.erase(it);

		previous_id = id1;

		tmp2 = strtok(NULL,";");
		id2 = atoi(tmp2);

		tmp2 = strtok(NULL,";");
		id3 = atoi(tmp2);

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

	system("cls");

	if(clients.size()== 0){
		printf("No clients!\n");
		return;
	}

	sort(clients.begin(),clients.end());
	current_time = clients.begin()->arrival;

	size_t i = frstAvailableVehicle();
	if(i == vehicles.size()){
		printf("No available vehicles!\nClient %s lost ...\n", clients.begin()->name.c_str());
		clients.erase(clients.begin());
		return;
	}

	printf("\nAdding %s to a vehicle.\n",clients.begin()->name.c_str());
	vehicles[i].addPassanger(*clients.begin());
	clients.erase(clients.begin());

	time w1,w2, w3, w4;

	while(clients.size() != 0){

		if(vehicles[i].passangers.size() == vehicle::max_passangers){
			printf("Vehicle is full.\n");
			printf("\nVehicle went to the following destinations:\n");

			for(size_t n = 0; n < vehicles[i].passangers.size(); n++)
				cout << vehicles[i].passangers[n].destination.name << endl;

			vehicles[i].available = 0;
			displayPath(vehicles[i]);
			return;
		}

		w3 = clients.begin()->arrival;
		w4 = current_time;
		w1 = w3 - w4;

		w3 = calcTime(vehicles[i]);
		w4 = clients.begin()->arrival;
		w2 =  w3 - w4;

		if(clients.begin()->arrival + w2 < clients.begin()->arrival){
			printf("\nVehicle went to the following destinations:\n");

			for(size_t n = 0; n < vehicles[i].passangers.size(); n++)
				cout << vehicles[i].passangers[n].destination.name << endl;


			vehicles[i].available = 0;
			displayPath(vehicles[i]);

			return;
		}

		if(w1 < w2){
			printf("\nAdding %s to a vehicle.\n",clients.begin()->name.c_str());
			vehicles[i].addPassanger(*clients.begin());
			clients.erase(clients.begin());
			continue;
		}
		else{
			printf("\nVehicle went to the following destinations:\n");

			for(size_t n = 0; n < vehicles[i].passangers.size(); n++)
				cout << vehicles[i].passangers[n].destination.name << endl;

			vehicles[i].available = 0;
			displayPath(vehicles[i]);
			return;
		}
	}
	if(clients.empty()){
		printf("\nVehicle went to the following destinations:\n");

		for(size_t n = 0; n < vehicles[i].passangers.size(); n++)
			cout << vehicles[i].passangers[n].destination.name << endl;
		vehicles[i].available = 0;
		displayPath(vehicles[i]);
	}
}

time cg1::calcTime(vehicle &c){

	time tmp;
	double n = calcPath(c);
	unsigned int t = (unsigned int)  (0.5 + n /vehicle::avg_speed );

	if(t < 0)
		return time(0,0);

	while(t > 59){
		t = t % 60;
		tmp.h++;
	}
	tmp.m = t;

	c.arrival = c.departure + tmp;

	return c.arrival;

};

time time::operator+(const time &b) const{
	time tmp;
	int _m = m + b.m;
	int _h = h + b.h;
	while(_m > 59){
		_m %= 60;
		_h++;
	}
	tmp.h = _h;
	tmp.m = _m;
	return tmp;
}

time time::operator-(const time &b) const{

	time tmp;
	int _m = m - b.m;
	int _h = abs(h - b.h);
	if(_m < 0 && _h != 0){
		_m += 60;
		_h--;

	}
	_m = abs(_m);

	tmp.m = (unsigned int)_m;
	tmp.h = (unsigned int)_h;
	return tmp;
}

bool time::operator<(const time &b) const{
	if(h < b.h)
		return 1;
	else if(h == b.h){
		if(m < b.m)
			return 1;
		else return 0;
	}
	else
		return 0;
}
bool time::operator<=(const time &b) const{
	if(h < b.h)
		return 1;
	else if(h == b.h){
		if(m < b.m)
			return 1;
		if(m == b.m)
			return 1;
		else
			return 0;
	}
	else
		return 0;
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
			gv->setEdgeLabel(it2->getId(),it2->name);

			it2++;
		}
		it1++;
	}

	gv->rearrange();

}

void cg1::displayPath(const vehicle &v){

	clearGraph();
	for(size_t i = 1; i < v.path.size(); i++){
		if(i== 1)
			if(v.path[i-1] != airport.node)
				gv->setVertexColor(v.path[i-1],BLUE);
		if(v.path[i] != airport.node)
			gv->setVertexColor(v.path[i],BLUE);
		gv->setEdgeColor(graph->getEdge(v.path[i-1],v.path[i]), BLUE);
		gv->setEdgeThickness(graph->getEdge(v.path[i-1],v.path[i]),5);
	}
	gv->rearrange();
}

void cg1::createVehicles(){
	for(size_t i = 0; i < available_vehicles; i++)
		vehicles.push_back(vehicle());
}

double cg1::calcPath(vehicle &v){
	v.path.clear();

	if(v.passangers.size() == 0)
		return 0;

	double acc = 0;

	vector<client> clients = v.passangers;

	vector<unsigned int> tmp_v;
	vector<unsigned int> tmp_v2;
	double tmp_dist=0;
	double tmp_dist2=0;
	size_t tmp_index=0;

	v.path.push_back(airport.node);

	while(!clients.empty()){

		graph->dijkstraShortestPath(*(v.path.end()-1));

		for(size_t i = 0; i < clients.size(); i++){

			if(i == 0){

				tmp_v = graph->getPath(*(v.path.end()-1),clients[i].destination.node,tmp_dist);
				tmp_index = i;
				continue;
			}
			tmp_v2 = graph->getPath(*(v.path.end()-1),clients[i].destination.node,tmp_dist2);


			if(tmp_dist2 < tmp_dist){
				tmp_dist = tmp_dist2;
				tmp_v = tmp_v2;
				tmp_index = i;
			}
		}

		clients.erase(clients.begin() + tmp_index);
		v.path.insert(v.path.end(),tmp_v.begin()+1,tmp_v.end());

		acc += tmp_dist;

	}


	graph->dijkstraShortestPath(*(v.path.end()-1));
	tmp_v = graph->getPath(*(v.path.end()-1),airport.node,tmp_dist);
	acc += tmp_dist;
	v.path.insert(v.path.end(),tmp_v.begin()+1,tmp_v.end());

	return acc;

}

bool vehicle::addPassanger(client c){
	if(passangers.size() < max_passangers){
		passangers.push_back(c);
		departure = c.arrival;
		if(available)
			available = 0;
		return 1;
	}
	return 0;
}

void cg1::updateVehicles(){
	for(size_t i = 0; i < vehicles.size(); i++){
		if(vehicles[i].arrival <= current_time){
			vehicles[i].passangers.clear();
			vehicles[i].path.clear();
			vehicles[i].available = 1;
			vehicles[i].departure = time();
			vehicles[i].arrival = time();
		}
	}
	system("pause");
}

void cg1::clearGraph(){
	vector<Vertex<unsigned int>* > v = graph->getVertexSet();
	vector<Vertex<unsigned int>* >::const_iterator it1 = v.begin();
	vector<landmark >::const_iterator it2;

	while(it1 != v.end()){

		it2 = find(landmarks.begin(),landmarks.end(),(*it1)->getInfo());

		if(it2 != landmarks.end())
			gv->setVertexColor(it2->node,RED);

		if((*it1)->getInfo() == airport.node)
			gv->setVertexColor((*it1)->getInfo(),MAGENTA);

		else
			gv->setVertexColor((*it1)->getInfo(),YELLOW);

		it1++;
	}

	for(size_t i = 0; i < 80; i++){
		gv->setEdgeColor(i,BLACK);
		gv->setEdgeThickness(i,1);
	}
}

bool cg1::addClient(){

	system("cls");
	client c;
	time t;
	string s;
	unsigned long tmp;

	cout << "Name: ";
	getline(cin,c.name);

	cout << "Arrival hour: ";
	cin >> t.h;
	cout << "Arrival minute: ";
	cin >> t.m;

	c.arrival = t;

	if(t < current_time){
		printf("\nInvalid arrival: ");
		t.info();
		system("pause");
		return 0;
	}

	cout << "NIF: ";
	cin >> c.NIF;

	tmp = c.NIF;
	size_t cont = 0;
	while(tmp != 0){
		tmp /= 10;
		cont++;
	}
	if(cont != 9){
		printf("\nInvalid NIF: %lu\n",c.NIF);
		system("pause");
		return 0;
	}

	cout << "Destination: ";
	getline(cin,s);

	vector<landmark>::const_iterator it;

	it = find(landmarks.begin(),landmarks.end(), s.c_str());

	if(it == landmarks.end()){
		printf("\nInvalid destination: %s\n",s.c_str());
		system("pause");
		return 0;
	}

	c.destination = *it;

	clients.push_back(c);

	sort(clients.begin(),clients.end());

	printf("Client added successfully!\n");

	system("pause");

	return 1;

};

void cg1::clientInfo(){

	system("cls");

	sort(clients.begin(),clients.end());

	cout  << setw(8) << "ARRIVAL" << setw(15) << "NIF"<< setw(30) << "DESTINATION" << setw(25) << "NAME\n\n";

	for(size_t i = 0; i < clients.size(); i++){
		cout << setw(8) << clients[i].arrival.info_s() << setw(15) << clients[i].NIF << setw(30) << clients[i].destination.name << setw(25) << clients[i].name <<endl;
	}

	system("pause");


}

void cg1::searchClient(){
	system("cls");

	cin.ignore(1000,'\n');

	cout << "Client name? ";

	string client_name;
	getline(cin,client_name);

	cout << endl;

	std::vector<string> exact;
	std::vector<string> aprox;
	std::vector<int> substr;
	int distance;

	std::vector<client>::const_iterator it1;

	it1 = clients.begin();

	while(it1 != clients.end()){

		substr = KMP(it1->name,client_name);

		if(substr.size() > 0)
			exact.push_back(it1->name);
		else{
			distance = levenshtein_distance(it1->name,client_name);

			if( (size_t) distance <= client_name.size()/2)
				aprox.push_back(it1->name);
		}

		it1++;
	}
	size_t j;
	if( (j = exact.size()) > 0){
		cout << "Exact matches:" << endl;
		for(size_t i = 0; i < j; i++)
			cout << exact[i] << endl;
		cout << endl;
	}

	size_t k;
	if( (k = aprox.size()) > 0){
		cout << "Aproximate matches:" << endl;
		for(size_t i = 0; i < k; i++)
			cout << aprox[i] << endl;
		cout << endl;
	}

	if(j+k == 0)
		cout << "No matches found.\n\n";

	system("pause");

}

void cg1::searchDest(){
	system("cls");

	cin.ignore(1000,'\n');

	cout << "Adress name? ";

	string adress_name;
	getline(cin,adress_name);

	cout << endl;

	vector<string> v_tkn1;
	char tmp[128];
	string biggest_token = "";
	strcpy(tmp,adress_name.c_str());
	for(char *tkn = strtok(tmp," "); tkn != NULL; tkn = strtok(NULL," "))
		if(strlen(tkn) > biggest_token.size())
			biggest_token = string(tkn);

	std::vector<string> exact;
	std::vector<string> aprox;
	std::vector<int> substr;

	std::vector<Edge<unsigned int> *>::iterator it_edge;
	std::vector<landmark>::const_iterator it_landmark;
	std::vector<client>::iterator it_client;

	it_edge = Edge<unsigned int>::edgeSet.begin();

	while(it_edge != Edge<unsigned int>::edgeSet.end()){


		substr = KMP((*it_edge)->name,adress_name);

		//Exact match found
		if(substr.size() > 0){
			it_landmark = find(landmarks.begin(),landmarks.end(),(*it_edge)->getDestInfo());
			if(it_landmark != landmarks.end()){
				it_client = clients.begin();
				while( (it_client = find(it_client,clients.end(),(*it_landmark))) != clients.end() ){
					exact.push_back((*it_edge)->name + "\t" + it_client->name);
					it_client++;
				}
			}
		}
		else{
			v_tkn1.clear();
			strcpy(tmp,(*it_edge)->name.c_str());
			for(char *tkn = strtok(tmp," "); tkn; tkn = strtok(NULL," "))
				v_tkn1.push_back(string(tkn));

			int distance = 2147483647;
			for(size_t j = 0; j < v_tkn1.size();j++){
				int k = levenshtein_distance(biggest_token,v_tkn1[j]);
				if(distance > k)
					distance = k;
			}

			//Aproximate match found
			if( (size_t) distance <= biggest_token.size()/2){
				it_landmark = find(landmarks.begin(),landmarks.end(),(*it_edge)->getDestInfo());
				if(it_landmark != landmarks.end()){
					it_client = clients.begin();
					while( (it_client = find(it_client,clients.end(),(*it_landmark))) != clients.end() ){
						aprox.push_back((*it_edge)->name + "\t" + it_client->name);
						it_client++;
					}
				}
			}
		}

		(++it_edge)++;
	}

	size_t j;
	if( (j = exact.size()) > 0){
		cout << "Exact matches:" << endl;
		for(size_t i = 0; i < j; i++)
			cout << exact[i] << endl;
		cout << endl;
	}

	size_t k;
	if( (k = aprox.size()) > 0){
		cout << "Aproximate matches:" << endl;
		for(size_t i = 0; i < k; i++)
			cout << aprox[i] << endl;
		cout << endl;
	}

	if(j+k == 0)
		cout << "No matches found.\n\n";

	system("pause");
}



void cg1::blockRoad(){

	system("cls");

	unsigned int src, dst;

	cout << "Source node: ";
	cin >> src;
	cout << "Destination node: ";
	cin >> dst;

	if(src > 24 || dst > 24){
		cout << "Invalid nodes!\n";
		system("pause");
		return;
	}

	int n = abs((int)src - (int)dst);
	if(n == 1 || n == 5){
		unsigned int edge = graph->getEdge(src,dst);
		unsigned int _edge = rand() % 80 + 80;
		gv->removeEdge(edge);
		gv->addEdge(_edge,src,dst,EdgeType::DIRECTED);
		gv->setEdgeColor(_edge,GRAY);
		gv->setEdgeDashed(_edge,1);
		graph->removeEdge(src,dst);
		gv->rearrange();

		cout << "Successfully removed the edge from " << src << " to " << dst << "!\n";

		system("pause");
		return;
	}
	else{
		cout << "Invalid nodes!\n";
		system("pause");
		return;
	}
}
