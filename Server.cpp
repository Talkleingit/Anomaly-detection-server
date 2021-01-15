
#include "Server.h"

Server::Server(int port) throw(const char *)
{

    Server::m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (Server::m_fd < 0)
    {
        throw "socket error";
    }
    Server::m_serverinfo.sin_family = AF_INET;
    Server::m_serverinfo.sin_addr.s_addr = INADDR_ANY;
    Server::m_serverinfo.sin_port = htons(port);
    int bind_success = bind(Server::m_fd, (struct sockaddr *)&(Server::m_serverinfo), sizeof(Server::m_serverinfo));
    if (bind_success < 0)
    {
        throw "bind error";
    }
    int listen_success = listen(Server::m_fd, 5);
    if (listen_success < 0)
    {
        throw "listen error";
    }
}

void Server::start(ClientHandler &ch) throw(const char *)
{
    Server::m_thread = new thread([this, &ch]() {
        while (!m_stop)
        {

            alarm(2);
            socklen_t client_length = sizeof(m_clientinfo);
            m_clientID = accept(m_fd, (struct sockaddr *)&m_clientinfo, &client_length);

            if (m_clientID < 0)
            {
                throw std::system_error(errno, std::generic_category(), "Accept error");
            }
            ch.handle(m_clientID);
            close(m_clientID);
            alarm(0);
            /*
            thread *t = new thread([this, &ch]() {

            });
            */
            //   alarm(0);m
        }
        close(Server::m_fd);
    });
}

void Server::stop()
{

    Server::m_thread->join(); // do not delete this!
    Server::m_stop = true;
}

Server::~Server()
{
}
