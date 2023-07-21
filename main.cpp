#include <iostream>
#include "Window.h"


int main()
{
    std::cout << "Creating Window\n";

    Window* pWindow = new Window();

    bool running = true;
    while(running)
    {
        if (!pWindow->ProcessMessages())
        {
            std::cout << "Closing Window\n";
            running = false;
        }

        // render

        Sleep(10);
    }

    delete pWindow;

    return 0;
}