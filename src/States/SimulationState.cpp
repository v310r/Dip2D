#include "SimulationState.h"
#include "StateManager.h"
#include "../Objects/Circle.h"
#include <iostream>


void SimulationState::OnCreate()
{
	EventManager* const eManager = m_stateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Simulation, "Key_Escape", &SimulationState::GoToMainMenu, this);
	eManager->AddCallback(StateType::Simulation, "Key_P", &SimulationState::Pause, this);
	eManager->AddCallback(StateType::Simulation, "LeftMouseButtonClick", &SimulationState::SpawnObjectAtMousePosition, this);

	m_physicsWorld = std::make_unique<PhysicsWorld>(m_objects);
}

void SimulationState::OnDestroy()
{
	EventManager* const eManager = m_stateManager->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::Simulation, "Key_Escape");
	eManager->RemoveCallback(StateType::Simulation, "Key_P");
	eManager->RemoveCallback(StateType::Simulation, "LeftMouseButtonClick");

	for (const Object* const obj : m_objects)
	{
		delete obj;
	}
}

void SimulationState::Activate()
{
}

void SimulationState::Deactivate()
{
}

void SimulationState::Update(const sf::Time& deltaTime)
{
	m_physicsWorld->Step(deltaTime.asSeconds());

	//if (m_objects.size() > 0)
	//	std::cout << "Position (x: " << m_objects[0]->GetPosition().x << ", y: " << m_objects[0]->GetPosition().y << ")\n";
}

void SimulationState::Draw()
{
	for (Object* const obj : m_objects)
	{
		m_stateManager->GetContext()->GetWindow()->GetRenderWindow()->draw(obj->GetSprite());
	}
}

void SimulationState::GoToMainMenu(EventDetails* details)
{
	m_stateManager->SwitchTo(StateType::MainMenu);
}

void SimulationState::Pause(EventDetails* details)
{
	m_stateManager->SwitchTo(StateType::Paused);
}

void SimulationState::SpawnObjectAtMousePosition(EventDetails* details)
{
	const glm::vec2 mousePos = details->GetMousePos();

	m_objects.push_back(new Circle(mousePos, 10.0f));
}
