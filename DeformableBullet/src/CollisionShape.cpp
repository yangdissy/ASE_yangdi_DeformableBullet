#include "CollisionShape.h"
#include<iostream>
#include<ngl/Types.h>
#include<ngl/Vec3.h>
#include<ngl/Vec4.h>
#include <ngl/Obj.h>
#include<bullet/btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"

CollisionShape *CollisionShape::instance()
{
  // in C++ 11 we use this to avoid having to do a double check lock pattern
  static CollisionShape s_instance;

  return &s_instance;
}
////----------------------------------------------------------------------------------------------------------------------
//add box in the CollosinShape list
void CollisionShape::addBox(const std::string & _name, ngl::Real _w, ngl::Real _h, ngl::Real _d)
{
    btCollisionShape* shape = new btBoxShape(btVector3(_w/2.0f,_h/2.0f,_d/2.0f));
    m_shapes[_name].reset(shape);

}
//add Irregular object in the CollisionShape list
void CollisionShape::addMesh(const std::string & _name, const std::string &_objFilePath)
{
      CollisionObject _obj = CollisionObject(_name,_objFilePath);
      //btMultiSphereShape*  shape = new btMultiSphereShape();
      //create a dynamic rigidbody
      btConvexHullShape*  shape =  new btConvexHullShape();
      auto points=_obj.m_mesh->getVertexList();

      for(auto p : points)
      {
          shape->addPoint(btVector3(p.m_x,p.m_y,p.m_z));
      }
      m_shapes[_name].reset(shape);

}
//set Sphere as the CollosinShape and add it in the the CollisionShape list
void CollisionShape::SetSphere(CollisionObject &_obj, ngl::Real _rad)
{
    btCollisionShape* shape = new btSphereShape(btScalar(_rad/2.0f));
    m_shapes[_obj.name()].reset(shape);
}
//set Box as the CollosinShape and add it in the the CollisionShape list
void CollisionShape::SetBox(CollisionObject &_obj)
{
    auto bbox = _obj.m_mesh->getBBox();
    btCollisionShape* shape = new btBoxShape(btVector3(bbox.width()/2.0f,bbox.height()/2.0f,bbox.depth()/2.0f));
    m_shapes[_obj.name()].reset(shape);

}
//set Capsule as the CollosinShape and add it in the the CollisionShape list
void CollisionShape::SetCapsule(CollisionObject &_obj,ngl::Real _rad, ngl::Real _height)
{
    auto bbox = _obj.m_mesh->getBBox();
    btCollisionShape* shape = new btCapsuleShape(_rad,_height);
    m_shapes[_obj.name()].reset(shape);
}
//set Cylinder as the CollosinShape and add it in the the CollisionShape list
void CollisionShape::SetCylinder(CollisionObject &_obj, ngl::Real _rad, ngl::Real _height)
{
    btCollisionShape* shape = new btCylinderShape(btVector3(_rad,_height/2.0f,_rad));
    m_shapes[_obj.name()].reset(shape);
}
//set Cone as the CollosinShape and add it in the the CollisionShape list
void CollisionShape::SetCone(CollisionObject &_obj, ngl::Real _rad, ngl::Real _height)
{
    btCollisionShape* shape = new btConeShape(_rad,_height);
    m_shapes[_obj.name()].reset(shape);
}
////----------------------------------------------------------------------------------------------------------------------

//choice Multi spheres as the CollosinShape and add it in the the CollisionShape list
void CollisionShape::SetSphercial(CollisionObject &_obj,float radius)
{

    Greedyfulfill(_obj,radius);
    btMultiSphereShape *shape =  new btMultiSphereShape(&m_SpherePositioins[0],&m_SpherePradii[0],int(m_numSpheres));
    shape->setMargin(0.1f * radius);
    m_shapes[_obj.name()].reset(shape);
    m_SpherePositioins.clear();
    m_SpherePradii.clear();
    m_numSpheres = 0;


}
//fullfil the space of Irregular object using points
void CollisionShape::Greedyfulfill(CollisionObject &_obj,float radius)
{
     auto points = _obj.m_mesh->getVertexList();
     auto centerpoint = _obj.m_mesh->getCenter();
     auto normal = _obj.m_mesh->getNormalList();


     size_t stepx = size_t( (_obj.m_bbox.bbox_max.m_x - _obj.m_bbox.bbox_min.m_x)/ (radius * 2.0f));
     size_t stepy = size_t( (_obj.m_bbox.bbox_max.m_y - _obj.m_bbox.bbox_min.m_y)/ (radius * 2.0f) );
     size_t stepz = size_t( (_obj.m_bbox.bbox_max.m_z - _obj.m_bbox.bbox_min.m_z)/ (radius * 2.0f) );
     //std::cout<< stepx<<"  "<<stepy<<"  "<<stepz<<'\n';
     ngl::Vec3 startpoint;
     ngl::Vec3 minpoint;
     startpoint.m_x = centerpoint.m_x+radius - 0.5f*(_obj.m_bbox.bbox_max.m_x - _obj.m_bbox.bbox_min.m_x);
     startpoint.m_y = centerpoint.m_y+radius - 0.5f*(_obj.m_bbox.bbox_max.m_y - _obj.m_bbox.bbox_min.m_y);
     startpoint.m_z = centerpoint.m_z+radius - 0.5f*(_obj.m_bbox.bbox_max.m_z - _obj.m_bbox.bbox_min.m_z);
     //std::cout<< startpoint.m_x<<"  "<<startpoint.m_y<<"  "<<startpoint.m_z<<'\n';
     minpoint = startpoint;

     size_t nearpointindex = 0;


     for (size_t i=0;i<=stepx;i++) {
         startpoint.m_x += float(radius * i * 2.0f);

         for (size_t j=0;j<=stepy;j++) {
             startpoint.m_y += float(radius * j* 2.0f);

             for (size_t k=0;k<=stepz;k++) {

                 startpoint.m_z += float(radius * k* 2.0f);
                 nearpointindex = nearnestmeshpoint(startpoint,points);
                 ngl::Vec3 NearpointNormal;
                 ngl::Vec3 dir;
                 NearpointNormal.set(normal[nearpointindex].m_x,normal[nearpointindex].m_y,normal[nearpointindex].m_z);
                 dir.set(startpoint.m_x-points[nearpointindex].m_x,startpoint.m_y-points[nearpointindex].m_y,startpoint.m_z-points[nearpointindex].m_z);
                 dir.normalize();
                 NearpointNormal.normalize();
                 if(dir.dot(NearpointNormal) < 0  ){

                    m_SpherePositioins.push_back(btVector3(startpoint.m_x,startpoint.m_y,startpoint.m_z));
                    m_SpherePradii.push_back(btScalar(radius));
                    m_numSpheres++;
                    //std::cout<<m_numSpheres<<'\n';
                    //std::cout<< startpoint.m_x<<"  "<<startpoint.m_y<<"  "<<startpoint.m_z<<'\n';
                }
                 startpoint.m_z = minpoint.m_z;
             }
             startpoint.m_y = minpoint.m_y;
         }
         startpoint.m_x = minpoint.m_x;
     }



}
//find the nearnest point, return index.
size_t CollisionShape::nearnestmeshpoint(ngl::Vec3 pos,std::vector<ngl::Vec3> &points)
{
    size_t index =0;
    ngl::Vec3 dis;
    dis.set(pos.m_x - points[0].m_x,pos.m_x - points[0].m_x,pos.m_x - points[0].m_x);
    for (size_t i=1;i<points.size();i++) {
        ngl::Vec3 distance;
        distance.m_x = pos.m_x - points[i].m_x;
        distance.m_y = pos.m_y - points[i].m_y;
        distance.m_z = pos.m_z - points[i].m_z;
        if(distance.length()<dis.length())
        {
            dis.set(distance);
            index = i;
        }
     }

    return index;
}

////----------------------------------------------------------------------------------------------------------------------




//get the CollisionShape from list by name
btCollisionShape* CollisionShape::getShape(const std::string &_name)
{
    btCollisionShape *shape=nullptr;
    auto shapeIt=m_shapes.find(_name);
    // make sure we have a valid shader
    if(shapeIt!=m_shapes.end())
    {
        shape=shapeIt->second.get();
    }
    return shape;
}

