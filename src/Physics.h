#ifndef PHYSICS_H
#define PHYSICS_H

#include <set>
#include <map>
#include <string>

#include "Core.h"
//#include "Point4.h"
//#include "Vector4.h"

class Physics;
class RigidBody;

class Physics : public System
{
public:
  static Physics * Instance();
  virtual void Update(float dt);
  
private:
  friend class RigidBody;
  
  std::set<RigidBody*> m_components;
  
};

class RigidBody : public Component
{
public:
  RigidBody();
  ~RigidBody();
private:
  //Point4 m_position;  // m/s.
  //Vector4 m_velocity; // m/s.
  //float m_radius;     // m.   assumption: everything's a circle
};
#endif
