#ifndef COMMANDS_H_
#define COMMANDS_H_
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "timeseries.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#define READING_BUFFER_SIZE 1
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

/**
 * @brief this is the socket io class that implements the DefaultIO interface
 * 
 */
class SocketIo : public DefaultIO
{
    int m_clientid;
    char m_buffer[READING_BUFFER_SIZE];
    char *buf;
    const char *m_datamsg;
    int m_bytesread;
    int m_bytessent = 0;
    int m_totalsent = 0;

public:
    SocketIo(int clientid)
    {
        this->m_clientid = clientid;
    }
    virtual string read() throw(const char *) override
    {
        string str;
        int n = ::read(m_clientid, m_buffer, sizeof(m_buffer));
        while (m_buffer[0] != '\n')
        {
            str += m_buffer[0];
            ::read(m_clientid, m_buffer, sizeof(m_buffer));
        }
        return str;
    }
    virtual void read(float *f)
    {
        char buf[4];
        if ((this->m_bytesread = ::read(this->m_clientid, buf, 4)) <= 0)
        {
            throw std::system_error(errno, std::generic_category(), "Error reading");
        }

        std::string tmpstr(buf);
        *f = std::stof(tmpstr);
    }
    virtual void write(string text) throw(const char *)
    {

        int text_length = text.size();
        const char *msg = text.c_str();
        if ((this->m_bytessent = ::send(this->m_clientid, msg, text_length, 0)) <= 0)
        {
            throw std::system_error(errno, std::generic_category(), "Error sending");
        }
    }
    virtual void write(float f) throw(const char *)
    {
        std::stringstream sstream;
        sstream << f;
        string str = sstream.str();

        int n = str.size();
        this->m_datamsg = str.c_str(); // conver to char *
        if ((this->m_bytessent = ::write(this->m_clientid, this->m_datamsg, n)) < 0)
        {
            throw std::system_error(errno, std::generic_category(), "Error sending");
        }
    }

    ~SocketIo() {}
};

// you may add here helper classes

// you may edit this class

struct NeededAttributes
{
    HybridAnomalyDetector *detector;
    vector<AnomalyReport> *reports;
    string train_file = "train.csv";
    string test_file = "test.csv";
    int ts_length;
};

struct TimeFrame
{
    long start;
    long end;
};

class InfoForCommands
{

public:
    NeededAttributes *attributes;
    InfoForCommands(NeededAttributes *attr)
    {
        this->attributes = attr;
    }
    ~InfoForCommands() {}
};
class Command
{

protected:
    DefaultIO *dio;
    InfoForCommands *info;

public:
    Command(DefaultIO *dio, InfoForCommands *info) : dio(dio), info(info)
    {
    }
    virtual void execute() = 0;
    virtual void printDescription() = 0;
    virtual ~Command()
    {
    }
};

class UpdloadCommand : public Command
{
    string description = "1.upload a time series csv file";

public:
    UpdloadCommand(DefaultIO *io, InfoForCommands *info) : Command(io, info) {}
    virtual void execute()
    {
        string str;
        Command::dio->write("Please upload your local train CSV file.\n");
        ofstream out(Command::info->attributes->train_file);
        ofstream out2(Command::info->attributes->test_file);
        str = Command::dio->read();
        while (str != "done")
        {
            out << str;
            out << "\n";
            str = Command::dio->read();
        }
        out.close();
        Command::dio->write("Upload complete.\n");
        Command::dio->write("Please upload your local test CSV file.\n");
        str = Command::dio->read();
        while (str != "done")
        {
            out2 << str;
            out2 << "\n";
            str = Command::dio->read();
        }
        /*
        do
        {
            str = Command::dio->read();
            if (str.compare("done") != 0)
            {
                out2 << str;
                out2 << "\n";
            }

        } while (str.compare("done") != 0);
        */
        out2.close();
        Command::dio->write("Upload complete.");
        Command::dio->write("\n");
    }
    /**
	 * @brief prints the menu option - upload a time series
	 * 
	 */
    virtual void printDescription() override
    {

        Command::dio->write(this->description);
        Command::dio->write("\n");
    }
    virtual ~UpdloadCommand() {}
};

class AlgorithmSettingsCommand : public Command
{
    string description = "2.algorithm settings";

public:
    AlgorithmSettingsCommand(DefaultIO *io, InfoForCommands *info) : Command(io, info) {}
    virtual void execute()
    {
        float *updated_threshold = new float;
        bool not_in_range = false;
        // input of new threshold (try until given a value between 0 and 1)
        // need to changed implementation
        do
        {
            Command::dio->write("The current correlation threshold is ");
            Command::dio->write(Command::info->attributes->detector->getMaxThreshold());
            Command::dio->write("\n");
            Command::dio->write("Type a new threshold");
            Command::dio->write("\n");
            Command::dio->read(updated_threshold);
            not_in_range = *updated_threshold < 0 || *updated_threshold > 1;
            if (not_in_range)
            {
                Command::dio->write("please choose a value between 0 and 1.");
                Command::dio->write("\n");
            }
        } while (not_in_range);
        Command::info->attributes->detector->setMaxThreshold(*updated_threshold); //update new threshold
        delete updated_threshold;
    }
    virtual void printDescription() override
    {
        Command::dio->write(this->description);
        Command::dio->write("\n");
    }
    virtual ~AlgorithmSettingsCommand() {}
};

class DetectAnomaliesCommand : public Command
{
    string description = "3.detect anomalies";

public:
    DetectAnomaliesCommand(DefaultIO *io, InfoForCommands *info) : Command(io, info) {}
    virtual void execute()
    {
        int n = Command::info->attributes->train_file.length();
        int m = Command::info->attributes->test_file.length();
        char train_f[n + 1];
        char test_f[m + 1];
        strcpy(train_f, Command::info->attributes->train_file.c_str());
        strcpy(test_f, Command::info->attributes->test_file.c_str());
        TimeSeries ts_train(train_f); // create a time series
        TimeSeries ts_test(test_f);
        Command::info->attributes->ts_length = ts_test.get_num_of_rows();
        Command::info->attributes->detector->learnNormal(ts_train);                                   // learn the data from the training file.
        *(Command::info->attributes->reports) = Command::info->attributes->detector->detect(ts_test); //save anomaly reports.
        Command::dio->write("anomaly detection complete.\n");
    }
    virtual void printDescription()
    {
        Command::dio->write(this->description);
        Command::dio->write("\n");
    }
    virtual ~DetectAnomaliesCommand() {}
};

class DisplayResultsCommand : public Command
{
    string description = "4.display results";

public:
    DisplayResultsCommand(DefaultIO *io, InfoForCommands *info) : Command(io, info) {}
    void execute() override
    {
        vector<AnomalyReport> reports = *Command::info->attributes->reports;
        for (AnomalyReport report : reports)
        {
            Command::dio->write(report.timeStep);
            Command::dio->write("\t");
            Command::dio->write(report.description);
            Command::dio->write("\n");
        }
        Command::dio->write("Done.\n");
    }
    void printDescription() override
    {
        Command::dio->write(this->description);
        Command::dio->write("\n");
    }
    ~DisplayResultsCommand() {}
};

class AnalyzeCommand : public Command
{
    string description = "5.upload anomalies and analyze results";

public:
    AnalyzeCommand(DefaultIO *io, InfoForCommands *info) : Command(io, info) {}
    void execute() override
    {
        string str, str_time, str_end_time;
        vector<AnomalyReport> reports = *(Command::info->attributes->reports);
        int size = reports.size();
        double FP = 0, TP = 0, TN = 0, FN = 0;
        long start_time, end_time;
        int P = 0;
        int n = Command::info->attributes->ts_length;
        int N = n; // start the negative parameter with the number of lines in the csv file
        bool found = false, found_tp = false;
        size_t current = reports[0].timeStep;
        vector<TimeFrame> timeframes;
        vector<TimeFrame> clientTimeFrames;
        Command::dio->write("Please upload your local anomalies file.\n");
        Command::dio->write("Upload complete.\n");
        str = Command::dio->read();
        //create time frames according to the times found by the anomaly detector
        for (int i = 1; i < size; i++)
        {
            if ((i == size - 1) && (reports[i].description.compare(reports[i - 1].description) == 0 && reports[i].timeStep - 1 == reports[i - 1].timeStep))
            {
                TimeFrame timeframe;
                timeframe.start = current;
                timeframe.end = reports[i].timeStep;
                timeframes.push_back(timeframe);
                current = reports[i].timeStep;
            }
            if (reports[i].description.compare(reports[i - 1].description) != 0 || reports[i].timeStep - 1 != reports[i - 1].timeStep)
            {
                TimeFrame timeframe;
                timeframe.start = current;
                timeframe.end = reports[i - 1].timeStep;
                timeframes.push_back(timeframe);
                current = reports[i].timeStep;
            }
        }
        while (str != "done")
        {
            size_t end_pos = str.find(",");
            str_time = str.substr(0, end_pos);
            stringstream ss(str_time);
            ss >> start_time;
            str_end_time = str.substr(end_pos + 1);
            stringstream ss2(str_end_time);
            ss2 >> end_time;
            current = 1;
            TimeFrame tf;
            tf.start = start_time;
            tf.end = end_time;
            clientTimeFrames.push_back(tf);
            //update number of
            P++;
            N -= (end_time - start_time + 1); //update the number of negative lines
            str = Command::dio->read();       // read for the next iteration
        }
        bool f = false;
        for (TimeFrame st : timeframes)
        {
            f = false;
            for (TimeFrame ct : clientTimeFrames)
            {
                if ((st.start >= ct.start) && (st.end <= ct.end))
                {
                    TP = TP + 1;
                    f = true;
                }
                else if ((st.start <= ct.start) && ((st.end < ct.end) && (st.end >= ct.start)))
                {
                    TP = TP + 1;
                    f = true;
                }
                else if (((st.start >= ct.start) && (st.start <= ct.end)) && (st.end >= ct.end))
                {
                    TP = TP + 1;
                    f = true;
                }
                else if ((st.start < ct.start) && (st.end > ct.end))
                {
                    TP = TP + 1;
                    f = true;
                }
            }
            if (f == false)
            {
                FP = FP + 1;
            }
        }
        // calculate rates:
        float true_positive_rate = TP / P;
        true_positive_rate = floor(true_positive_rate * 1000) / 1000.0;
        float false_positive_rate = FP / N;
        false_positive_rate = floor(false_positive_rate * 1000) / 1000.0;
        Command::dio->write("True Positive Rate: ");
        Command::dio->write(true_positive_rate);
        Command::dio->write("\n");
        Command::dio->write("False Positive Rate: ");
        Command::dio->write(false_positive_rate);
        Command::dio->write("\n");
    }

    void printDescription() override
    {
        Command::dio->write(this->description);
        Command::dio->write("\n");
    }
    ~AnalyzeCommand() {}
};

class ExitCommand : public Command
{
    string description = "6.exit";

public:
    ExitCommand(DefaultIO *io, InfoForCommands *info) : Command(io, info) {}
    void execute()
    {
        return;
    }
    void printDescription()
    {
        Command::dio->write(this->description);
        Command::dio->write("\n");
    }
    ~ExitCommand() {}
};

class MenuCommand : public Command
{
    vector<Command *> commands;

public:
    MenuCommand(DefaultIO *io, InfoForCommands *info) : Command(io, info) {}
    void createMenu()
    {
        //command 1
        Command *c1 = new UpdloadCommand(Command::dio, Command::info);
        this->add(c1);
        //command 2
        Command *c2 = new AlgorithmSettingsCommand(Command::dio, Command::info);
        this->add(c2);
        //command 3
        Command *c3 = new DetectAnomaliesCommand(Command::dio, Command::info);
        this->add(c3);
        //command 4
        Command *c4 = new DisplayResultsCommand(Command::dio, Command::info);
        this->add(c4);
        //command 5
        Command *c5 = new AnalyzeCommand(Command::dio, Command::info);
        this->add(c5);
        //command 6
        Command *c6 = new ExitCommand(Command::dio, Command::info);
        this->add(c6);
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
        Command::dio->write("Welcome to the Anomaly Detection Server.");
        Command::dio->write("\n");
        Command::dio->write("Please choose an option:");
        Command::dio->write("\n");
        for (Command *c : this->commands)
        {
            c->printDescription();
        }
        int x;
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