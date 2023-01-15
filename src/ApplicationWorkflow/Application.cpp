#include "Application.h"


Application::Application() : m_window("Rigid body simulation", sf::Vector2u(800, 600)), m_stateManager(&m_context)
{
	m_context.SetWindow(&m_window);
	m_context.SetEventManager(m_window.GetEventManager());
	m_stateManager.SwitchTo(StateType::Simulation);
}

Application::~Application()
{
}

void Application::HandleInput()
{

}

void Application::Update()
{
	m_window.Update();
	const sf::Time deltaTime = m_clock.getElapsedTime();
	m_stateManager.Update(deltaTime);
}

void Application::Render()
{
	m_window.BeginDraw();
	m_stateManager.Draw();
	m_window.EndDraw();
}

Window& Application::GetWindow()
{
	return m_window;
}

void Application::PostUpdate()
{
	m_stateManager.ProcessRequests();
	m_clock.restart();
}
