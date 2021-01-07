#include "CLI.h"
#include "string.h"
#include "sstream"
#include "HybridAnomalyDetector.h"
#include "timeseries.h"

CLI::CLI(DefaultIO *dio)
{
    CLI::dio = dio;
}

void CLI::start()
{

    vector<AnomalyReport> *reports_p = new vector<AnomalyReport>();
    HybridAnomalyDetector *detector = new HybridAnomalyDetector();
    NeededAttributes needed;
    needed.detector = detector;
    needed.reports = reports_p;
    InfoForCommands *info = new InfoForCommands(&needed);
    MenuCommand *menu = new MenuCommand(CLI::dio, info);
    menu->createMenu();
    menu->printDescription();
    bool found = false;
    string s;
    while (true)
    {
        s = CLI::dio->read();
        stringstream ss(s);
        int option;
        ss >> option;
        switch (option)
        {
        case 1:
            menu->executeGivenCommand(1);
            break;
        case 2:
            menu->executeGivenCommand(2);
            break;
        case 3:
            menu->executeGivenCommand(3);
            break;
        case 4:
            menu->executeGivenCommand(4);
            break;
        case 5:
            menu->executeGivenCommand(5);
            break;
        case 6:
            found = true;
            break;
        default:
            break;
        }
        if (found)
        {
            break;
        }

        menu->printDescription();
    }
    delete reports_p;
    delete detector;
    delete info;
    delete menu;
}

CLI::~CLI()
{
}
