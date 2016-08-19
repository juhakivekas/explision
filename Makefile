OPENMESH=/home/juha/Repos/OpenMesh
OPENMESH_SRC=$(OPENMESH)/src
#We are building with static archives to make dependency management easier
#since the software will be compiled and ran by non-technical people.
OPENMESH_LIB=$(OPENMESH)/build/Build/lib/libOpenMeshCore.a

CGFLAGS=--std=c++11 -g -Wall -DOM_STATIC_BUILD -Wno-unused-variable
CPPC=g++
all:
	$(CPPC) $(CGFLAGS) -o OpenMesh_test src/OpenMesh_test.cpp src/jsoncpp.cpp $(OPENMESH_LIB) -I $(OPENMESH_SRC)

meshopen:
	$(CPPC) $(CGFLAGS) -o meshopen src/meshopen.cc $(OPENMESH_LIB) -I $(OPENMESH_SRC)
	
