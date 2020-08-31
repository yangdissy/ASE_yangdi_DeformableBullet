#pragma once
#include<iostream>
#include<memory>
#include<ngl/Types.h>
#include<bullet/btBulletDynamicsCommon.h>
#include <ngl/Vec3.h>
#include "CollisionObject.h"
#include "CollisionShape.h"

class PhysicWorld
{
public:
    ///initialize the Physicworld
    PhysicWorld();
    ~PhysicWorld();
    ///reset the Physicworld
    void reset();

    ///set the Gravity
    inline void setGravity(float _x, float _y, float _z)
    {
      m_dynamicsWorld->setGravity(btVector3(_x,_y,_z));
    }
    inline void setGravity(ngl::Vec3 _g)
    {
      m_dynamicsWorld->setGravity(btVector3(_g.m_x,_g.m_y,_g.m_z));
    }
//----------------------------------------------------------------------------------------------------------------------
    ///get the name of RigidBody
    inline std::string getBodyNameAtIndex(unsigned int i) const{return m_bodies[i].name;}


    ///get the rigidbody TransformMatrix from bullet system
    ngl::Mat4 getTransformMatrix(unsigned int _index);
    ///get the rigidbody Position from bullet system
    ngl::Vec3 getPosition(unsigned int _index);
    ///add Groudplane into bullet system
    void addGroundPlane(const ngl::Vec3 &_pos);
    ///add an Irregular object into bullet system
    void addMesh(CollisionObject &_obj,const ngl::Vec3 &_pos);
    ///add an Box into bullet system
    void addBox(std::string _shapeName,const ngl::Vec3 &_pos);
    ///add an Box into bullet system
    void addSphere(std::string _shapeName,const ngl::Vec3 &_pos);
    ///set the step of simulation
    void step(float _time, float _step);
    ///get the number/id of object in bullet system
    size_t getNumCollisionObjects()const
    {
      return static_cast<size_t>(m_dynamicsWorld->getNumCollisionObjects());
    }
    ///get the CollisionShape of the object which works in bullet system
    int getCollisionShape(unsigned int _index) const;
//----------------------------------------------------------------------------------------------------------------------
/// prepare for constrains
    //struct the class of Constraints
    class PtpConstraints
    {
    public:
        PtpConstraints(btRigidBody* rb,const btVector3 &pivot);
        PtpConstraints(btRigidBody* rbA,btRigidBody* rbB,const btVector3& pivotInA, const btVector3& pivotInB);
        btTypedConstraint* GettypedConstraint() { return PConstraint; }
        void Constraints_Rigidbody(CollisionObject &_obj,std::vector<ngl::Vec3> &points);
        void Constraints_PlasticSoftbody();
        void Constraints_ElasticSoftbody();
    private:
        btPoint2PointConstraint* PConstraint;
    };
    //add Constrains into the rigidbody
    void AddConstraint(PtpConstraints* constraint);
//----------------------------------------------------------------------------------------------------------------------
private:

    typedef struct
    {
      std::string name;
      btRigidBody* body;
    }Body;

    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btBroadphaseInterface> m_overlappingPairCache ;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
    std::shared_ptr<btCollisionShape> m_groundShape;
    std::vector <Body> m_bodies;

};
