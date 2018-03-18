#!/bin/python2
#This is explision, see README.md for usage.
#If you wish to modify the mesh reding code, then please read the intro to
#OpenMesh first, the datastructure isn't the simplest one, but it is very
		#powerful for this application. Here's a few links:
#https://www.openmesh.org/intro/
#https://www.openmesh.org/media/Documentations/OpenMesh-Doc-Latest/a00020.html
#https://www.openmesh.org/media/Documentations/OpenMesh-Doc-Latest/index.html
import sys
import json
from openmesh import *
from Shaper import Shaper

def parse_mesh( inputfile ):
	'''this uses OpenMesh to do the complex 3D measurements and operations of explision'''
	#read the mesh form the input file
	mesh = TriMesh()
	if not read_mesh( mesh, inputfile ):
		raise ValueError( "Could not read mesh from " + inputfile )
	#create shapes out of mesh faces
	shapes = []
	for face in mesh.faces():
		shape_edges = []
		maxlength = 0
		#reorder all faces to have their longest side as the the first edge
		for halfedge in mesh.fh( face ):
			length = mesh.calc_edge_length( mesh.edge_handle( halfedge ))
			if length > maxlength:
				maxlength = length
				mesh.set_halfedge_handle(face, halfedge);
		#calculate all informataion needed to draw a shape
		for halfedge in mesh.fh( face ):
			edge = mesh.edge_handle( halfedge )
			adjacent_face = mesh.face_handle(mesh.opposite_halfedge_handle( halfedge ))
			#if this looks like magic, then ask Juha or read OpenMesh docs
			shape_edges.append({
				'adjacent_index':adjacent_face.idx(),
				'length'        :mesh.calc_edge_length( edge ),
				'sector_angle'  :mesh.calc_sector_angle( halfedge ),
				'dihedral_angle':mesh.calc_dihedral_angle( edge ),
				'connected'     :not mesh.is_boundary( edge ),
				'connectors'    :adjacent_face.idx() > face.idx()
			})
		shapes.append({
			'index' :face.idx(),
			'edges' :shape_edges 
		})
	#create connectors for of non-boundary edges
	return (shapes)

if __name__ == "__main__":
	config = json.loads(open(sys.argv[1]).read(-1))
	shaper = Shaper(config)
	inputfile = sys.argv[2]	
	shapes = parse_mesh( inputfile )
	open('shapes.svg',     'w').write(shaper.render_shapes(shapes))
	open('connectors.svg', 'w').write(shaper.render_connectors(shapes))
