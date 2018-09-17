#include "Emitter.h"
#include "ParticleFactory.h"
#include <ngl/Random.h>
#include <typeinfo>
#include <Sphere.h>

Emitter::Emitter(ngl::Vec3 _pos, unsigned int _numParticles,const Camera *_cam )
{
  m_pos=_pos;
  m_numParticles=_numParticles;
  m_cam=_cam;
  // create our render factory
  std::unique_ptr<ParticleFactory> p(  new  ParticleFactory);
  // grab an instance of the random generator
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 dir;
  ngl::Vec4 c;
  int which;
  // resize the particle container for speed
  m_particles.resize(m_numParticles);
  // loop for each of the particles and create
  for(auto &particle : m_particles)
  {
    // get a random direction and colour
    dir=rng->getRandomVec3();
    c=rng->getRandomColour4();
    // choose which input we want
    which=static_cast<int>(rng->randomPositiveNumber(4));
    switch(which)
    {
    // use the factory to create a new particle
    case 0 : particle.reset(p->CreateParticle(ParticleType::SPHERE,_pos,dir,c,"Phong",this)); break;
    case 1 : particle.reset(p->CreateParticle(ParticleType::TEAPOT,_pos,dir,c,"Phong",this)); break;
    case 2 : particle.reset(p->CreateParticle(ParticleType::TORUS,_pos,dir,c,"Phong",this)); break;
    case 3 : particle.reset(p->CreateParticle(ParticleType::TROLL,_pos,dir,c,"Phong",this)); break;
    }
  }


}


Emitter::Emitter( ngl::Vec3 _pos,unsigned int _numParticles, const Camera *_cam, const ParticleType _type  )
{
  m_cam=_cam;
  m_pos=_pos;
  m_numParticles=_numParticles;
  // create our render factory
  ParticleFactory *p = new  ParticleFactory;

  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 dir;
  ngl::Vec4 c;

  for(auto &particle : m_particles)
  {
    dir=rng->getRandomVec3();
    dir.normalize();
    c=rng->getRandomColour4();
   // std::unique_ptr<Particle> part( );
    particle.reset(p->CreateParticle(_type,_pos,dir,c,"Phong",this));
  }
// we've done with the factory so delete it
  delete p;

}


void Emitter::addParticle(ParticleType _type )
{
  // create our render factory
  ParticleFactory *p = new  ParticleFactory;

  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 dir;
  ngl::Vec4 c;
  dir=rng->getRandomVec3();
  c=rng->getRandomColour4();
  m_particles.push_back(std::unique_ptr<Particle> (p->CreateParticle(_type,m_pos,dir,c,"Phong",this)));
// we've done with the factory so delete it
  delete p;
  ++m_numParticles;
}

void Emitter::removeParticle()
{
  /// when we remove a particle we need to first check to see if we have any
  if(m_numParticles>0)
  {
    // now remove from the end of the std::vector
    m_particles.pop_back();
    // decrement the number of particles
    --m_numParticles;
  }
}
Emitter::~Emitter()
{
  m_particles.clear();
}


void Emitter::draw() const
{
  // call the draw method for each of the particles
  for(auto &p : m_particles)
  {
    p->draw();
  }
}

void Emitter::update()
{
  // call the update method for each particle
  for(auto &p : m_particles)
  {
    p->update();
  }
}
void Emitter::updateEmitAngle(GLfloat _a)
{
  /*
  // in this case only sphere has a set emit angle
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    // so if it is a sphere
    if(m_particles[i]->getType()==SPHERE)
    {
      // now we have to cast to a Sphere to call the setEmitAngle method
      dynamic_cast<Sphere *>(m_particles[i])->setEmitAngle(_a);
    }
  }
*/

  // in this case only sphere has a set emit angle
  for(auto &p : m_particles)
  {
    // so if it is a sphere
    if(Sphere *s = dynamic_cast<Sphere *>( p.get() ))
    {
      // now we have to cast to a Sphere to call the setEmitAngle method
      s->setEmitAngle(_a);
    }
  }
}

void Emitter::clearParticles()
{ 
  // must remember to re-size the std::vector back to 0
  m_particles.clear();
  m_numParticles=0;
}
