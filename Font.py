from lxml import etree
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

def expfont(text):
	'''stand-alone function to make that sweet lasercutter font'''
	chars = 0
	d = "M 0 0"
	for c in text:
		if c == '\n':
			d += "m {} {}".format(-(1.5+font_space) * chars, 4)
			chars = 0
		else:
			d += font_paths[c]
			d += "m {} {}".format(font_space, 0)
			chars += 1
	path = etree.Element('path')
	path.set('d', d)
	path.set('stroke', 'red')
	path.set('stroke-width', '0.2')
	path.set('fill', 'none')
	return path
