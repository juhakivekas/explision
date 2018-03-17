#The shaper takes in data about edge lengths and angles between them and
#returns SVG objects that can be added to procuxible SVG files
from lxml import etree
import math

def moveto(x, y, relative = False):
	command = 'm' if relative else 'M'
	return '{} {} {} '.format(command, x, y)

def lineto(x, y, relative = False):
	command = 'l' if relative else 'L'
	return '{} {} {} '.format(command, x, y)

class Shaper():
	def __init__(self, config):
		self.material = config['material']
		self.kerf     = config['kerf']
		if 'dpi' in config:
			self.dpi = config['dpi']
		else:
			self.dpi = 90

		if 'connector_spacing' in config:
			self.connector_spacing = config['connector_spacing']
		else:
			self.connector_spacing = 10 * self.material

		if 'connector_inset' in config:
			self.connector_inset = config['connector_inset']
		else:
			self.connector_inset = self.material

		self.connector_height = self.material
		if 'connector_width' in config:
			self.connector_width = config['connector_width']
		else:
			self.connector_width = self.connector_height

	def make_edge_sockets(self, length):
		# XXX needs to be defensive (special cases for short edges)
		inset  = self.connector_inset  + self.kerf
		edge_w = self.connector_width  - self.kerf
		edge_h = self.connector_height - self.kerf
		socket_count = int(length / self.connector_spacing)
		x = (length - ((socket_count-1) * self.connector_spacing)) / 2
		#move to middle of top edge of first socket
		d = moveto(x, inset)
		for i in range(0, socket_count):
			# make a single socket
			d += moveto(edge_h/2, 0, True)
			d += lineto(0, edge_w, True)
			d += lineto(-edge_h, 0, True)
			d += lineto(0,-edge_w, True)
			d += lineto( edge_h, 0, True)
			d += moveto(-edge_h/2, 0, True)
			# move to origin of next socket
			d += moveto(self.connector_spacing, 0, True)
		return d

	def make_sockets(self, edges):
		angle = 0; xpos = 0; ypos = 0
		group = etree.Element('g')
		for edge in edges:
			#only make sockets for edges taht are not mesh boundaties
			if edge['dihedral_angle']:
				path = etree.Element('path')
				path.set('stroke', 'blue')
				path.set('fill', 'none')
				path.set('d', self.make_edge_sockets(edge['length']))
				path.set('transform',
					'translate({x},{y})rotate({r}, 0, 0)'.format(
					r = (angle / math.pi) * 180,
					x = xpos,
					y = ypos
				))
				group.append(path)
			xpos += math.cos(angle) * edge['length']
			ypos += math.sin(angle) * edge['length']
			angle += math.pi - edge['sector_angle']
		return group

	def make_outline(self, edges):
		xpos = 0; ypos = 0; angle = 0
		path = etree.Element('path')
		d = moveto(0, 0)
		for edge in edges:
			xpos += math.cos(angle) * edge['length']
			ypos += math.sin(angle) * edge['length']
			angle += math.pi - edge['sector_angle']
			d += lineto(xpos, ypos)
		path.set('d', d)
		path.set('stroke', 'black')
		path.set('fill', 'none')
		return path
	
	def shape(self, edges):
		outline = self.make_outline(edges)
		sockets = self.make_sockets(edges)
		#notes = self.make_notes(edges)
		shape = etree.Element('g')
		shape.append(sockets)
		shape.append(outline)
		#shape.append(notes)
		return shape

	def make_connectors(self, edge):
		connector_count = int(edge['length'] / self.connector_spacing)
		group = etree.Element('g')
		path = etree.Element('path')
		path.set('stroke', 'black')
		path.set('fill', 'none')
		d = moveto(0,0)
		d += lineto(self.connector_width, 0, True)
		d += lineto(0, self.material, True)
		d += lineto(self.connector_inset, self.material, True)
		
if __name__ == '__main__':
	config = {
		'material': 4,
		'kerf': 0.20,
		'connector_spacing' : 40,
		'connector_width': 10
	}
	shaper = Shaper(config)
	edges = [
	{'length': 180,	'sector_angle': math.pi/3, 'dihedral_angle':math.pi/4},
	{'length': 140, 'sector_angle': math.pi/3, 'dihedral_angle':math.pi/3},
	{'length': 100, 'sector_angle': math.pi/3, 'dihedral_angle':math.pi/5},
	]
	svg = etree.Element('svg')
	svg.set('width', "300")
	svg.set('height', "300")
	svg.append(shaper.shape(edges))
	open('test.svg','w').write(
		etree.tostring(svg, pretty_print=True).decode('utf8')
	)
