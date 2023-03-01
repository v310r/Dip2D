#include "SimulationState.h"
#include "StateManager.h"
#include "../Objects/Circle.h"
#include "../Objects/Rectangle.h"
#include "../Physics/ImpulseSolver.h"
#include "../Physics/PositionSolver.h"
#include <iostream>


void SimulationState::OnCreate()
{
	EventManager* const eManager = m_stateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Simulation, "Key_Escape", &SimulationState::GoToMainMenu, this);
	eManager->AddCallback(StateType::Simulation, "Key_P", &SimulationState::Pause, this);
	eManager->AddCallback(StateType::Simulation, "LeftMouseButtonClick", &SimulationState::SpawnObjectAtMousePosition, this);

	m_physicsWorld = std::make_unique<PhysicsWorld>(m_objects);
	m_physicsWorld->AddSolver(new ImpulseSolver());
	m_physicsWorld->AddSolver(new PositionSolver());

	m_objects.push_back(new Rectangle({300, 400}));
	m_objects[0]->SetDynamic(false);

	
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

void SimulationState::Update(const float deltaTime)
{
	//m_physicsWorld->Step(deltaTime * 5.0f);
	//m_physicsWorld->ResolveCollisions(deltaTime * 5.0f);

	m_physicsWorld->Step(deltaTime);
	m_physicsWorld->ResolveCollisions(deltaTime);

	std::cout << "Objects num: " << m_objects.size() << "\n";
	for (auto it = m_objects.begin(); it != m_objects.end();)
	{
		if ((*it)->GetPosition().y >= 2000.0f)
		{
			Object* temp = *it;
			it = m_objects.erase(it);
			delete temp;
		}
		else
		{
			it++;
		}
	}
}

void SimulationState::Draw()
{
	for (Object* const obj : m_objects)
	{
		obj->Draw(*m_stateManager->GetContext()->GetWindow()->GetRenderWindow());
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
	glm::vec2 mousePos = details->GetMousePos();

	m_objects.push_back(new Circle(mousePos, 10.0f));
}
