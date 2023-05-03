#include <iostream>
#include <SFML/Graphics.hpp>
#include "Events/EventManager.h"
#include "ApplicationWorkflow/Application.h"
#include <memory>

#include <glm/glm.hpp>

int main()
{
    auto app = std::make_unique<Application>();

    while (app->GetWindow().IsDone() == false)
    {
        const sf::Time deltaTime = app->RestartClock();
        app->Update(deltaTime.asSeconds());
        std::cout << "ms Update: " << (app->GetElapsedTime().asSeconds() * 1000.0f) << "\n";
        app->Render();
        std::cout << "ms Render: " << (app->GetElapsedTime().asSeconds() * 1000.0f) << "\n";
        app->PostUpdate();

    }
}

