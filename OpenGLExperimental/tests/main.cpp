#include "../src/OBJClasses/Mesh.h"
#include "../src/OBJClasses/Face.h"
#include "../src/OBJClasses/OBJ.h"
#include "../src/OBJClasses/Reader/MeshReader.h"

int main() {
	MeshReader* meshReader = new MeshReader();
	Mesh* mesh = meshReader->loadMesh("res/obj/cube.obj");
	OBJ* obj = new OBJ(mesh);

	mesh = meshReader->loadMesh("res/obj/cube2.obj");
	OBJ* obj2 = new OBJ(mesh);

	std::cin.get();
}