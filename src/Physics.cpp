#include "Physics.h"
//#include "Transform.h"
Physics * Physics::Instance()
{
  static Physics * instance;
  return instance ? instance : instance = new Physics();
}

void Physics::Update(float dt)
{
  for (auto it = m_components.begin(); it != m_components.end(); ++it)
  {
    RigidBody * component = *it;
    // todo: integrate, etc...
  }
}

RigidBody::RigidBody()
{
  Physics::Instance()->m_components.insert(this);
}
RigidBody::~RigidBody()
{
  Physics::Instance()->m_components.erase(this);
}
