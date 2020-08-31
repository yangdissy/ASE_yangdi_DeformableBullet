#include "CollisionObject.h"
#include "CollisionShape.h"
#include <gtest/gtest.h>
#include <iostream>
#include <ngl/Vec3.h>
#include "NGLScene.h"
#include "PhysicWorld.h"

using namespace::testing;


TEST(NGLScene,createVAOforPig)
{
    NGLScene window;
    window.createVAOforPig("pig","models/Testobjs_pig.obj",0,0.2f);
    auto name = window.getAddedobject(0)->name();
    EXPECT_EQ(name,"pig");
}


TEST(NGLScene,createVAOforRubbertoy)
{
    NGLScene window;
    window.createVAOforRubbertoy("rubbertoy","models/Testobjs_rubbertoy.obj",1);
    auto name = window.getAddedobject(1)->name();
    EXPECT_EQ(name,"rubbertoy");
}
TEST(PhysicWorld,addBox)
{
    PhysicWorld *m_physics = nullptr;

    ngl::Vec3 pos;
    pos.null();
    m_physics->addBox("box",pos);
    auto name = m_physics->getBodyNameAtIndex(0);
    EXPECT_EQ(name,"box");
    EXPECT_TRUE(m_physics != nullptr);
}
TEST(PhysicWorld,addSphere)
{
    PhysicWorld *m_physics = nullptr;

    ngl::Vec3 pos;
    pos.null();
    m_physics->addSphere("sphere",pos);
    auto name = m_physics->getBodyNameAtIndex(0);
    EXPECT_EQ(name,"sphere");
    EXPECT_TRUE(m_physics != nullptr);
}
TEST(PhysicWorld,addMesh)
{
    PhysicWorld *m_physics = nullptr;
    CollisionObject obj = CollisionObject("pig","models/Testobjs_pig.obj");
    ngl::Vec3 pos;
    pos.null();
    m_physics->addMesh(obj,pos);
    auto name = m_physics->getBodyNameAtIndex(0);
    EXPECT_EQ(name,"pig");
    EXPECT_TRUE(m_physics != nullptr);
}
TEST(PhysicWorld,addGroundPlane)
{
    PhysicWorld *m_physics = nullptr;

    ngl::Vec3 pos;
    pos.null();
    m_physics->addGroundPlane(pos);
    auto name = m_physics->getBodyNameAtIndex(0);
    EXPECT_EQ(name,"groundPlane");
    EXPECT_TRUE(m_physics != nullptr);
}
