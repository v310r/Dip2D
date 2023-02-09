#pragma once
#include "BaseState.h"
#include "../Objects/Object.h"
#include "../Physics/PhysicsWorld.h"


class SimulationState : public BaseState
{
public:

	SimulationState(StateManager* stateManager) : BaseState(stateManager) {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void Update(const float deltaTime) override;
	virtual void Draw() override;

	void GoToMainMenu(EventDetails* details);
	void Pause(EventDetails* details);

	void SpawnObjectAtMousePosition(EventDetails* details);

private:

	std::unique_ptr<PhysicsWorld> m_physicsWorld;

	std::vector<Object*> m_objects;

};

