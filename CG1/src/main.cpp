#include <iostream>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include "graph.h"
#include "graphviewer.h"
#include "cg1.h"

size_t vehicle::max_passangers = 0;
double vehicle::avg_speed = 0;

int main(int argc, char *argv[]){

	vehicle::setStatics(.25, 5);

	cg1 obj("res/nodes.txt",
			"res/roads.txt",
			"res/subroads.txt",
			"res/landmarks.txt",
			"res/clients.txt",
			2
	);

	return 0;

}

