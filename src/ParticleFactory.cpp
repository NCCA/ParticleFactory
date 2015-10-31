#include "ParticleFactory.h"
#include "Sphere.h"
#include "Teapot.h"
#include "Torus.h"
#include "Troll.h"

Particle * ParticleFactory::CreateParticle(
                                            ParticleType _type,
                                            ngl::Vec3 _pos,
                                            ngl::Vec3 _dir,
                                            ngl::Colour _c,
                                            const std::string &_shaderName,
                                            Emitter *_parent
                                          )
{

  if(_type==ParticleType::TEAPOT)
  {
    return new Teapot(_pos,_dir,_c,_shaderName,_parent);
  }
  else if(_type==ParticleType::SPHERE)
  {
    return new Sphere(_pos,_dir,_c,_shaderName,_parent);
  }
  else if(_type==ParticleType::TORUS)
  {
    return new Torus(_pos,_dir,_c,_shaderName,_parent);
  }
  else if(_type==ParticleType::TROLL)
  {
    return new Troll(_pos,_dir,_c,_shaderName,_parent);
  }

}
