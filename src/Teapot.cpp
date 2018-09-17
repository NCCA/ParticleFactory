#include "Teapot.h"
#include <cmath>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include "Emitter.h"



Teapot::Teapot(
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
	ngl::Random *rng=ngl::Random::instance();

	m_rotX=rng->randomNumber(360);
	m_rotY=rng->randomNumber(360);
	m_rotZ=rng->randomNumber(360);

	m_rotUpdateX=rng->randomNumber(10);
	m_rotUpdateY=rng->randomNumber(10);
	m_rotUpdateZ=rng->randomNumber(10);

}


void Teapot::draw() const
{

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[m_shaderName]->use();
  shader->setUniform("Colour",m_colour);

  ngl::Transformation t;
  t.setPosition(m_pos);
  t.setRotation(m_rotX,m_rotY,m_rotZ);
  ngl::Mat4 MVP=m_parent->getCamera()->project *
                m_parent->getCamera()->view *
                m_parent->getGlobalTransform() *
                t.getMatrix();
  shader->setUniform("MVP",MVP);

  // get an instance of the VBO primitives for drawing
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  // draw the sphere
  prim->draw("teapot");

}


void Teapot::update()
{
// more the particle by adding the Direction vector to the position
	ngl::Random *rng=ngl::Random::instance();

  m_pos+=m_dir;
  ++m_life;
	m_rotX+=m_rotUpdateX;
	m_rotY+=m_rotUpdateY;
	m_rotZ+=m_rotUpdateZ;

  if(m_life > m_maxLife)
	{
    m_life=0.0;
    m_pos=m_parent->getPos();

    m_maxLife=rng->randomPositiveNumber(80);

		m_rotX=rng->randomNumber(360);
		m_rotUpdateX=rng->randomPositiveNumber(10);
		m_rotY=rng->randomNumber(360);
		m_rotUpdateY=rng->randomPositiveNumber(10);
		m_rotZ=rng->randomNumber(360);
		m_rotUpdateZ=rng->randomPositiveNumber(10);
	}
}


Teapot::~Teapot()
{
  std::cout<<"Teapot dtor\n";
}

