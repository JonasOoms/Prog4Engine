#pragma once
#include "memory"

namespace dae
{
	class GameObject;
};

class Component
{
public:
	virtual ~Component() = default;
	Component(const Component& copy) = delete;
	Component(Component&& move) = delete;
	Component& operator=(const Component& rhs) = delete;
	Component& operator=(Component&& rhs) = delete;

	void DeleteComponent();
	bool IsFlaggedForDelete() const { return m_DeleteFlag; }

	virtual void Update(float) {};
	virtual void FixedUpdate(float) {};
	virtual void LateUpdate(float) {};

	virtual void ComponentOwnerInitialized() {};

	// TODO: Dangerous! Make private then friend class the gameobject
	dae::GameObject* GetOwner() const;

	//maybe not a good idea to use friend
	friend class dae::GameObject;
protected:
	// TODO: Add friend clas to gameobject
	Component();
private:
	void SetOwner(dae::GameObject* owner);
	bool m_DeleteFlag{false};
	dae::GameObject* m_pOwner;



};

