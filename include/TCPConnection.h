#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "ThreadSafeQueue.h"
#include "CommonDef.h"
#include <atomic>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TCPConnection {
public:
    enum ESTATE_CONNECTIONS{
        INIT = 0,
        CONNECTED,
        CLOSED
    };

    explicit TCPConnection(ConnectionInfo info);
    ~TCPConnection();
    bool receive_from_server();
    bool open_connection();
    void close_connection();
    void stop();
    void start();

    void setState(ESTATE_CONNECTIONS state);

private:
    void rxWorker();
    void config_optional_socket();
private:
    std::thread _rxWorker;
    std::mutex _rxMutex;
    ConnectionInfo _infoConn;
    int _sockfd;
    std::atomic<bool> _stopFlag;
    ESTATE_CONNECTIONS _state;
};

#endif // __TCPCONNECTION_H__