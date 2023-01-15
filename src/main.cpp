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
        app->Update();
        app->Render();
        app->PostUpdate();
    }
}

