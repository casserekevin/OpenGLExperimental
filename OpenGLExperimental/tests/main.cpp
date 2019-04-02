#include <iostream>
#include <string>
#include <cmath>

#include "../src/OBJClasses/Mesh.h"
#include "../src/OBJClasses/Face.h"
#include "../src/OBJClasses/OBJ.h"
#include "../src/OBJClasses/Reader/OBJReader.h"

int main() {
	OBJ* obj = OBJReader().loadOBJ("res/obj/test.obj");
	//bool result = OBJReader().CloadOBJ("res/obj/test.obj");

	/*string s = "0.999999";
	float f = stof(s);

	float fl = ((float)k);
	float flo = fl / 1000000;
	int n = (int)(f * 1000000);
	int j = round(n);
	f = (float)(n / 1000000);


	std::cout << f;*/
	int k = round(999999);

	std::cin.get();
}