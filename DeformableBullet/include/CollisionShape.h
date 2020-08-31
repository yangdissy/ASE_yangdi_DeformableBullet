#pragma once
#include "CollisionObject.h"
#include<iostream>
#include<memory>
#include<ngl/Types.h>
#include<bullet/btBulletDynamicsCommon.h>
#include<unordered_map>


class CollisionShape
{
public:
    //initial the CollisionShape
    static CollisionShape *instance();
    CollisionShape()=default;
    ~CollisionShape()=default;
    CollisionShape(CollisionObject &_obj);
    //add Irregular object in the CollisionShape list
    void addMesh(const std::string & _name, const std::string &_objFilePath);
    //add box in the CollosinShape list
    void addBox(const std::string & _name, ngl::Real _w, ngl::Real _h, ngl::Real _d);
    //void addSphere(const std::string & _name, ngl::Real _rad);
    //set Sphere as the CollosinShape and add it in the the CollisionShape list
    void SetSphere(CollisionObject &_obj, ngl::Real _rad);
    //set Box as the CollosinShape and add it in the the CollisionShape list
    void SetBox(CollisionObject &_obj);
    //set Capsule as the CollosinShape and add it in the the CollisionShape list
    void SetCapsule(CollisionObject &_obj, ngl::Real _rad, ngl::Real _height);
    //set Cylinder as the CollosinShape and add it in the the CollisionShape list
    void SetCylinder(CollisionObject &_obj, ngl::Real _rad, ngl::Real _height);
    //set Cone as the CollosinShape and add it in the the CollisionShape list
    void SetCone(CollisionObject &_obj, ngl::Real _rad, ngl::Real _height);
    //choice Multi spheres as the CollosinShape and add it in the the CollisionShape list
    void SetSphercial(CollisionObject &_obj,float radius);
    //fullfil the space of Irregular object using points
    void Greedyfulfill(CollisionObject &_obj,float radius);
    //find the nearnest point, return index.
    size_t nearnestmeshpoint(ngl::Vec3 pos,std::vector<ngl::Vec3> &points);
//----------------------------------------------------------------------------------------------------------------------
    //perpare for editing Constrians
    size_t getPNumpoints(){return  m_numSpheres;}
    std::vector<btVector3> getSphericalPPosition(){return m_SpherePositioins;}
    std::vector<btScalar> getSphericalPradii(){return m_SpherePradii;}
    btCollisionShape* getShape(const std::string &_name);


private:

     std::vector<CollisionObject> m_objects;
     std::vector<btVector3> m_SpherePositioins;
     std::vector<btScalar> m_SpherePradii;
     size_t m_numSpheres = 0;
     std::unordered_map <std::string,std::unique_ptr<btCollisionShape>> m_shapes;
};
