#pragma once

#include "../Window/Window.h"
#include <iostream>
#include "../States/StateManager.h"


class Application
{
public:
	Application();

	~Application();

	void HandleInput();

	void Update(const float deltaTime);

	void Render();

	Window& GetWindow();

	void PostUpdate();

	sf::Time RestartClock();

	sf::Time GetElapsedTime();

private:

	Window m_window;
	sf::Clock m_clock;
	SharedContext m_context;
	StateManager m_stateManager;
};
