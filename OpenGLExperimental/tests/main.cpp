#include <iostream>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
//#include <stdc++.h>

#include "../src/OBJClasses/Mesh.h"
#include "../src/OBJClasses/Face.h"
#include "../src/OBJClasses/OBJ.h"
#include "../src/OBJClasses/Reader/OBJReader.h"

int main() {
	//OBJ* obj = OBJReader().loadOBJ("res/obj/test.obj");
	//bool result = OBJReader().CloadOBJ("res/obj/test.obj");

	string s = "0.999999";
	std::istringstream iss(s);
	float f = 0;
	iss >> std::setprecision(3) >> f;

	std::cin.get();
}