OPENMESH=/home/juha/Repos/OpenMesh
OPENMESH_SRC=$(OPENMESH)/src
#We are building with static archives to make dependency management easier
#since the software will be compiled and ran by non-technical people.
OPENMESH_LIB=$(OPENMESH)/build/Build/lib/libOpenMeshCore.a

CGFLAGS=--std=c++11 -g -Wall -DOM_STATIC_BUILD -Wno-unused-variable 
CPPC=g++

explision:                      \
	src/main.o                  \
	src/jsoncpp.o               \
	src/struct/shape.o          \
	src/struct/vector.o         \
	src/module/make_shapes.o    \
	src/module/pack_shapes.o    \
	src/module/draw_shapes.o    \
	src/module/make_connectors.o\
	src/io/svg_io.o
	$(CPPC) $(CGFLAGS) -o $@ $^ $(OPENMESH_LIB)

%.o:%.cpp
	$(CPPC) $(CGFLAGS) -c -o $@ $^ -I $(OPENMESH_SRC)

%.o:%.c
	$(CPPC) $(CGFLAGS) -c -o $@ $^ -I $(OPENMESH_SRC) 

tester:src/tester.cpp
	$(CPPC) $(CGFLAGS) -o $@ $^ $(OPENMESH_LIB) -I $(OPENMESH_SRC)

clean:
	rm -f explision
	rm -f src/*.o
	rm -f src/*/*.o
	
