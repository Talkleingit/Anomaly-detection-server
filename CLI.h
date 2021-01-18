

#ifndef CLI_H_
#define CLI_H_
#include <thread>
#include <string.h>
#include "commands.h"
#include <mutex>
using namespace std;

class CLI
{
	DefaultIO *dio;
	MenuCommand *menu;
	InfoForCommands *info;
	mutex lock;

public:
	CLI(DefaultIO *dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
