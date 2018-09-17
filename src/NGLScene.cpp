#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Image.h>



NGLScene::NGLScene()
{
  setTitle("Qt5 Simple NGL Demo");
  m_emitterPos.set(0,0,0);
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL( int _w, int _h )
{
  m_cam.project=ngl::perspective( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(20,20,35);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam.view=ngl::lookAt(from,to,up);
  m_cam.project=ngl::perspective(45,720.0f/576.0f,0.5f,150);
  // now to load the shader and set the values
  // grab an instance of shader manager
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
  ngl::Vec4 lightPos(-2.0f,5.0f,2.0f,0.0f);
  shader->setUniform("light.position",lightPos);
  shader->setUniform("light.ambient",0.0f,0.0f,0.0f,1.0f);
  shader->setUniform("light.diffuse",1.0f,1.0f,1.0f,1.0f);
  shader->setUniform("light.specular",0.8f,0.8f,0.8f,1.0f);
  // gold like phong material
  shader->setUniform("material.ambient",0.274725f,0.1995f,0.0745f,0.0f);
  shader->setUniform("material.diffuse",0.75164f,0.60648f,0.22648f,0.0f);
  shader->setUniform("material.specular",0.628281f,0.555802f,0.3666065f,0.0f);
  shader->setUniform("material.shininess",51.2f);
  shader->setUniform("viewerPos",from);
  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",1.0,20);

  prim->createTorus("torus",0.2f,0.8f,20,20);
  // create the default particle inital position
  m_emitter.reset( new Emitter(m_emitterPos,1000,&m_cam) );
  m_text.reset( new ngl::Text(QFont("Arial",14)));
  m_text->setScreenSize(width(),height());
  startTimer(40);

}




void NGLScene::paintGL()
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  glViewport(0,0,m_win.width,m_win.height);
  // clear the screen and depth buffer
  shader->use("Phong");

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_win.spinXFace);
  rotY.rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
  m_emitter->setGlobalTransform(m_mouseGlobalTX);
  m_emitter->draw();
  QString text;
  text.sprintf("number of particles %d",m_emitter->getNumParticles());
  m_text->setColour(1,1,1);
  m_text->renderText(10,10,text);
  m_text->renderText(10,30,QString("1 add Sphere 2 add Teapot"));
  m_text->renderText(10,50,QString("r remove last particle"));
  m_text->renderText(10,70,QString("z,x,c,v change emit angle of sphere"));
  m_text->renderText(10,90,QString("Space clears all particles"));
}

void NGLScene::addParticles( int _type)
{
switch(_type)
{
  case 0 : m_emitter->addParticle(ParticleType::SPHERE); break;
  case 1 : m_emitter->addParticle(ParticleType::TEAPOT); break;

}

}


//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
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
  case Qt::Key_1 : addParticles(0); break;
  case Qt::Key_2 : addParticles(1); break;
  case Qt::Key_Z : m_emitter->updateEmitAngle(30); break;
  case Qt::Key_X : m_emitter->updateEmitAngle(90); break;
  case Qt::Key_C : m_emitter->updateEmitAngle(200); break;
  case Qt::Key_V : m_emitter->updateEmitAngle(360); break;
  case Qt::Key_Space : m_emitter->clearParticles(); break;
  case Qt::Key_R : m_emitter->removeParticle(); break;
  case Qt::Key_Left : updateEmitterPos(-1,0,0); break;
  case Qt::Key_Right : updateEmitterPos(1,0,0); break;
  case Qt::Key_Up : updateEmitterPos(0,1,0); break;
  case Qt::Key_Down : updateEmitterPos(0,-1,0); break;
  case Qt::Key_I : updateEmitterPos(0,0,-1); break;
  case Qt::Key_O : updateEmitterPos(0,0,1); break;
  case Qt::Key_9 :
    ngl::Image::saveFrameBufferToFile("image.png",0,0,m_win.width,m_win.height,ngl::Image::ImageModes::RGB);
  break;
  case Qt::Key_8 :
    ngl::Image::saveFrameBufferToFile("image2.png",250,250,550,550,ngl::Image::ImageModes::RGB);
  break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    update();
}

void NGLScene::timerEvent( QTimerEvent *_event)
{
  NGL_UNUSED(_event);
	m_emitter->update();
	update();
}
void NGLScene::updateEmitterPos(float _dx, float _dy, float _dz)
{
  m_emitterPos.m_x+=_dx;
  m_emitterPos.m_y+=_dy;
  m_emitterPos.m_z+=_dz;
  m_emitter->setPos(m_emitterPos);
}

