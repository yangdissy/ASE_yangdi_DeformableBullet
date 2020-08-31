#pragma once
#include<iostream>
#include<memory>
#include<ngl/Types.h>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <unordered_map>
//#include"CollisionShape.h"
#include <ngl/Obj.h>
//#include<bullet/btBulletDynamicsCommon.h>

class CollisionObject
{
public:
    CollisionObject() = default;
    ~CollisionObject() = default;
    CollisionObject(const std::string _name,const std::string &_filedir,unsigned int _id,float_t _mass);
    CollisionObject(const std::string _name,const std::string &_filedir);
    CollisionObject(const CollisionObject &)=default;
    CollisionObject & operator = (const CollisionObject &)= default;
    CollisionObject(CollisionObject &&) = default;
    CollisionObject & operator = (CollisionObject &&)= default;
//----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 pos() noexcept;//get position
    ngl::Vec3 vel() noexcept;//get vel
    ngl::Vec4 orient() noexcept;//get orient
    ngl::Vec3 pivot() noexcept;//get pivot
    std::string name() noexcept;//get name
    //ngl::BBox bbox()noexcept;//get bounding box
    float_t mass() noexcept;//get mass
    unsigned int id() noexcept;//get id
    void importfile(const std::string &_objFilePath);//import obj file
    bool removefile();//remove obj file
//----------------------------------------------------------------------------------------------------------------------
    void scale(float _x,float _y,float _z);// set scale
    void SetVel(ngl::Vec3 _vel);// set vel
    void SetMass(float_t _mass);//set mass
    void SetPivot(ngl::Vec3 _pivot);//set pivot
    void SetName(std::string _name);//set name
    void SetId(unsigned int _id);//set Id

    //struct the element of bbox
    struct bbox{
      ngl::Vec3 cen_pos;
      ngl::Vec3 bbox_max;
      ngl::Vec3 bbox_min;
    };

    struct bbox m_bbox;
    std::unique_ptr<ngl::Obj> m_mesh;

//----------------------------------------------------------------------------------------------------------------------
private:



    ngl::Vec3 m_pos = {0,0,0};
    ngl::Vec3 m_vel = {0,0,0};
    ngl::Vec4 m_orient = {0,0,0,0};
    ngl::Vec3 m_pivot = {0,0,0};
    std::string m_name = "CollisionObject";
    float_t m_mass = 0;
    unsigned int m_id = 0;
    //can not use the ngl::BBox element.
    //ngl::BBox m_BBox;


};
