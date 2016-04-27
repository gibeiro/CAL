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

void menu(cg1 *o){
	cout << "Current time: ";
	o->getCurrentTime().info();
	cout << "1 - Update\n";
	cout << "2 - Add Client\n";
	cout << "3 - Add road block\n";
	cout << "4 - Change vehicle speed\n";
	cout << "5 - Change vehicle space\n";
	cout << "6 - Change vehicle number\n";
	cout << "7 - Client info\n";
	cout << "0 - Exit\n";
	cout << "Selection: ";
}

void change_speed(){

	system("cls");
	double d;
	cout << "Enter new vehicle speed: ";
	cin >> d;
	vehicle::avg_speed = d;
	cout << "Speed set to " << d << endl;

	system("pause");

}

void change_space(){
	system("cls");
	size_t n;
	cout << "Enter new vehicle size: ";
	cin >> n;
	vehicle::max_passangers = n;
	cout << "Vehicle size set to " << n << endl;

	system("pause");

}

void change_number(cg1 o){
	system("cls");
	size_t n;
	cout << "Enter new number of vehicles: ";
	cin >> n;
	o.available_vehicles = n;
	cout << "Number of vehicles set to " << n << endl;

	system("pause");

}

int main(int argc, char *argv[]){

	vehicle::setStatics(.3, 6);

	cg1 *obj = new cg1("res/nodes.txt",
			"res/roads.txt",
			"res/subroads.txt",
			"res/landmarks.txt",
			"res/clients.txt",
			2
	);

	int choice;
	do{
		system("cls");
		menu(obj);
		cin >> choice;

		switch(choice) {
		case 1:
			obj->distrClients();
			obj->updateVehicles();
			cin.ignore(1);
			break;
		case 2:
			obj->addClient();
			break;
		case 3:
			obj->blockRoad();
			break;
		case 4:
			change_speed();
			break;
		case 5:
			change_space();
			break;
		case 6:
			change_number(*obj);
			break;
		case 7:
			obj->clientInfo();
			break;
		default:
			break;
		}
	} while(choice != 0);

	system("PAUSE");


	delete(obj);
	return 0;

}



