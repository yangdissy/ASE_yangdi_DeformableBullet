#include "CollisionObject.h"
#include "CollisionShape.h"
#include <gtest/gtest.h>
#include <iostream>
#include <ngl/Vec3.h>
using namespace ::testing;


TEST(CollisionObject,defaultCtor)
{
     CollisionObject obj;
     EXPECT_EQ(obj.name(),"CollisionObject");
     EXPECT_EQ(obj.id(),0);
     EXPECT_EQ(obj.mass(),0);
}

TEST(CollisionObject,userCtor)
{
    CollisionObject obj("obj1","models/ball.obj",1,0.2f);
    ngl::Vec3 privet = {0,0,0};
    EXPECT_EQ(obj.name(),"obj1");
    EXPECT_EQ(obj.id(),1);
    EXPECT_EQ(obj.mass(),0.2f);
    EXPECT_NE(obj.pivot(),privet);

}

TEST(CollisionObject,importfile)
{
     CollisionObject obj;
     ngl::Vec3 dir = {0,0,0};
     obj.importfile("models/ball.obj");
     EXPECT_NE(obj.pivot(),dir);

}
TEST(CollisionObject,removefile)
{
     CollisionObject obj;
     obj.importfile("models/ball.obj");
     bool a = obj.removefile();
     EXPECT_EQ(a,true);
}

TEST(CollisionObject,SetVel)
{
    CollisionObject obj;
    ngl::Vec3 vel = {0,1,0};
    obj.SetVel(vel);
    EXPECT_EQ(obj.vel(),vel);
}
TEST(CollisionObject,SetMass)
{
    CollisionObject obj;
    obj.SetMass(1);
    EXPECT_EQ(obj.mass(),1);
}
TEST(CollisionObject,SetPivot)
{
    CollisionObject obj;
    ngl::Vec3 a = {0,1,2};
    obj.SetPivot(a);
    EXPECT_EQ(obj.pivot(),a);
}
TEST(CollisionObject,SetName)
{
    CollisionObject obj;
    std::string a = "obj02";
    obj.SetName(a);
    EXPECT_EQ(obj.name(),a);
}
TEST(CollisionObject,SetId)
{
    CollisionObject obj;
    unsigned int a = 5;
    obj.SetId(a);
    EXPECT_EQ(obj.id(),a);
}

