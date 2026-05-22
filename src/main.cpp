#include "Application.h"

int main()
{
    Application app;
    app.Start();

    while (app.IsRunning())
    {
        app.Update();
    }
}
