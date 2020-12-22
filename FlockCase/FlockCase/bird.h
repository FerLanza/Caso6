#pragma once

#include <iostream>
#include <string> 
#include <fstream>
using namespace std;

struct Bird {

	string id;
	double distance;
	double velocity;

	Bird() {
		this->id = "";
		this->distance = 0;
		this->velocity = 0;
	}
};
