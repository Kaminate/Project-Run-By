#include "Core.h"
#include "n8time.h"
#include <iostream>
// Core
void Core::Run()
{
  // todo: control framerate
  const float fixed_dt = 1 / 60.0f;
  float lastUpdateTime = GetTimeInSeconds();

  while(1)
  {
    float currTime = GetTimeInSeconds();
    float timeSinceUpdate = currTime - lastUpdateTime;
    if (timeSinceUpdate >= fixed_dt)
    {
      Update(fixed_dt);
      std::cout << "poop" << std::endl;
    }
    else
    {
      //GetSystem<Graphics>().Update(timeElapsedSinceLastUpdate);
    }
  }
}
void Core::Update(float dt)
{
  for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
  {
    (*it)->Update(dt);
  }
}
void Core::AddSystem(System * system) { m_systems.push_back(system); }
// System
System::~System(){}
// Component
Component::Component() : m_owner(0) {}
Component::~Component(){}
// GameObject
GameObject::~GameObject()
{
  for (auto it = m_components.begin(); it != m_components.end(); ++it)
  {
    delete *it;
  }
}
void GameObject::AddComponent(Component * component)
{
  component->m_owner = this;
  m_components.push_back(component);
}

