#ifndef SRC_CG1_H_
#define SRC_CG1_H_

#include "graph.h"
#include "graphviewer.h"
#include <string>

#define INTERFACE_HEIGHT 640
#define INTERFACE_WIDTH 640

class cg1{

private:
	Graph<int> *graph;
	//GraphViewer *interface;

public:

	cg1(const char* nodes_file, const char* roads_file, const char* subroads_file);
	~cg1();

};

class road{
	public:
		string name;
		bool isBothWays;
		int id;
		road():name(0),isBothWays(0),id(0){};
		road(string s, bool b, int i):name(s),isBothWays(b),id(i){};
		bool operator==(const int &id){
			return this->id == id;
		};

	};


#endif /* SRC_CG1_H_ */
