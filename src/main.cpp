#include <iostream>
#include <SFML/Graphics.hpp>
#include "Events/EventManager.h"
#include "ApplicationWorkflow/Application.h"
#include <memory>
#include <glm/glm.hpp>
#include<locale>

struct comma_separator : std::numpunct<char> 
{
	virtual char do_decimal_point() const override { return ','; }
};

int main()
{
    auto app = std::make_unique<Application>();

    std::cout.imbue(std::locale(std::cout.getloc(), new comma_separator));

    while (app->GetWindow().IsDone() == false)
    {
        const sf::Time deltaTime = app->RestartClock();
        app->Update(deltaTime.asSeconds());
        std::cout << (app->GetElapsedTime().asSeconds() * 1000.0f) << "\n";
        app->Render();
        //std::cout << "ms Render: " << (app->GetElapsedTime().asSeconds() * 1000.0f) << "\n";
        app->PostUpdate();

    }
}

