#include <iostream>
#include "GraphicsApplication.h"


int main()
{
    GraphicsApplication* graphicsApp = new GraphicsApplication();

    if (graphicsApp->Startup() == true)
    {
        while (graphicsApp->Update() == true) 
        {
            graphicsApp->Draw();
        }
        graphicsApp->Shutdown();
    }

    delete graphicsApp;
    return 0;
}
