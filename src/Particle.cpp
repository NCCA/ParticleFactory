#include "Particle.h"
#include <ngl/Random.h>
#include <iostream>
Particle::Particle(
                      ngl::Vec3 _pos,
                      ngl::Vec3 _dir,
                      ngl::Vec4 _c,
											const std::string &_shaderName,
                      Emitter *_parent
                     )
{
	// set the particles position on direction
	m_pos=_pos;
	m_dir=_dir;
	m_colour=_c;
  // create some random life values

  m_maxLife=ngl::Random::randomNumber(200)+40;
	m_life=0.0;
	m_shaderName=_shaderName;
  m_parent=_parent;
}

Particle::~Particle()
{
  std::cout<<"particle  dtor\n";
}

