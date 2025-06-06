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

	/// <summary>
	/// Is called once when a scene stops being selected. Useful for taking care of resources that are no longer needed.
	/// </summary>
	virtual void EndPlay() {};
	/// <summary>
	/// Is called once when a scene is selected. Useful to create resources when a scene starts up.
	/// </summary>
	virtual void BeginPlay() {};
	virtual void Update(float) {};
	virtual void FixedUpdate(float) {};
	virtual void LateUpdate(float) {};
	virtual void DebugDraw() {};

	virtual void ComponentOwnerInitialized() {};

	dae::GameObject* GetOwner() const;

	//maybe not a good idea to use friend
	friend class dae::GameObject;
protected:
	Component();
private:
	void SetOwner(dae::GameObject* owner);
	bool m_DeleteFlag{false};
	dae::GameObject* m_pOwner;



};

