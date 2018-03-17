#!/bin/python2
#This is explision, see README.md for usage.
#If you wish to modify the mesh reding code, then please read the intro to
#OpenMesh first, the datastructure isn't the simplest one, but it is very
		#powerful for this application. Here's a few links:
#https://www.openmesh.org/intro/
#https://www.openmesh.org/media/Documentations/OpenMesh-Doc-Latest/a00020.html
#https://www.openmesh.org/media/Documentations/OpenMesh-Doc-Latest/index.html

from openmesh import *
from lxml import etree
import sys
import json
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
				'index'         :adjacent_face.idx(),
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
	open('shapes.svg',    'w').write(dump_svg(shapes))
	open('conectors.svg', 'w').write(dump_svg(connectors))

def dump_svg(elements):
	svg = etree.Element('svg')
	svg.set('id', 'svg')
	svg.set('version', '1.1')
	svg.set('xmlns', 'http://www.w3.org/2000/svg')
	svg.set('width', '300')
	svg.set('height', '300')
	for element in elements:
		svg.append(element)
	return etree.tostring(svg, pretty_print=True, encoding='utf8')

if __name__ == "__main__":
	config = json.loads(open(sys.argv[1]).read(-1))
	shaper = Shaper(config)
	inputfile = sys.argv[2]	
	main( shaper, inputfile )
