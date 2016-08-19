#ifndef ExpMesh_h
#define ExpMesh_h

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>

struct ExpTraits : public OpenMesh::DefaultTraits
{
/*
	template <class Base, class Refs>
	struct FaceT : public Base
	{
		//An explision triangle.
		//This is the basic shape that all physical manufacturing will be
		//done with. It's a copy of the the face, projected to a 2D shape.
		//We need a copy because some augmented reality applications might
		//need both versions.
		OpenMesh::Vec2f corner[3];
		//The center of a face will be defined as the center of it's
		//incircle. This will be used to match the 2 and 3 dimensional
		//faces with eachother.
		OpenMesh::Vec2f incircle2;
		OpenMesh::Vec3f incircle3;
	};
*/
	//the explision software needs some special traits for storing the
	//angles between faces and the length of the face edges.
	template <class Base, class Refs>
	struct EdgeT : public Base
	{
	private:
		float angle_;	//the angle between the two faces sharing an edge
		float length_;	//the length of the edge in whatever unit the mesh uses
	public:
		//override the constructor
		EdgeT() : angle_(0), length_(0) { }
		//getters and setters to avoid making the traits public
		float angle()  const { return angle_;  }
		float length() const { return length_; }
		void set_angle( float a_) { angle_  = a_; }
		void set_length(float l_) { length_ = l_; }
	};
};

typedef OpenMesh::TriMesh_ArrayKernelT<ExpTraits> ExpMesh;

#endif//ExpMesh_h
