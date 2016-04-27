#ifndef _CG1_H_
#define _CG1_H_

#include "graph.h"
#include "graphviewer.h"
#include <string>
#include <stdio.h>


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
	landmark(string s, unsigned int n):name(s),node(n){};
	landmark():name(""),node(-1){};
};

class time{
public:
	unsigned int h;
	unsigned int m;
	time():h(0),m(0){};
	time(unsigned int a, unsigned int b):h(a),m(b){};
	time operator+(const time &b) const;
	time operator-(const time &b) const;
	bool operator<(const time &b) const;
	bool operator<=(const time &b) const;
	void info() const{
		printf("%dh%d\n",h,m);
	};
	string info_s() const{
		char tmp[32];
		sprintf(tmp,"%dh%d",h,m);
		return std::string(tmp);
	};
};

//time operator-(const time &a, const time &b);

class client{
public:
	string name;
	unsigned long NIF;
	landmark destination;
	time arrival;
	bool operator<(const client &c) const{
		return arrival < c.arrival;
	}

};

class vehicle{
public:
	vector<client> passangers;
	vector<unsigned int> path;
	static size_t max_passangers;
	static double avg_speed;
	bool available;
	time departure;
	time arrival;
	vehicle():available(1){};
	static void setStatics(double d,unsigned int ui){
		max_passangers = ui;
		avg_speed = d;
	};

	bool addPassanger(client c);
	friend class graph;
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
	GraphViewer * gv;

	landmark airport;
	time current_time;

public:

	friend class vehicle;

	unsigned int available_vehicles;

	bool readNodes(const char* file);
	vector<road> readRoads(const char* file);
	bool readSubroads(const char* file, vector<road> &roads);
	bool readLandmarks(const char* file);
	bool readClients(const char* file);
	void createVehicles();
	void updateVehicles();
	void distrClients();
	time getCurrentTime() const{
		return current_time;
	}

	bool addClient();
	void clientInfo();


	size_t frstAvailableVehicle() const{

		for(size_t i = 0; i <= vehicles.size(); i++)
			if(vehicles[i].available)
				return i;

		return vehicles.size();

	}

	void addNodes();
	void addEdges();
	double calcPath(vehicle &v);
	time calcTime(vehicle &c);
	void displayPath(const vehicle &v);
	void clearGraph();



	cg1(const char* nodes_file,
			const char* roads_file,
			const char* subroads_file,
			const char* landmarks_file,
			const char* clients_file,
			unsigned int nr_vehicles
	);
	~cg1(){
		gv->closeWindow();
		delete(gv);
		delete(graph);
	};

};

#endif /* _CG1_H_ */
