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
from Font import expfont

def moveto(vec, relative = False):
	'''a path move command as described in SVG specifivation'''
	command = 'm' if relative else 'M'
	return '{} {} {} '.format(command, vec[0], vec[1])

def lineto(vec, relative = False):
	'''a path line command as described in SVG specifivation'''
	command = 'l' if relative else 'L'
	return '{} {} {} '.format(command, vec[0], vec[1])

def rot(x, y, r):
	'''applies a rotation matrix on a vector'''
	#makes connector drawing much cleaner
	#https://en.wikipedia.org/wiki/Rotation_matrix 
	rx = x * math.cos(r) - y * math.sin(r)
	ry = x * math.sin(r) + y * math.cos(r)
	return (rx, ry)
	
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
		return expfont(text)

	def make_edge_sockets(self, length):
		'''make the sockets for connectors for a single edge of a shape'''
		# XXX needs to be defensive (special cases for short edges)
		inset  = self.connector_inset  + self.kerf
		edge_w = self.connector_width  - self.kerf
		edge_h = self.connector_height - self.kerf
		socket_count = int(length / self.connector_spacing)
		x = (length - ((socket_count-1) * self.connector_spacing)) / 2
		#move to middle of top edge of first socket
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
			d += moveto((self.connector_spacing, 0), True)
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

	def shape(self, edges, index):
		'''creates a shape based on edge information'''
		outline = self.make_outline(edges)
		sockets = self.make_sockets(edges)
		notes   = self.make_note(edges, index)
		notes.set('transform', 'translate({x},{y})'.format(
			x = edges[0]['length']/2 -6,
			y = self.connector_inset + self.connector_width
		))
		shape = etree.Element('g')
		shape.append(sockets)
		shape.append(outline)
		shape.append(notes)
		return shape

	def make_connectors(self, edge):
		'''makes the connector for an edge of a specified length'''
		angle = edge['dihedral_angle']
		d = moveto((0, self.connector_width + self.material))
		d += lineto((0, -(self.connector_width + self.material)), True)
		d += lineto((self.connector_width - self.kerf, 0), True)
		d += lineto((0, self.material), True)
		d += lineto((self.material, 0), True)
		d += lineto(rot(self.material, 0, angle), True)
		d += lineto(rot(0, -self.material, angle), True)
		d += lineto(rot(self.connector_width - self.kerf, 0, angle), True)
		d += lineto(rot(0, self.connector_width + self.material, angle), True)
		d += 'z'
		path = etree.Element('path')
		path.set('stroke', 'black')
		path.set('stroke-width', '0.2')
		path.set('fill', 'none')
		path.set('d', d)
		connector_count = int(edge['length'] / self.connector_spacing)
		group = etree.Element('g')
		for i in range(connector_count):
			group.append(path)
		return group

	def dump_svg(self, elements):
		svg = etree.Element('svg')
		svg.set('id', 'svg')
		svg.set('version', '1.1')
		svg.set('xmlns', 'http://www.w3.org/2000/svg')
		svg.set('width', '300')
		svg.set('height', '300')
		for element in elements:
			svg.append(element)
		return etree.tostring(svg, pretty_print=True, encoding='utf8')
