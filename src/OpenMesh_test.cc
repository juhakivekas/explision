#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> ExplisionMesh;

int main(int argc, char* argv[]){
	//the model we will be manipulating
	ExplisionMesh Model;

	//read from file, the filetype will be guessed!
	OpenMesh::IO::Options readOptions;
	OpenMesh::IO::read_mesh(Model, argv[1], readOptions);
	
	//iterate all edges
	for(auto fIt = Model.faces_begin(); fIt != Model.faces_end(); ++fIt){
		//std::cout << "{";
		//std::cout << ""fIt->idx() << std::endl;
		for(auto feIt = Model.fh_cwiter(*fIt); feIt.is_valid(); ++feIt){
			//std::cout << '\t';
			//std::cout << Model.opposite_face_handle(*feIt).idx() << '\t';
			//std::cout << Model.calc_edge_length(*feIt) << '\t';
			//std::cout << 180 * Model.calc_dihedral_angle(*feIt)/3.14159265<< std::endl;
		}
	}
	//OpenMesh::IO::write_mesh(Model, "output.ply");
}
