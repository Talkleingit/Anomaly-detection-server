
#ifndef SERVER_H_
#define SERVER_H_

using namespace std;
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <thread>
#include "commands.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "CLI.h"

// edit your ClientHandler interface here:
class ClientHandler
{

public:
	virtual void handle(int clientID) const = 0;
};

//dds
// you can add helper classes here and implement on the cpp file

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler
{

public:
	virtual void handle(int clientID) const
	{
		SocketIo sio(clientID);
		CLI cli(&sio);
		cli.start();
	}
};

// implement on Server.cpp
class Server
{
	thread *m_thread; // the thread to run the start() method in
	sockaddr_in m_serverinfo;
	sockaddr_in m_clientinfo;
	mutex m_lock;
	int m_fd;
	int m_clientID;
	volatile bool m_stop = false;

	// you may add data members

public:
	Server(int port) throw(const char *);
	virtual ~Server();
	void start(ClientHandler &ch) throw(const char *);
	void stop();
};

#endif /* SERVER_H_ */
