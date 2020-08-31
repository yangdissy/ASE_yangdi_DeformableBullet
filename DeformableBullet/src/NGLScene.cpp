#include <QMouseEvent>
#include <QGuiApplication>
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/Random.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <iostream>
#include <QMouseEvent>
#include "PhysicWorld.h"
//#include "CollisionShape.h"
#include <ngl/VAOFactory.h>
#include <string>
NGLScene::NGLScene()
{
    setTitle("DeformableBullet NGL");
    m_physics = new PhysicWorld();
    //m_physics->reset();
    m_physics->setGravity(ngl::Vec3(0.0f,-10.0f,0.0f));
    m_physics->addGroundPlane(ngl::Vec3(0.0f,0.0f,0.0f));

}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_physics;
  //std::cout<<"delete the point m_physics\n";
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::addCube()
{
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  //pos*=10.0f;
  pos.m_y=10.0f;
  //pos.set(0.0f,10.0f,0.0f);
  m_physics->addBox("box",pos);
}
void NGLScene::addpig()
{
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  //pos*=10.0f;
  pos.m_y=5.0f;
  //pos.set(0.0f,10.0f,0.0f);
  m_physics->addMesh(*m_otype[0],pos);
}
void NGLScene::addrubbertoy()
{
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  //pos*=10.0f;
  pos.m_y=7.0f;
  //pos.set(0.0f,10.0f,0.0f);
  m_physics->addMesh(*m_otype[1],pos);


}

//----------------------------------------------------------------------------------------------------------------------


void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_project = ngl::perspective(45.0f, float(_w) / _h, 0.1f, 80.0f);
}

//----------------------------------------------------------------------------------------------------------------------


void NGLScene::initializeGL()
{
    // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
    // be done once we have a valid GL context but before we call any GL commands. If we dont do
    // this everything will crash
    ngl::NGLInit::instance();
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);			   // Grey Background
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);

    //  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    //  (*shader)["nglDiffuseShader"]->use();
    //  shader->setUniform("Colour",1.0f,1.0f,0.0f,1.0f);
    //  shader->setUniform("lightPos",1.0f,1.0f,1.0f);
    //  shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

    auto shader = ngl::ShaderLib::instance();
    (*shader)["nglDiffuseShader"]->use();
    shader->setUniform("Colour",1.0f,1.0f,0.0f,1.0f);
    shader->setUniform("lightPos",1.0f,1.0f,1.0f);
    shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);


    ngl::Vec3 from(0.0f,5.0f,15.0f);
    ngl::Vec3 to(0.0f,0.0f,0.0f);
    ngl::Vec3 up(0.0f,1.0f,0.0f);
    m_view=ngl::lookAt(from,to,up);
    ngl::VAOPrimitives::instance()->createLineGrid("ground",40,40,100);
    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    prim->createSphere("sphere",0.5f,40.0f);


    CollisionShape *shapes=CollisionShape::instance();
    shapes->addBox("box",1.0f,1.0f,1.0f);

    createVAOforPig("pig","models/Testobjs_pig.obj",0,0.2f);

    createVAOforRubbertoy("rubbertoy","models/Testobjs_rubbertoy.obj",1);


    startTimer(10);



}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::paintGL()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_win.width,m_win.height);
    //ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    //shader->use(ngl::nglColourShader);
    auto prim = ngl::VAOPrimitives::instance();
    ngl::Mat4 rot_X,rot_Y;
    ngl::Transformation tx;
    rot_X.rotateX(m_win.spinXFace);
    rot_Y.rotateY(m_win.spinYFace);
    m_globalMouseTX = rot_X * rot_Y;
    m_globalMouseTX.m_30 = m_modelPos.m_x;
    m_globalMouseTX.m_31 = m_modelPos.m_y;
    m_globalMouseTX.m_32 = m_modelPos.m_z;
    //  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    //  (*shader)["nglDiffuseShader"]->use();
    auto shader = ngl::ShaderLib::instance();
    //shader->use(ngl::nglColourShader);
    shader->use(ngl::nglDiffuseShader);

    m_bodyTransform.identity();
    shader->setUniform("Colour",0.5f,0.0f,0.5f,1.0f);
    loadMatricesToShader();
    prim->draw("ground");


    size_t bodies=m_physics->getNumCollisionObjects();
    for(unsigned int i=1; i<bodies; i++)
    {
        std::string name = m_physics->getBodyNameAtIndex(i);
        m_bodyTransform=m_physics->getTransformMatrix(i);
        loadMatricesToShader();//get the
        shader->setUniform("Colour",0.0f,0.0f,1.0f,1.0f);

        if(m_physics->getCollisionShape(i) == BOX_SHAPE_PROXYTYPE){
            shader->setUniform("Colour",1.0f,0.0f,0.0f,1.0f);
            prim->draw("cube");
        }
        if(name == "pig")//draw pig
        {
            shader->setUniform("Colour",0.0f,1.0f,0.0f,1.0f);
            m_otype[0]->m_mesh->draw();
        }
        if(name == "rubbertoy")//draw RubberToy
        {
            shader->setUniform("Colour",0.0f,0.0f,1.0f,1.0f);
            m_otype[1]->m_mesh->draw();
        }
    }

 }

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::createVAOforPig(const std::string  &_name, const std::string &_objFilePath,size_t index,float radius)
{
    CollisionShape *shapes=CollisionShape::instance();
    m_otype[index] = new CollisionObject(_name,_objFilePath);
//Choice the ConvexHullShape as the CollosinShape
    //shapes->addMesh(_name,_objFilePath);

//Choice the Multi spheres as the CollosinShape
    shapes->SetSphercial(*m_otype[index],radius);
    m_otype[index]->m_mesh->createVAO();

}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::createVAOforRubbertoy(const std::string & _name, const std::string &_objFilePath,size_t index)
{
    CollisionShape *shapes=CollisionShape::instance();
    m_otype[index] = new CollisionObject(_name,_objFilePath);
//Choice the ConvexHullShape as the CollosinShape
    //shapes->addMesh(_name,_objFilePath);
//Choice the Multi spheres as the CollosinShape
    shapes->SetSphercial(*m_otype[index],0.1f);
    m_otype[index]->m_mesh->createVAO();
}


//----------------------------------------------------------------------------------------------------------------------

void NGLScene::loadMatricesToShader()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;

    MV = m_view * m_globalMouseTX * m_bodyTransform;
    MVP = m_project * MV;

    normalMatrix=MV;
    normalMatrix.inverse().transpose();
    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
}


 //----------------------------------------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------------------------------------
 void NGLScene::mouseMoveEvent( QMouseEvent* _event )
 {
   // note the method buttons() is the button state when event was called
   // that is different from button() which is used to check which button was
   // pressed when the mousePress/Release event is generated
   if ( m_win.rotate && _event->buttons() == Qt::LeftButton )
   {
     int diffx = _event->x() - m_win.origX;
     int diffy = _event->y() - m_win.origY;
     m_win.spinXFace += static_cast<int>( 0.5f * diffy );
     m_win.spinYFace += static_cast<int>( 0.5f * diffx );
     m_win.origX = _event->x();
     m_win.origY = _event->y();
     update();
   }
   // right mouse translate code
   else if ( m_win.translate && _event->buttons() == Qt::RightButton )
   {
     int diffX      = static_cast<int>( _event->x() - m_win.origXPos );
     int diffY      = static_cast<int>( _event->y() - m_win.origYPos );
     m_win.origXPos = _event->x();
     m_win.origYPos = _event->y();
     m_modelPos.m_x += INCREMENT * diffX;
     m_modelPos.m_y -= INCREMENT * diffY;
     update();
   }
 }

 //----------------------------------------------------------------------------------------------------------------------
 void NGLScene::mousePressEvent( QMouseEvent* _event )
 {
   // that method is called when the mouse button is pressed in this case we
   // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
   if ( _event->button() == Qt::LeftButton )
   {
     m_win.origX  = _event->x();
     m_win.origY  = _event->y();
     m_win.rotate = true;
   }
   // right mouse translate mode
   else if ( _event->button() == Qt::RightButton )
   {
     m_win.origXPos  = _event->x();
     m_win.origYPos  = _event->y();
     m_win.translate = true;
   }
 }

 //----------------------------------------------------------------------------------------------------------------------
 void NGLScene::mouseReleaseEvent( QMouseEvent* _event )
 {
   // that event is called when the mouse button is released
   // we then set Rotate to false
   if ( _event->button() == Qt::LeftButton )
   {
     m_win.rotate = false;
   }
   // right mouse translate mode
   if ( _event->button() == Qt::RightButton )
   {
     m_win.translate = false;
   }
 }

 //----------------------------------------------------------------------------------------------------------------------
 void NGLScene::wheelEvent( QWheelEvent* _event )
 {

   // check the diff of the wheel position (0 means no change)
   if ( _event->delta() > 0 )
   {
     m_modelPos.m_z += 2 * ZOOM;
   }
   else if ( _event->delta() < 0 )
   {
     m_modelPos.m_z -= 2 * ZOOM;
   }
   update();
 }

 //----------------------------------------------------------------------------------------------------------------------

 void NGLScene::keyPressEvent(QKeyEvent *_event)
 {
   // this method is called every time the main window recives a key event.
   // we then switch on the key value and set the camera in the NGLScene
   switch (_event->key())
   {
   // escape key to quite
   case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
   // turn on wirframe rendering
   case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
   // turn off wire frame
   case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
   // show full screen
   case Qt::Key_F : showFullScreen(); break;
   // show windowed
   case Qt::Key_N : showNormal(); break;

   case Qt::Key_1 : addCube(); break;
   case Qt::Key_2 : addpig(); break;
   case Qt::Key_3 : addrubbertoy(); break;
   default : break;
   }
   // finally update the GLWindow and re-draw
   //if (isExposed())
     update();
 }

//----------------------------------------------------------------------------------------------------------------------

 void NGLScene::resetSim()
 {
    m_physics->reset();
 }


 void NGLScene::timerEvent(QTimerEvent *)
 {
     m_physics->step(1.0f/60.0f,10);
     update();
 }
 void NGLScene::stepSim()
 {
    m_physics->step(1.0f/60.0f,10);
 }

