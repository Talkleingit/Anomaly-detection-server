

#ifndef COMMANDS_H_
#define COMMANDS_H_
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "timeseries.h"

using namespace std;

class DefaultIO
{
public:
    virtual string read() = 0;
    virtual void write(string text) = 0;
    virtual void write(float f) = 0;
    virtual void read(float *f) = 0;
    virtual ~DefaultIO() {}

    // you may add additional methods here
};

// you may add here helper classes

// you may edit this class
class Command
{

protected:
    DefaultIO *dio;
    HybridAnomalyDetector *detector;
    vector<AnomalyReport> reports;
    string train_file = "train.csv";
    string test_file = "test.csv";

public:
    Command(DefaultIO *dio) : dio(dio) {}
    virtual void execute() = 0;
    virtual void printDescription() = 0;
    virtual ~Command() {}
};

// implement here your command classes

class UpdloadCommand : public Command
{
    string description = "1.upload a time series csv file\n";

public:
    UpdloadCommand(DefaultIO *io) : Command(io) {}
    virtual void execute()
    {
        string str;
        Command::dio->write("Please upload your local train csv file.\n");
        ofstream out(Command::train_file);
        ofstream out2(Command::test_file);
        do
        {
            str = Command::dio->read();
            out << str;
        } while (str.compare("done") != 0);
        out.close();
        Command::dio->write("Upload complete.\n");
        Command::dio->write("Please upload your local test csv file.\n");
        do
        {
            str = Command::dio->read();
            out2 << str;
        } while (str.compare("done") != 0);
        out2.close();
        Command::dio->write("Upload complete.\n");
        out2.close();
        Command::dio->write("Upload complete.\n");
    }
    /**
	 * @brief prints the menu option - upload a time series
	 * 
	 */
    virtual void printDescription() override
    {

        Command::dio->write(this->description);
    }
    virtual ~UpdloadCommand() {}
};

class AlgorithmSettingsCommand : public Command
{
    string description = "2.algorithm settings\n";

public:
    AlgorithmSettingsCommand(DefaultIO *io) : Command(io) {}
    virtual void execute()
    {
        float *updated_threshold;
        // input of new threshold (try until given a value between 0 and 1)
        do
        {
            Command::dio->write("The current correlation threshold is ");
            Command::dio->write(Command::detector->getMaxThreshold());
            Command::dio->write("\n");
            Command::dio->read(updated_threshold);
        } while (*updated_threshold < 0 || *updated_threshold > 1);
        Command::detector->setMaxThreshold(*updated_threshold); //update new threshold
    }
    virtual void printDescription() override
    {
        Command::dio->write(this->description);
    }
    virtual ~AlgorithmSettingsCommand() {}
};

class DetectAnomaliesCommand : public Command
{
    string description = "3.detect anomalies\n";

public:
    DetectAnomaliesCommand(DefaultIO *io) : Command(io) {}
    virtual void execute()
    {
        int n = Command::train_file.length();
        int m = Command::test_file.length();
        char train_f[n + 1];
        char test_f[m + 1];
        strcpy(train_f, Command::train_file.c_str());
        strcpy(test_f, Command::test_file.c_str());
        TimeSeries ts_train(train_f); // create a time series
        TimeSeries ts_test(test_f);
        Command::detector->learnNormal(ts_train);              // learn the data from the training file.
        Command::reports = Command::detector->detect(ts_test); //save anomaly reports.
        Command::dio->write("anomaly detection complete.\n");
    }
    virtual void printDescription()
    {
        Command::dio->write(this->description);
    }
    virtual ~DetectAnomaliesCommand() {}
};

class DisplayResultsCommand : public Command
{
    string description = "4.display results";

public:
    DisplayResultsCommand(DefaultIO *io) : Command(io) {}
    void execute() override
    {
        for (AnomalyReport report : Command::reports)
        {
            Command::dio->write(report.timeStep);
            Command::dio->write("  ");
            Command::dio->write(report.description);
            Command::dio->write("\n");
        }
        Command::dio->write("Done");
    }
    void printDescription() override
    {
        Command::dio->write(this->description);
    }
    ~DisplayResultsCommand() {}
};

class AnalyzeCommand : public Command
{
    string description = "5.upload anomalies and analyze results";

public:
    AnalyzeCommand(DefaultIO *io) : Command(io) {}
    void execute() override
    {
    }
    void printDescription() override
    {
        Command::dio->write(this->description);
    }
    ~AnalyzeCommand() {}
}

class MenuCommand : public Command
{
    vector<Command *> commands;

public:
    MenuCommand(DefaultIO *io) : Command(io) {}
    void createMenu()
    {
        Command *uc = new UpdloadCommand(Command::dio);
        this->add(uc);
        Command *asc = new AlgorithmSettingsCommand(Command::dio);
        this->add(asc);
        Command *dac = new DetectAnomaliesCommand(Command::dio);
        this->add(dac);
    }
    void executeGivenCommand(int i)
    {
        this->commands[i - 1]->execute();
    }
    void execute() override
    {
        for (Command *c : this->commands)
        {
            c->execute();
        }
    }
    vector<Command *> get_commands()
    {
        return this->commands;
    }
    void add(Command *c)
    {
        this->commands.push_back(c);
    }
    void printDescription() override
    {
        if (this->commands.size() == 0)
        {
            createMenu();
        }
        Command::dio->write("Welcome to the Anomaly Detection Server.\n");
        Command::dio->write("Please choose an option:\n");
        for (Command *c : this->commands)
        {
            c->printDescription();
        }
    }
    virtual ~MenuCommand()
    {
        for (Command *c : commands)
        {
            delete c;
        }
    }
};

#endif /* COMMANDS_H_ */
