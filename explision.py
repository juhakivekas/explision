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

def main( shaper, inputfile ):
	#read the mesh form the input file
	mesh = TriMesh()
	if not read_mesh( mesh, inputfile ):
		raise ValueError( "Could not read mesh from " + inputfile )
	#create shapes out of mesh faces
	shapes = []
	for face in mesh.faces():
		shape_edges = []
		for halfedge in mesh.fh( face ):
			edge = mesh.edge_handle( halfedge )
			adjacent_face = mesh.face_handle(mesh.opposite_halfedge_handle( halfedge ))
			#if this looks like magic, then ask Juha or read OpenMesh docs
			shape_edges.append({
				'adjacent_index':adjacent_face.idx(),
				'length'        :mesh.calc_edge_length( edge ),
				'sector_angle'  :mesh.calc_sector_angle( halfedge ),
				'dihedral_angle':mesh.calc_dihedral_angle( edge ),
				'connected'     :not mesh.is_boundary( edge )
			})
		shapes.append(shaper.shape( shape_edges, face.idx() ))

	#create connectors for of non-boundary edges
	connectors = []
	for edge in mesh.edges():
		if not mesh.is_boundary( edge ):
			connectors.append(shaper.make_connectors({
				'dihedral_angle': mesh.calc_dihedral_angle( edge ),
				'length':         mesh.calc_edge_length( edge )
			}))
	open('shapes.svg',     'w').write(shaper.dump_svg(shapes))
	open('connectors.svg', 'w').write(shaper.dump_svg(connectors))

if __name__ == "__main__":
	config = json.loads(open(sys.argv[1]).read(-1))
	shaper = Shaper(config)
	inputfile = sys.argv[2]	
	main( shaper, inputfile )
