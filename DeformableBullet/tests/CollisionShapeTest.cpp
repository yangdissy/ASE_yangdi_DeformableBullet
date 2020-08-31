#include "CollisionObject.h"
#include "CollisionShape.h"
#include <gtest/gtest.h>
#include <iostream>
#include <ngl/Vec3.h>
using namespace ::testing;

TEST(CollisionShape,defaultCtor)
{
   auto obj = CollisionShape::instance();
   EXPECT_TRUE(obj != nullptr);
}
TEST(CollisionShape,addBox)
{
   auto obj = CollisionShape::instance();
   obj->addBox("Box",1.0f,1.0f,1.0f);
   auto name = obj->getShape("Box")->getShapeType();
   EXPECT_EQ(name,BOX_SHAPE_PROXYTYPE);
}
TEST(CollisionShape,addMesh)
{
   auto obj = CollisionShape::instance();
   obj->addMesh("pig","models/Testobjs_pig.obj");
   auto name = obj->getShape("pig")->getShapeType();
   EXPECT_EQ(name,CONVEX_HULL_SHAPE_PROXYTYPE);
}
TEST(CollisionShape,SetSphere)
{
   auto obj = CollisionShape::instance();
   CollisionObject mesh = CollisionObject("pig","models/Testobjs_pig.obj");
   obj->SetSphere(mesh,2.0);
   auto name = obj->getShape("pig")->getShapeType();
   EXPECT_EQ(name,SPHERE_SHAPE_PROXYTYPE );
}
TEST(CollisionShape,SetBox)
{
   auto obj = CollisionShape::instance();
   CollisionObject mesh = CollisionObject("pig","models/Testobjs_pig.obj");
   obj->SetBox(mesh);
   auto name = obj->getShape("pig")->getShapeType();
   EXPECT_EQ(name,BOX_SHAPE_PROXYTYPE);
}
TEST(CollisionShape,SetCapsule)
{
   auto obj = CollisionShape::instance();
   CollisionObject mesh = CollisionObject("pig","models/Testobjs_pig.obj");
   obj->SetCapsule(mesh,1.0f,2.0f);
   auto name = obj->getShape("pig")->getShapeType();
   EXPECT_EQ(name,CAPSULE_SHAPE_PROXYTYPE);
}
TEST(CollisionShape,SetSphercial)
{
   auto obj = CollisionShape::instance();
   CollisionObject mesh = CollisionObject("pig","models/Testobjs_pig.obj");
   obj->SetSphercial(mesh,0.2f);
   auto name = obj->getShape("pig")->getShapeType();
   EXPECT_EQ(name,MULTI_SPHERE_SHAPE_PROXYTYPE);
}
TEST(CollisionShape,Greedyfulfill)
{
   auto obj = CollisionShape::instance();
   CollisionObject mesh = CollisionObject("pig","models/Testobjs_pig.obj");
   obj->Greedyfulfill(mesh,0.2f);
   size_t numpoints = obj->getPNumpoints();
   EXPECT_TRUE(numpoints != 0);

}
TEST(CollisionShape,getShape)
{
   auto obj = CollisionShape::instance();
   CollisionObject mesh = CollisionObject("pig","models/Testobjs_pig.obj");
   obj->SetSphercial(mesh,0.2f);
   auto name = obj->getShape("pig")->getShapeType();
   EXPECT_EQ(name,MULTI_SPHERE_SHAPE_PROXYTYPE);
}

