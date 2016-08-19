#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "json/json.h"

typedef OpenMesh::TriMesh_ArrayKernelT<> ExplisionMesh;

int main(int argc, char* argv[]){
	//the model we will be manipulating
	ExplisionMesh Model;

	//read from file, the filetype will be guessed!
	OpenMesh::IO::Options readOptions;
	OpenMesh::IO::read_mesh(Model, argv[1], readOptions);
	
	Json::Value explision(Json::arrayValue);
	//iterate all edges
	for(auto fIt = Model.faces_begin(); fIt != Model.faces_end(); ++fIt){
		Json::Value face;
		face["id"] = fIt->idx();
		for(auto feIt = Model.fh_cwiter(*fIt); feIt.is_valid(); ++feIt){
			Json::Value conn;
			conn["id"]  = Model.opposite_face_handle(*feIt).idx();
			conn["len"] = Model.calc_edge_length(*feIt);
			conn["deg"] = 180 * Model.calc_dihedral_angle(*feIt)/3.14159265;
			face["cons"].append(conn);
		}
		explision.append(face);
	}
	std::cout << explision << std::endl;
}
