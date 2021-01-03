#include "CLI.h"
#include "string.h"
#include "sstream"
CLI::CLI(DefaultIO *dio)
{
    CLI::dio = dio;
}

void CLI::start()
{
    MenuCommand *menu = new MenuCommand(CLI::dio);
    menu->printDescription();
    stringstream ss(CLI::dio->read());
    int option;
    ss >> option;
    switch (option)
    {
    case 1:
        menu->executeGivenCommand(1);
        break;

    default:
        break;
    }
}

CLI::~CLI()
{
}
