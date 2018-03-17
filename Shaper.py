#The shaper takes in data about edge lengths and angles between them and
#returns SVG objects that can be added to procucible SVG files. Since SVG
#is an XML based format, lcml is used to vreate content. Many paths are
#built manually in this code, so check out these links for details on
#what is going on:
#https://developer.mozilla.org/en-US/docs/Web/SVG/Element/path
#https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d
#https://www.w3.org/TR/SVG/paths.html
from lxml import etree
import math

#this is a cool font, even if the paths look quite raw :P
font_space = 0.5
font_paths = {
	'0':"m 0 3 l 1.5 -3 l 0 3 l -1.5 0 l 0 -3 l 1.5 0 ",
	'1':"m 0.75 0 l 0 3 m 0.75 -3 ",
	'2':"l 1.5 0 l 0 1.5 l -1.5 0 l 0 1.5 l 1.5 0 m 0 -3 ",
	'3':"l 1.5 0 l -1.5 1.5 l 1.5 0 l 0 1.5 l -1.5 0 m 1.5 -3 ",
	'4':"m 1.5 1.5 l -1.5 0 l 1.5 -1.5 l 0 3 m 0 -3 ",
	'5':"m 1.5 0 l -1.5 0 l 0 1.5 l 1.5 0 l 0 1.5 h -1.5 m 1.5 -3 ",
	'6':"m 1.5 0 l -1.5 0 l 0 3 l 1.5 0 l 0 -1.5 l -1.5 0 m 1.5 -1.5 ",
	'7':"l 1.5 0 l -1.5 3 m 1.5 -3 ",
	'8':"m 0 1.5 l 1.5 0 l 0 1.5 l -1.5 0 l 0 -3 l 1.5 0 l 0 1.5 m 0 -1.5 ",
	'9':"m 1.5 1.5 l -1.5 0 l 0 -1.5 l 1.5 0 l 0 3 l -1.5 0 m 1.5 -3 ",
	' ':"m 1.5 0 ",
	'-':"m 0 1.5 l 1.5 0 m 0 -1.5 ",
	'>':"l 1.5 1.5 l -1.5 1.5 m 1.5 -3 ",
	'.':"m 0.75 2.5 l 0 0.75 m 0.75 -3.25 "	
}

def vector_font(text):
	chars = 0
	d = moveto(-6, 0)
	for c in text:
		if c == '\n':
			d += moveto(-(1.5+font_space) * chars, 4, True)
			chars = 0
		else:
			d += font_paths[c]
			d += moveto(font_space, 0, True)
			chars += 1
	path = etree.Element('path')
	path.set('d', d)
	path.set('stroke', 'red')
	path.set('stroke-width', '0.2')
	path.set('fill', 'none')
	return path

def moveto(x, y, relative = False):
	command = 'm' if relative else 'M'
	return '{} {} {} '.format(command, x, y)

def lineto(x, y, relative = False):
	command = 'l' if relative else 'L'
	return '{} {} {} '.format(command, x, y)

def rotlineto(x, y, r):
	rx = x * math.cos(r) - y * math.sin(r)
	ry = x * math.sin(r) + y * math.cos(r)
	return lineto(rx, ry, True)
	

class Shaper():
	def __init__(self, config):
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
			d += lineto(0,  edge_w, True)
			d += lineto(-edge_h, 0, True)
			d += lineto(0, -edge_w, True)
			d += lineto( edge_h, 0, True)
			d += moveto(-edge_h/2, 0, True)
			# move to origin of next socket
			d += moveto(self.connector_spacing, 0, True)
		return d

	def make_sockets(self, edges):
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
		xpos = 0; ypos = 0; angle = 0
		d = moveto(0, 0)
		for edge in edges:
			xpos += math.cos(angle) * edge['length']
			ypos += math.sin(angle) * edge['length']
			angle += math.pi - edge['sector_angle']
			d += lineto(xpos, ypos)
		path = etree.Element('path')
		path.set('d', d)
		path.set('stroke', 'black')
		path.set('stroke-width', '0.2')
		path.set('fill', 'none')
		return path

	def shape(self, edges, index):
		outline = self.make_outline(edges)
		sockets = self.make_sockets(edges)
		notes = vector_font(self.make_note(edges, index))
		notes.set('transform', 'translate({x},{y})'.format(
			x = edges[0]['length']/2,
			y = self.connector_inset + self.connector_width
		))
		shape = etree.Element('g')
		shape.append(sockets)
		shape.append(outline)
		shape.append(notes)
		return shape

	def make_note(self, edges, index):
		text = '{:3d}-->\n'.format(index)
		for edge in edges:
			if edge['connected']:
				text += '{:2d}{:4d}\n'.format(
					edge['index'],
					int(180 * edge['dihedral_angle'] / math.pi))
			else:
				text += '------\n'
		return text

	def make_connectors(self, edge):
		connector_count = int(edge['length'] / self.connector_spacing)
		angle = edge['dihedral_angle']
		group = etree.Element('g')
		d = moveto(0, 2*self.material)
		d += lineto(0, -2*self.material, True)
		d += lineto(self.connector_width, 0, True)
		d += lineto(0, self.material, True)
		d += lineto(self.material, 0, True)
		d += rotlineto(  self.material, 0, angle)
		d += rotlineto(0,  -self.material, angle)
		d += rotlineto(  self.material, 0, angle)
		d += rotlineto(0, 2*self.material, angle)
		d += 'z'
		path = etree.Element('path')
		path.set('stroke', 'black')
		path.set('stroke-width', '0.2')
		path.set('fill', 'none')
		path.set('d', d)
		return path
		
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
