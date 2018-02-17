#ifndef COREH
#define COREH

#include <vector>

class Core;
class System;
class Component;
class GameObject;

class Core
{
public:
  void Run();
  void AddSystem(System * system);
  
  template <class SystemType>
  SystemType * GetSystem();

private:
  void Update(float dt);
  std::vector<System*> m_systems;
};

class System
{
public:
  virtual ~System();
  virtual void Update(float dt) = 0;
};

class Component
{
public:
  Component();
  virtual ~Component();

  template <typename ComponentType>
  ComponentType * GetSibling(); // returns 0 if not found

private:
  friend class GameObject;
  GameObject * m_owner; // set by GameObject::AddComponent   // maybe this should just be public
};

class GameObject
{
public:
  ~GameObject(); // deletes all components
  void AddComponent(Component * component);
  
  template<typename ComponentType>
  ComponentType * GetComponent(); // return 0 if not found

private:
  std::vector<Component *> m_components; // owned by GameObject
};

// template functions:

template <class SystemType>
SystemType * Core::GetSystem()
{
  SystemType * system;
  for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
  {
    system = dynamic_cast<SystemType*>(*it);
    if (system)
    {
      break;
    }
  }
  return system;
}

template<typename ComponentType> 
ComponentType * Component::GetSibling()
{
  return m_owner->GetComponent<ComponentType>();
}

template<typename ComponentType> 
ComponentType * GameObject::GetComponent()
{
  for (auto it = m_components.begin(); it != m_components.end(); ++it)
  {
    ComponentType * component = dynamic_cast<ComponentType*>(*it);
    if (component)
    {
      return component;
    }
  }
  return 0;
}

#endif
