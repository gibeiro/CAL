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



#endif /* SRC_CG1_H_ */
