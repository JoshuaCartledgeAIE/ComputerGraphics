#include <iostream>
#include "GraphicsApplication.h"


int main()
{
    GraphicsApplication* graphicsApp = new GraphicsApplication();

    if (graphicsApp->startup() == true)
    {
        while (graphicsApp->update() == true) 
        {
            graphicsApp->draw();
        }
        graphicsApp->shutdown();
    }

    delete graphicsApp;
    return 0;
}
