#The shaper takes in data about edge lengths and angles between them and
#returns SVG objects that can be added to procucible SVG files. Since SVG
#is an XML based format, lcml is used to vreate content. Many paths are
#built manually in this code, so check out these links for details on
#what is going on:
#https://developer.mozilla.org/en-US/docs/Web/SVG/Element/path
#https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d
#https://www.w3.org/TR/SVG/paths.html
import math
from lxml import etree
from copy import deepcopy
from Font import explisionfont

def moveto(vec, relative = False):
	'''a path move command as described in SVG specifivation'''
	command = 'm' if relative else 'M'
	return '{} {} {} '.format(command, vec[0], vec[1])

def lineto(vec, relative = False):
	'''a path line command as described in SVG specifivation'''
	command = 'l' if relative else 'L'
	return '{} {} {} '.format(command, vec[0], vec[1])

def translate(x,y):
	return 'translate({},{})'.format(x,y)

def rot(x, y, r):
	'''applies a rotation matrix on a vector'''
	#makes connector drawing much cleaner
	#https://en.wikipedia.org/wiki/Rotation_matrix 
	rx = x * math.cos(r) - y * math.sin(r)
	ry = x * math.sin(r) + y * math.cos(r)
	return (rx, ry)

def add(vec_a, vec_b):
	'''adds two vetors'''
	#numpy isn't worth importing just for this
	return (vec_a[0] + vec_b[0], vec_a[1] + vec_b[1])
	
class Shaper():
	def __init__(self, config):
		'''reads configurations and sets default values'''
		self.material = config['material']
		self.kerf     = config['kerf']
		if 'dpi' in config:
			self.dpi = config['dpi']
			print("The dpi configuration currently does nothing.")
		else:
			self.dpi = 90

		if config['socket_spacing']:
			self.socket_spacing = config['socket_spacing']
		else:
			self.socket_spacing = 10 * self.material
		
		if config['socket_margin']:
			self.socket_margin = config['socket_margin']
		else:
			self.socket_margin = self.socket_spacing

		if config['socket_inset']:
			self.socket_inset = config['socket_inset']
		else:
			self.socket_inset = self.material

		self.connector_height = self.material
		if config['connector_width']:
			self.connector_width = config['connector_width']
		else:
			self.connector_width = self.connector_height

		if config['connector_margin']:
			self.connector_margin = config['connector_margin']
		else:
			#some more sophisticated math could go here
			self.connector_margin = self.material

	def make_note(self, edges, index):
		'''makes the necessary marking texts'''
		text = '{:3d}-->\n'.format(index)
		for edge in edges:
			if edge['connected']:
				text += '{:2d}{:4d}\n'.format(
					edge['adjacent_index'],
					int(180 * edge['dihedral_angle'] / math.pi))
			else:
				text += '------\n'
		return explisionfont(text)

	def make_edge_sockets(self, length):
		'''make the sockets for connectors for a single edge of a shape'''
		inset  = self.socket_inset  + self.kerf
		edge_w = self.connector_width  - self.kerf
		edge_h = self.connector_height - self.kerf
		socket_count = int((length - 2*self.socket_margin) / self.socket_spacing) + 1
		if socket_count <= 0:
			socket_count = 1
		#move to middle of top edge of first socket
		x = (length - ((socket_count-1) * self.socket_spacing)) / 2
		d = moveto((x, inset))
		for i in range(0, socket_count):
			# make a single socket
			d += moveto((edge_h/2, 0), True)
			d += lineto((0,  edge_w), True)
			d += lineto((-edge_h, 0), True)
			d += lineto((0, -edge_w), True)
			d += lineto(( edge_h, 0), True)
			d += moveto((-edge_h/2, 0), True)
			# move to origin of next socket
			d += moveto((self.socket_spacing, 0), True)
		return d

	def make_sockets(self, edges):
		'''make sockets for all edges of a shape'''
		angle = 0; xpos = 0; ypos = 0
		group = etree.Element('g')
		for edge in edges:
			if edge['connected']:
				path = etree.Element('path')
				path.set('stroke', 'blue')
				path.set('stroke-width', '0.2')
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
		'''draws a simple outline based on shape edges'''
		xpos = 0; ypos = 0; angle = 0
		d = moveto((0, 0))
		for edge in edges:
			xpos += math.cos(angle) * edge['length']
			ypos += math.sin(angle) * edge['length']
			angle += math.pi - edge['sector_angle']
			d += lineto((xpos, ypos))
		path = etree.Element('path')
		path.set('d', d)
		path.set('stroke', 'black')
		path.set('stroke-width', '0.2')
		path.set('fill', 'none')
		return path

	def make_shape(self, index, edges):
		'''creates a shape based on edge information'''
		outline = self.make_outline(edges)
		sockets = self.make_sockets(edges)
		notes   = self.make_note(edges, index)
		notes.set('transform', translate(
			x = edges[0]['length']/2 -6,
			y = self.socket_inset + self.connector_width
		))
		shape = etree.Element('g')
		shape.append(sockets)
		shape.append(outline)
		shape.append(notes)
		return shape

	def make_connector_path(self, angle):
		d = moveto((0, self.connector_width + self.material))
		if angle < 0:
			d += moveto(( self.connector_width + self.material, 0), True)
			d += lineto((-(self.connector_width + self.material), 0), True)
		d += lineto((0, -(self.connector_width + self.material)), True)
		d += lineto((self.connector_width - self.kerf, 0), True)
		d += lineto((0, self.material), True)
		d += lineto((self.material, 0), True)
		#the sharp corner between the faces is here
		if angle < 0:
			d += lineto(add((0, -self.material), rot(0, self.material, angle)), True)
		d += lineto(rot(self.material, 0, angle), True)
		d += lineto(rot(0, -self.material, angle), True)
		d += lineto(rot(self.connector_width - self.kerf, 0, angle), True)
		d += lineto(rot(0, self.connector_width + self.material, angle), True)
		if angle < 0:
			d += lineto(rot(-(self.connector_width + self.material), 0, angle), True)
		d += 'z'
		return d

	def make_connectors(self, edge):
		'''makes the connector for an edge of a specified length'''
		angle = edge['dihedral_angle']
		path = etree.Element('path')
		path.set('stroke', 'black')
		path.set('stroke-width', '0.2')
		path.set('fill', 'none')
		path.set('d', self.make_connector_path(angle))
		text = explisionfont(str(int(180 * angle / math.pi)))
		text.set('transform', translate(
			x = 0.5,
			y = self.material
		))
		pair = etree.Element('g')
		pair.append(path)
		pair.append(text)
		#copy the connector as many times as needed
		connector_count = int(edge['length'] / self.socket_spacing)
		group = etree.Element('g')
		for i in range(connector_count):
			#translate the path to make sure connectpors don't overlap
			pair.set('transform', translate(
				x = 0,
				y = i * (self.connector_margin + self.material + self.connector_width)
			))
			group.append(deepcopy(pair))
		return group

	def render_shapes(self, shapes):
		svg = self.new_svg()
		for i, shapeinfo in enumerate(shapes):
			shape = self.make_shape(shapeinfo['index'], shapeinfo['edges'])
			shape.set('transform', translate(
				x = 0,
				y = i * 50 #TODO make smarter or configurable
			))
			svg.append(shape)
		return etree.tostring(svg, pretty_print=True, encoding='utf8')
			
	def render_connectors(self, shapes):
		svg = self.new_svg()
		#get all edges that have a connection and need conenctors made
		connectors = []
		for shape in shapes:
			for edge in shape['edges']:
				if edge['connected'] and edge['connectors']:
					connectors.append(edge)
		#sort connectors by angle
		connectors = sorted(connectors, key=lambda x:x['dihedral_angle'], reverse=False)
		for i, edge in enumerate(connectors):
			connector = self.make_connectors(edge)
			connector.set('transform', translate(
				x = i*2.5*(self.connector_width + self.material),
				y = 0
			))
			svg.append(connector)
		return etree.tostring(svg, pretty_print=True, encoding='utf8')

	def new_svg(self):
		svg = etree.Element('svg')
		svg.set('id', 'svg')
		svg.set('version', '1.1')
		svg.set('xmlns', 'http://www.w3.org/2000/svg')
		svg.set('width', '300')
		svg.set('height', '300')
		square = etree.Element('rect')
		square.set('x', '0')
		square.set('y', '0')
		square.set('width', '10')
		square.set('height', '10')
		square.set('fill', 'orange')
		square.set('stroke', 'none')
		svg.append(square)
		return svg
