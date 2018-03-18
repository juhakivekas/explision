Explision
---------
This program  will explode your  models for you  so that you  can then
build  them  back  together  from magnificent  lasercut  triangles  of
plywood, acrylic or MDF. The tool  is meant for making physical models
of any 3D model.

Running
-------
Using explision is as simple as this:

	python3 explision.py config.json yourmodel.ply

All file formats supported by OpenMesh can be used as input.

Configuration
-------------
There  are  two  mandatory  configurations   that  are  also  the  two
most  important ones.  The defaults  of all  other configurations  are
calculated based on material and kerf.

- `material` This is the thickness of the material in millimeters. The
more accurate  this is,  the tighter  your model will  be, so  use two
decimals of accuracy.

- `kerf` This is the thickness of the cut that the laser does, usually
between 0.05 and 0.25. This is harder to measure and will depend a lot
on the material used as well as the printer model and settings.

There are  some less  obvious tweaks  that can be  done, but  that are
really useful when needed:

- `socket_spacing` The distance between  sockets for the connectors of
an edge. Defaults to ten times the material thickness.

- `socket_margin` The  lower limit of how  close a socket can  be to a
shape corner. Defaults to `socket_spacing`.

- `socket_inset` The distance from the connector socket to the edge of
the  face.  Defaults  to  material thickness.  This  does  not  affect
connector shape in any way.

-  `connector_width`  The width  of  the  connector in  the  direction
perpendicular to the face edge. Defaults to material thickness.

-  `connector_margin` The  distance between  duplicate connectors.  If
connectors overlap, then try playing with this.

Printing
--------
Open the SVG files from  the `design_files` directory in your favorite
vector graphics editor, like Inkscape. Then combine all the pieces and
do some manual ordering if needed.

- The red lines  should be cut first, with a low  power setting on the
laser because they are only  documenting the pieces and their relative
positions. This means they only need  to leave a mark, not cut through
the material.

- The  blue lines  should be  cut next  to avoid  parts moving  in the
middle of the cutting.

- The black borders should be cut last.

There is  probably a "select by  color" tool in your  editor that will
make printing  only parts at  a time easy. Make  sure not to  move the
material on the printing bed between rounds.

Constructing
------------
The numbers on  the triangles have meaning. The markings  are a way to
document the  way the  pieces go  together. Let's look  at this  by an
example:

![Example of a triangle](./doc/example_trianlge.png)

Now this is  triangle number ten in the model,  and it has connections
to triangles 14,  3 and 18. The connections start  above the arrow and
we count in the direction of the arrow. The other column of numbers is
the  angle of  the connector  that should  be used  for that  specific
connection.

The  top edge  is connected  to triangle  14 with  a connector  for 38
degrees. The  triangle 14 will have  a similar printout so  we'll know
what edge to connect to. The next edge clockwise from the first one is
connected to  triangle number 3 with  a degree of 36  degrees. Finally
the last edge connects to piece 18 with an angle of 31 degrees.

Constructing the model might take some time but it's fun, a bit like a
jigsaw  puzzle. A  good tip  is to  start with  sorting the  connector
pieces  into piles  for 0-10,  10-20, 20-30,  etc. degrees,  it really
speeds things up. Also note that negative degrees are not a bug, but a
feature.

Common issues
------------
- **dpi:**  All graphics software  seems to  use a different  dpi when
converting SVGs into their internal viewing formats. There's an orange
10mm reference square in the corner of the files so you can check that
the scale of things is right.

- **out of  scale meshes:** The units in the  mesh being processed are
always interpreted  as being millimeters.  Make sure your model  is of
appropriate size before processing it.  MeshLab has a `Measuring Tool`
in the tool panel that can measure the size of models.

-  **incorrect  kerf** If  the  connectors  are  too loose  of  tight,
the  kerf  measurement is  probably  off.  Take  a caliper,  run  some
experiments, and do some math.

- **duplicate vertices:** Some 3D  modeling software save their models
with duplicate  vertices. Open mesh works  with tightly interconnected
meshes and does not do any deduplication, so you need to fix the model
before processing it  with explision. In MeshLab  deduplication can be
done  by  running  `Filters  → Cleaning  and  repairing  →  Remove
Duplicated Vertex`.

-  **inverted faces**  Sometimes  models have  faces  that are  facing
inwards or that are facing  in inconsistent directions. Check that the
face normals are facing outwards from the model. MeshLab has an option
to view normals in `Render → Show Face Normals`.

-  **non-planar  polygons** Qads  and  other  non triangle  faces  are
expected to be planar. If there are  faces where all points are not in
a plane,  then there will  be glitches.  Make quads planar  or convert
everything to triangle meshes.

Limitations
-----------
Building  the model  is  not always  as quick  as  one might  imagine.
Anything with more than 20 faces is going to take more than an hour to
build and  anything with dimensions larger  than half a meter  is best
done with more than one pair of hands. Large structures, such as those
that  fit a  human, are  not  rigid. Even  if our  modeling and  shape
generation  would be  perfect, the  truth is  that materials  flex and
bend. Even if some things wobble, they are still structurally sane.

Very sharp angles may lead to rattling corners in the structure and it
might be  hard to make all  of the connectors  fir on a thin  strip of
material. Concave shapes should be working, but haven't been tested.

Contributing
------------
Feel free to  raise issues and make pull requests.  For any questions,
suggestions or  offered help send  me mail at  guth.smash@gmail.com or
direct messages on twitter to `@_guttula`

References
----------
- [OpenMesh](https://www.openmesh.org/)
is a generic and efficient  polygon mesh data structure that explision
uses for measuring and handling of the mesh.

- [SVG](https://developer.mozilla.org/en-US/docs/Web/SVG/Element)
is an open  vector graphics format that explision uses  as it's output
media.

- [lxml](http://lxml.de/)
is an XML processing library for python that is used to create the SVG
files.
