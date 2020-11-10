#include "Torus.h"
#include <cmath>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include "Emitter.h"



Torus::Torus(
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

	m_rotX=ngl::Random::randomNumber(360);
	m_rotY=ngl::Random::randomNumber(360);
	m_rotZ=ngl::Random::randomNumber(360);

	m_rotUpdateX=ngl::Random::randomNumber(10);
	m_rotUpdateY=ngl::Random::randomNumber(10);
	m_rotUpdateZ=ngl::Random::randomNumber(10);

}


void Torus::draw() const
{

  ngl::ShaderLib::use(m_shaderName);
  ngl::ShaderLib::setUniform("Colour",m_colour);

  ngl::Transformation t;
  t.setPosition(m_pos);
  t.setRotation(m_rotX,m_rotY,m_rotZ);;
  ngl::Mat4 MVP=m_parent->getCamera()->project *
                m_parent->getCamera()->view *
                m_parent->getGlobalTransform() *
                t.getMatrix();

  ngl::ShaderLib::setUniform("MVP",MVP);

  ngl::VAOPrimitives::draw("torus");

}


void Torus::update()
{
// more the particle by adding the Direction vector to the position

  m_pos+=m_dir;
  ++m_life;
	m_rotX+=m_rotUpdateX;
	m_rotY+=m_rotUpdateY;
	m_rotZ+=m_rotUpdateZ;

  if(m_life > m_maxLife)
	{
    m_life=0.0;
    m_pos=m_parent->getPos();

    m_maxLife=ngl::Random::randomPositiveNumber(80);

		m_rotX=ngl::Random::randomNumber(360);
		m_rotUpdateX=ngl::Random::randomPositiveNumber(10);
		m_rotY=ngl::Random::randomNumber(360);
		m_rotUpdateY=ngl::Random::randomPositiveNumber(10);
		m_rotZ=ngl::Random::randomNumber(360);
		m_rotUpdateZ=ngl::Random::randomPositiveNumber(10);
	}
}


Torus::~Torus()
{
 // std::cout<<"Torus dtor\n";
}

