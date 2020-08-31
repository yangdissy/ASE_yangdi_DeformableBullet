#include "PhysicWorld.h"
#include "CollisionShape.h"
#include "CollisionObject.h"
#include <ngl/Obj.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <ngl/Mat4.h>

PhysicWorld::PhysicWorld()
{
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    m_collisionConfiguration.reset(  new btDefaultCollisionConfiguration());

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher.reset( new	btCollisionDispatcher(m_collisionConfiguration.get()));

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_overlappingPairCache.reset( new btDbvtBroadphase());

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver.reset(new btSequentialImpulseConstraintSolver);

    m_dynamicsWorld.reset(new btDiscreteDynamicsWorld(m_dispatcher.get(),m_overlappingPairCache.get(),
                                                      m_solver.get(),m_collisionConfiguration.get()));


}
void PhysicWorld::addGroundPlane(const ngl::Vec3 &_pos)
{
    m_groundShape.reset(new btStaticPlaneShape(btVector3(0,1,0),_pos.m_y));

    btTransform  groundTransform;
    groundTransform.setIdentity();
    {
        btScalar mass(0.0f);
        btVector3 localInertia(0.0f,0.0f,0.0f);
        btDefaultMotionState *myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,m_groundShape.get(),localInertia);
        rbInfo.m_rollingFriction = 1.0f;
        //rbInfo.m_spinningFriction = 1.0f;

        btRigidBody *body = new btRigidBody(rbInfo);
        m_dynamicsWorld->addRigidBody(body);
        //m_dynamicsWorld->addConstraint(btConeTwistConstraint(),1);
        Body ground;
        ground.name = "groundPlane";
        ground.body = body;
        m_bodies.push_back(ground);

    }
}
PhysicWorld::~PhysicWorld()
{
}
//----------------------------------------------------------------------------------------------------------------------
ngl::Mat4 PhysicWorld::getTransformMatrix(unsigned int _index)
{
    btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
    btRigidBody* body = btRigidBody::upcast(obj);
    if (body && body->getMotionState())
    {
        btTransform trans;

        body->getMotionState()->getWorldTransform(trans);
        float matrix[16];
        trans.getOpenGLMatrix(matrix);
        return ngl::Mat4( matrix[0],matrix[1],matrix[2],matrix[3],
                                             matrix[4],matrix[5],matrix[6],matrix[7],
                                             matrix[8],matrix[9],matrix[10],matrix[11],
                                             matrix[12],matrix[13],matrix[14],matrix[15]
                );
    }
    else return ngl::Mat4();
}

ngl::Vec3 PhysicWorld::getPosition(unsigned int _index)
{
    btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
    btRigidBody* body = btRigidBody::upcast(obj);
    if (body && body->getMotionState())
    {
        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);
        return ngl::Vec3(trans.getOrigin().getX(),
                                         trans.getOrigin().getY(),
                                         trans.getOrigin().getZ()
                                        );
    }
    else return ngl::Vec3();
}

//----------------------------------------------------------------------------------------------------------------------
void PhysicWorld::addMesh(CollisionObject &_obj,const ngl::Vec3 &_pos)
{
    btCollisionShape* colShape = CollisionShape::instance()->getShape(_obj.name());
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar	mass = _obj.mass();

    btVector3   pos = btVector3(_obj.pos().m_x+_pos.m_z,_obj.pos().m_y+_pos.m_y,_obj.pos().m_z+_pos.m_z);



    btVector3 localInertia(10,10,10);
    colShape->calculateLocalInertia(mass,localInertia);
    startTransform.setOrigin(pos);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setFriction(1.0);
    body->setRollingFriction(1.0);
    m_dynamicsWorld->addRigidBody(body);
    Body b;
    b.name=_obj.name();
    b.body=body;
    m_bodies.push_back(b);

}
void PhysicWorld::addBox(std::string _shapeName,const ngl::Vec3 &_pos)
{

    btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);



    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar	mass(1.f);


    btVector3 localInertia(10,10,10);
    colShape->calculateLocalInertia(mass,localInertia);
    startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = 0.1f;
    rbInfo.m_friction = 100.5f;
    rbInfo.m_additionalAngularDampingFactor=4.0;
    rbInfo.m_additionalDamping=true;
    btRigidBody* body = new btRigidBody(rbInfo);
    m_dynamicsWorld->addRigidBody(body);
    Body b;
    b.name=_shapeName;
    b.body=body;
    m_bodies.push_back(b);

}
void PhysicWorld::addSphere(std::string _shapeName,const ngl::Vec3 &_pos)
{
    //create a dynamic rigidbody

    btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar	mass(1.f);


    btVector3 localInertia(10,10,10);
    colShape->calculateLocalInertia(mass,localInertia);
    startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);

    btRigidBody* body = new btRigidBody(rbInfo);
    body->setFriction(1.0);
    body->setRollingFriction(1.0);


    m_dynamicsWorld->addRigidBody(body);
    //m_dynamicsWorld->addConstraint();
    Body b;
    b.name=_shapeName;
    b.body=body;
    m_bodies.push_back(b);

}
int PhysicWorld::getCollisionShape(unsigned int _index) const
{
    btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
    btCollisionShape *collisionShape = obj->getCollisionShape();

    return collisionShape->getShapeType();
}


void PhysicWorld::reset()
{
    for(unsigned int i=1; i<m_bodies.size(); ++i)
    {
        m_dynamicsWorld->removeRigidBody(m_bodies[i].body);
    }
    m_bodies.erase(m_bodies.begin()+1,m_bodies.end());


}
void PhysicWorld::step(float _time, float _step)
{
    m_dynamicsWorld->stepSimulation(_time,1);
}

//----------------------------------------------------------------------------------------------------------------------
//this part is mainly design for control the constraints between points

PhysicWorld::PtpConstraints::PtpConstraints(btRigidBody* rb,const btVector3 &pivot)
{
    PConstraint = new btPoint2PointConstraint(*rb, pivot);
}
PhysicWorld::PtpConstraints::PtpConstraints(btRigidBody* rbA,btRigidBody* rbB,const btVector3& pivotInA, const btVector3& pivotInB)
{
    PConstraint = new btPoint2PointConstraint(*rbA, *rbB, pivotInA, pivotInB);
}
void PhysicWorld::PtpConstraints::Constraints_Rigidbody(CollisionObject &_obj,std::vector<ngl::Vec3> &points)
{
    btCollisionShape* colShape = CollisionShape::instance()->getShape(_obj.name());
    if(colShape->isCompound())
    {
        for (size_t i=0;i<points.size();i++) {

        }
    }


}


