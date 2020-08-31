#include "CollisionObject.h"
#include<iostream>
#include<ngl/Types.h>
#include<ngl/Vec3.h>
#include<ngl/Vec4.h>
#include<bullet/btBulletDynamicsCommon.h>
#include <ngl/Obj.h>
//----------------------------------------------------------------------------------------------------------------------

//CollisionObject *CollisionObject::instance()
//{
//  // in C++ 11 we use this to avoid having to do a double check lock pattern
//  static CollisionObject o_instance;

//  return &o_instance;
//}


CollisionObject::CollisionObject(const std::string _name,const std::string &_filedir, unsigned int _id, float_t _mass)
{
    m_name = _name;
    m_id = _id;
    m_mass = _mass;
    m_mesh = std::make_unique<ngl::Obj> (_filedir);
    m_pos = m_mesh->getCenter();
    m_pivot = m_mesh->getCenter();


//----------------------------------------------------------------------------------------------------------------------
    //m_BBox = m_mesh->getBBox();


// donot know why ngl::BBox couldnot work. so create new bbox.
    m_bbox.cen_pos = m_mesh->getCenter();

    auto list = m_mesh->getVertexList();
    auto listmin = m_mesh->getVertexList();


    m_bbox.bbox_max.set(list[0].m_x,list[0].m_y,list[0].m_z);
        for (size_t i=1;i<list.size();i++) {

            //std::cout<< list[i].m_x<<"  "<<list[i].m_y<<"  "<<list[i].m_z<<'\n';
            if(list[i].m_x>m_bbox.bbox_max.m_x){
                m_bbox.bbox_max.m_x = list[i].m_x;
            }
            if(list[i].m_y>m_bbox.bbox_max.m_y){
                m_bbox.bbox_max.m_y = list[i].m_y;
            }
            if(list[i].m_z>m_bbox.bbox_max.m_z){
                m_bbox.bbox_max.m_z = list[i].m_z;
            }
        }

        m_bbox.bbox_min.set(listmin[0].m_x,listmin[0].m_y,listmin[0].m_z);
            for (size_t i=1;i<listmin.size();i++) {
                if(listmin[i].m_x<m_bbox.bbox_min.m_x){
                    m_bbox.bbox_min.m_x = listmin[i].m_x;
                }
                if(listmin[i].m_y<m_bbox.bbox_min.m_y){
                    m_bbox.bbox_min.m_y = listmin[i].m_y;
                }
                if(listmin[i].m_z<m_bbox.bbox_min.m_z){
                    m_bbox.bbox_min.m_z = listmin[i].m_z;
                }
            }

//----------------------------------------------------------------------------------------------------------------------


}
CollisionObject::CollisionObject(const std::string _name,const std::string &_filedir)
{
    m_name = _name;
    m_id = 0;
    m_mass = 1.0f;
    m_mesh = std::make_unique<ngl::Obj> (_filedir);
    m_pos = m_mesh->getCenter();
    m_pivot = m_mesh->getCenter();
    //m_BBox = m_mesh->getBBox();
//----------------------------------------------------------------------------------------------------------------------
    // donot know why ngl::BBox couldnot work. so create new bbox.
    m_bbox.cen_pos = m_mesh->getCenter();

    auto list = m_mesh->getVertexList();
    auto listmin = m_mesh->getVertexList();


    m_bbox.bbox_max.set(list[0].m_x,list[0].m_y,list[0].m_z);
        for (size_t i=1;i<list.size();i++) {

            //std::cout<< list[i].m_x<<"  "<<list[i].m_y<<"  "<<list[i].m_z<<'\n';
            if(list[i].m_x>m_bbox.bbox_max.m_x){
                m_bbox.bbox_max.m_x = list[i].m_x;
            }
            if(list[i].m_y>m_bbox.bbox_max.m_y){
                m_bbox.bbox_max.m_y = list[i].m_y;
            }
            if(list[i].m_z>m_bbox.bbox_max.m_z){
                m_bbox.bbox_max.m_z = list[i].m_z;
            }
        }

        m_bbox.bbox_min.set(listmin[0].m_x,listmin[0].m_y,listmin[0].m_z);
            for (size_t i=1;i<listmin.size();i++) {
                if(listmin[i].m_x<m_bbox.bbox_min.m_x){
                    m_bbox.bbox_min.m_x = listmin[i].m_x;
                }
                if(listmin[i].m_y<m_bbox.bbox_min.m_y){
                    m_bbox.bbox_min.m_y = listmin[i].m_y;
                }
                if(listmin[i].m_z<m_bbox.bbox_min.m_z){
                    m_bbox.bbox_min.m_z = listmin[i].m_z;
                }
            }

//----------------------------------------------------------------------------------------------------------------------
}
void CollisionObject::importfile(const std::string &_objFilePath)
{
    m_mesh = std::make_unique<ngl::Obj> (_objFilePath);
    m_mesh->load(_objFilePath);
    std::cout<<m_mesh->isLoaded()<<'\n';
    m_pos = m_mesh->getCenter();
    m_pivot = m_mesh->getCenter();
    //m_BBox = m_mesh->getBBox();

//----------------------------------------------------------------------------------------------------------------------
    // donot know why ngl::BBox couldnot work. so create new bbox.
    m_bbox.cen_pos = m_mesh->getCenter();

    auto list = m_mesh->getVertexList();

    m_bbox.bbox_max.set(list[0]);
        for (size_t i=1;i<list.size();i++) {
            if(list[i].m_x>m_bbox.bbox_max.m_x){
                m_bbox.bbox_max.m_x = list[i].m_x;
            }
            if(list[i].m_y>m_bbox.bbox_max.m_y){
                m_bbox.bbox_max.m_y = list[i].m_y;
            }
            if(list[i].m_z>m_bbox.bbox_max.m_z){
                m_bbox.bbox_max.m_z = list[i].m_z;
            }
        }

        m_bbox.bbox_min.set(list[0]);
            for (size_t i=1;i<list.size();i++) {
                if(list[i].m_x<m_bbox.bbox_min.m_x){
                    m_bbox.bbox_min.m_x = list[i].m_x;
                }
                if(list[i].m_y<m_bbox.bbox_min.m_y){
                    m_bbox.bbox_min.m_y = list[i].m_y;
                }
                if(list[i].m_z<m_bbox.bbox_min.m_z){
                    m_bbox.bbox_min.m_z = list[i].m_z;
                }
            }


//----------------------------------------------------------------------------------------------------------------------
}
bool CollisionObject::removefile()
{
    m_mesh.release();
    if(m_mesh == nullptr){
        return 1;
    }
    return 0;
}
void CollisionObject::SetVel(ngl::Vec3 _vel)//set Vel
{
    m_vel = _vel;
}
void CollisionObject::SetMass(float_t _mass)//set Mass
{
    m_mass = _mass;
}
void CollisionObject::SetName(std::string _name)//set Name
{
    m_name = _name;
}
void CollisionObject::SetPivot(ngl::Vec3 _pivot)//set centerpos
{
    m_pivot = _pivot;
}
void CollisionObject::SetId(unsigned int _id)//set id.
{
    m_id = _id;

}


////----------------------------------------------------------------------------------------------------------------------

std::string CollisionObject::name()noexcept
{
    return m_name;
}
ngl::Vec3 CollisionObject::pos()noexcept
{
    return m_pos;
}
ngl::Vec3 CollisionObject::vel()noexcept
{
    return m_vel;
}
ngl::Vec4 CollisionObject::orient()noexcept
{
    return m_orient;
}
ngl::Vec3 CollisionObject::pivot()noexcept
{
    return m_pivot;
}
float_t CollisionObject::mass()noexcept
{
    return m_mass;
}
unsigned int CollisionObject::id()noexcept
{
    return m_id;
}
//ngl::BBox CollisionObject::bbox()noexcept
//{
//   return m_BBox;
//}
//ngl::Vec3 CollisionObject::FindBboxMax(std::vector <ngl::Vec3> &_points)
//{
//    ngl::Vec3 max;
//    max.set(_points[0]);
//    for (size_t i=1;i<_points.size();i++) {
//        if(_points[i].m_x>max.m_x){
//            max.m_x = _points[i].m_x;
//        }
//        if(_points[i].m_y>max.m_y){
//            max.m_y = _points[i].m_y;
//        }
//        if(_points[i].m_z>max.m_z){
//            max.m_z = _points[i].m_z;
//        }
//    }
//    return max;
//}
