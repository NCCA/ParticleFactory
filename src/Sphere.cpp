#include "Sphere.h"
#include "Emitter.h"
#include <cmath>
#include <ngl/Util.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Random.h>
#include <ngl/ShaderLib.h>



Sphere::Sphere(
                ngl::Vec3 _pos,
                ngl::Vec3 _dir,
                ngl::Vec4 _c,
                const std::string &_shaderName,
                Emitter *_parent

							  ) :
                  Particle(
                            _pos,
                            _dir,
                            _c,
													  _shaderName,
                            _parent
                          )
{
  m_emitAngle=360.0f;
  // get the angles for emission
  GLfloat theta=ngl::radians(ngl::Random::randomNumber(m_emitAngle));
  GLfloat phi=ngl::radians(ngl::Random::randomNumber(m_emitAngle));
  // set the
  m_dir.m_x=sinf(theta)*cosf(phi);
  m_dir.m_y=sinf(theta)*sinf(phi);
  m_dir.m_z=cosf(theta);
  m_dir.normalize();
  m_radius=ngl::Random::randomPositiveNumber(1.2f)+0.1f;
}


void Sphere::draw() const
{
  ngl::ShaderLib::use(m_shaderName);
  ngl::ShaderLib::setUniform("Colour",m_colour);
  ngl::Transformation t;
  t.setPosition(m_pos);
  t.setScale(m_radius,m_radius,m_radius);
  ngl::Mat4 MVP=m_parent->getCamera()->project *
                m_parent->getCamera()->view *
                m_parent->getGlobalTransform() *
                t.getMatrix();
  ngl::ShaderLib::setUniform("MVP",MVP);
  // draw the sphere
  ngl::VAOPrimitives::draw("sphere");

}


void Sphere::update()
{
// more the particle by adding the Direction vector to the position
	
  m_pos+=m_dir;
  ++m_life;
  if(m_life > m_maxLife)
	{
    m_life=0.0;
    m_pos=m_parent->getPos();

    m_maxLife=ngl::Random::randomPositiveNumber(80);
    GLfloat theta=ngl::radians(ngl::Random::randomNumber(m_emitAngle));
    GLfloat phi=ngl::radians(ngl::Random::randomNumber(m_emitAngle));
    m_dir.m_x=sinf(theta)*cosf(phi);
    m_dir.m_y=sinf(theta)*cosf(theta);
    m_dir.m_z=cosf(theta);
    m_dir.normalize();
    m_radius=ngl::Random::randomPositiveNumber(1.2f)+0.01f;

	}
}



 Sphere::~Sphere()
{
   std::cout<<"Sphere dtor\n";
}

