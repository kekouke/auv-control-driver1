#include "application.h"

int main(int argc, char *argv[])
{
    Application app;

    ipc::Core::Description appDescription;
    appDescription._title       = "Джойстик";
    appDescription._version     = "1.1";
    appDescription._description = "Модуль управления джойстиком";

    app.Initialize(argc, argv, appDescription);
    app.Run();

    return 0;
}
