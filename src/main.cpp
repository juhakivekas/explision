#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "ExpMesh.hh"
#include "json/json.h"

//legacy:
#include "struct/shape.h"
#include "module/make_shapes.h"
#include "module/pack_shapes.h"
#include "module/draw_shapes.h"
#include "module/make_connectors.h"



void emit_json(ExpMesh &model){
	Json::Value explision(Json::arrayValue);
	//iterate all edges
	for(auto fIt = model.faces_begin(); fIt != model.faces_end(); ++fIt){
		Json::Value face;
		face["id"] = fIt->idx();
		for(auto feIt = model.fh_cwiter(*fIt); feIt.is_valid(); ++feIt){
			Json::Value conn;
			conn["id"]  = model.opposite_face_handle(*feIt).idx();
			conn["len"] = model.data(model.edge_handle(*feIt)).length();
			conn["deg"] = model.data(model.edge_handle(*feIt)).angle();
			face["cons"].append(conn);
		}
		explision.append(face);
	}
	//emit JSON of the shape connectivity and geometry data
	std::cout << explision << std::endl;
}

//open the mesh and reorder, calculate and prepare the model into such a
//state that it can be used by any explision tool (vectorizer, AR,
//projection, etc.)
void read_mesh(ExpMesh &model, std::string filename){
	OpenMesh::IO::Options readOptions;
	OpenMesh::IO::read_mesh(model, filename, readOptions);

	//pre-calculate all edge lengths and angles
	for(auto eIt = model.edges_begin(); eIt != model.edges_end(); ++eIt){
		model.data(*eIt).set_length(model.calc_edge_length(*eIt));
		model.data(*eIt).set_angle( model.calc_dihedral_angle(*eIt));
	}

	//TODO reorder the face halfedge hanles so that the first one is the
	//longest.
}

int main(int argc, char* argv[]){
	if(argc != 2){
		std::cout
		<< "Usage: " << argv[0] << " <filename>"
		<< std::endl;
		return 1;
	}
	//the model we will be manipulating
	ExpMesh model;

	//read from file, the filetype will be guessed!
	read_mesh(model, argv[1]);

	//emit_json(model);
	int nshapes = model.n_faces();
	shape* shapes[nshapes];			//allocate shape pointers
	std::cout << nshapes << std::endl;
	make_shapes(model, shapes);				//create, project and inset shapes

	make_connectors(nshapes, shapes);	//output connectors to file

	transform layout[nshapes];			//layout is an array of transformations
	pack_shapes(shapes, layout, nshapes);	//calculate a good layout
	draw_shapes(shapes, layout, nshapes);	//draw shapes according to the layout
}
