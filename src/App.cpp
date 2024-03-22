#include "App.h"

void App::Run() 
{
    while(m_window->IsRunning())
    {
        std::cout << "running" << std::endl;
    }
}