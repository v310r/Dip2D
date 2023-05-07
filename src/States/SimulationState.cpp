#include "SimulationState.h"
#include "StateManager.h"
#include "../Objects/Circle.h"
#include "../Objects/Rectangle.h"
#include "../Physics/ImpulseSolver.h"
#include "../Physics/PositionSolver.h"
#include <iostream>
#include <SFML/System/Vector2.hpp>


//static constexpr size_t NUMBER_OF_CIRCLES = 2000;

static constexpr size_t NUMBER_OF_CIRCLES_FOR_CONTINUOUS_MOUSE_SPAWNING = 103;

void SimulationState::OnCreate()
{
	EventManager* const eManager = m_stateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Simulation, "Key_Escape", &SimulationState::GoToMainMenu, this);
	eManager->AddCallback(StateType::Simulation, "Key_P", &SimulationState::Pause, this);
	//eManager->AddCallback(StateType::Simulation, "LeftMouseButtonClick", &SimulationState::BeginSpawning, this);
	//eManager->AddCallback(StateType::Simulation, "LeftMouseButtonReleased", &SimulationState::FinishSpawning, this);

	eManager->AddCallback(StateType::Simulation, "LeftMouseButtonClick", &SimulationState::SpawnObjectAtMousePosition, this);

	m_physicsWorld = std::make_unique<PhysicsWorld>(m_objects);
	m_physicsWorld->AddSolver(new ImpulseSolver());
	m_physicsWorld->AddSolver(new PositionSolver());

	m_objects.push_back(new Rectangle({ 0, 300 }, {40, 800}));
	m_objects.push_back(new Rectangle({ 400, 600 }, { 800, 40 }));
	m_objects.push_back(new Rectangle({ 800, 300 }, { 40, 800 }));
	m_objects[0]->SetDynamic(false);
	m_objects[1]->SetDynamic(false);
	m_objects[2]->SetDynamic(false);

	//for (size_t i = 0; i < NUMBER_OF_CIRCLES; ++i)
	//{
	//	m_objects.push_back(new Circle({ 0.0f, 0.0f }, 10.0f));
	//}

	
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
	m_physicsWorld->Step(deltaTime);

	if (bSpawning)
	{
		sf::RenderWindow* const window = m_stateManager->GetContext()->GetWindow()->GetRenderWindow();
		if (window)
		{
			sf::Vector2i mousePos = m_mouse.getPosition(*window);
			if (m_objects.size() <= NUMBER_OF_CIRCLES_FOR_CONTINUOUS_MOUSE_SPAWNING)
			{
				m_objects.push_back(new Circle({ mousePos.x, mousePos.y }));
			}
		}
	}

	//m_objects.push_back(new Circle(glm::vec2(300.0f, 10.0f), 10.0f));

	//DestroyIrrelevantObjects();
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

void SimulationState::DestroyIrrelevantObjects()
{
	//std::cout << "Objects num: " << m_objects.size() << "\n";
	for (auto it = m_objects.begin(); it != m_objects.end();)
	{
		if (Rectangle* rec = dynamic_cast<Rectangle*>(*it))
		{
			it++;
			continue;
		}

		// object has fallen too deep
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

void SimulationState::SpawnObjectAtMousePosition(EventDetails* details)
{
	glm::vec2 mousePos = details->GetMousePos();

	m_objects.push_back(new Circle(mousePos, 10.0f));
}

void SimulationState::BeginSpawning(EventDetails* details)
{
	bSpawning = true;
}

void SimulationState::FinishSpawning(EventDetails* details)
{
	bSpawning = false;
}
