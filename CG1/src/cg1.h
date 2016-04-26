#ifndef _CG1_H_
#define _CG1_H_

#include "graph.h"
#include "graphviewer.h"
#include <string>

class landmark{
public:
	string name;
	unsigned int node;
	bool operator==(const char* s) const{
		return name == std::string(s);
	}
	bool operator==(const unsigned int &n) const{
			return node == n;
		}
};

struct time{
	unsigned int h;
	unsigned int m;
};

time operator+(const time &a, const time &b);
time operator-(const time &a, const time &b);
bool operator<(const time &a, const time &b);
time null_time();

class client{
public:
	string name;
	unsigned long NIF;
	landmark destination;
	time arrival;
	bool operator<(const client c){
		if(arrival.h < c.arrival.h)
			return 1;
		else if(arrival.h == c.arrival.h){
			if(arrival.m < c.arrival.m)
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}

};

class vehicle{
public:
	vector<client> passangers;
	vector<unsigned int> path;
	static const size_t max_passangers = 8;
	static const unsigned int avg_speed = 60;
	bool set;
	time departure;
	time arrival;
	vehicle():set(0){};
};

class road{
public:
	string name;
	bool isBothWays;
	unsigned int id;
	road():name(0),isBothWays(0),id(0){};
	road(string s, bool b, unsigned int i):name(s),isBothWays(b),id(i){};
	bool operator==(const unsigned int &id){
		return this->id == id;
	};

};

class cg1{

private:
	Graph<unsigned int> *graph;
	vector<landmark> landmarks;
	vector<client> clients;
	vector<vehicle> vehicles;
	landmark airport;
	GraphViewer * gv;
	unsigned int available_vehicles;

public:

	bool readNodes(const char* file);
	vector<road> readRoads(const char* file);
	bool readSubroads(const char* file, vector<road> &roads);
	bool readLandmarks(const char* file);
	bool readClients(const char* file);

	void distrClients();
	bool freeVehicle() const {
		for(size_t i = 0; i < vehicles.size(); i++)
			if(!vehicles[i].set)
				return 1;
		return 0;
	};

	time calcTime(vehicle &c);
	void displayPath(vector<unsigned int> v);
	size_t frstAvailableVehicle(const time &t) const{

		for(size_t i = 0; i <= vehicles.size(); i++)
			if(vehicles[i].arrival < t)
				return i;

		return vehicles.size();

	}

	void addNodes();
	void addEdges();



	cg1(const char* nodes_file, const char* roads_file, const char* subroads_file, const char* landmarks_file, const char* clients_file);
	~cg1();

};

#endif /* _CG1_H_ */
