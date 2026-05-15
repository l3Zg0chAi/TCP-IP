#include "TCPConnection.h"
#include "Logger.h"
#include <thread>
#include <chrono>

TCPConnection::TCPConnection(ConnectionInfo info) 
    : _infoConn(info), _sockfd(-1), _stopFlag(true), _state(ESTATE_CONNECTIONS::CLOSED)
{
    DEBUG_LOG ("Constructor TCPConnection objejct");
}

TCPConnection::~TCPConnection()
{
    stop();
}

int TCPConnection::read_pdu()
{
    char buffer[1024];
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(_info.sockfd, &readfds);
    struct timeval timeout;
    timeout.tv_sec = 2;   // 2 second
    timeout.tv_usec = 0;
    int ret = select(_info.sockfd + 1, &readfds, nullptr, nullptr, &timeout);
    if (ret == 0){
        DEBUG_LOG("no data comes");
    }
    else if (ret < 0){
        DEBUG_LOG("select fail errno=%d error=%s", errno, strerror(errno));
    } 
    else{
        ret = recv(_info.sockfd, buffer, sizeof(buffer), 0);
        if ( ret > 0){
            DEBUG_LOG("recv %d bytes", ret);
        }
        else if (ret == 0){
            // close by peer, client close
            DEBUG_LOG("close by peer mean client close socket connection");
            ret = -2; // FIN
        }
        else {
            DEBUG_LOG("recv fail errno=%d error=%s", errno, strerror(errno));
        }
    }
    DEBUG_LOG("ret %d", ret);
    return ret;
}

bool TCPConnection::receive_from_server()
{
    int ret = read_pdu();
    if (ret < 0){
        if (ret == -2){
            DEBUG_LOG("CLOSE BY PEER");
        } else {
            DEBUG_LOG("CLOSE BY RST");
        }
        return false;
    }
    if ( ret > 0){
        DEBUG_LOG("read pdu success with %d bytes", ret);
    }
    return true;
}

bool TCPConnection::open_connection()
{
    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        DEBUG_LOG("bind fail errno=%d error=%s", errno, strerror(errno));
        return false;
    }

    // config ip và port của client sẽ dùng để connect
    struct sockaddr_in clientAddr{};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(_infoConn.clientPort);
    inet_aton(_infoConn.clientADDR.c_str(), &clientAddr.sin_addr);
    
    // config ip port server - nghĩa là kết nối tới ip và port nào của server
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_infoConn.serverPort);
    inet_aton(_infoConn.serverADDR.c_str(), &serverAddr.sin_addr);

    config_optional_socket();

    // bind client addr and port to socket
    if (bind(_sockfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == -1){
        DEBUG_LOG("bind ip %s and port %u client fail errno=%d, error=%s", _infoConn.clientADDR.c_str(), _infoConn.clientPort,errno,strerror(errno));
        return false;
    } else {
        DEBUG_LOG("bind ip %s and port %u client success", _infoConn.clientADDR.c_str(), _infoConn.clientPort);
    }

    if (connect(_sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        DEBUG_LOG("connect to ip %s and port %u server fail errno=%d, error=%s", _infoConn.serverADDR.c_str(), _infoConn.serverPort,errno,strerror(errno));
        return false;
    } else {
        DEBUG_LOG("connect to ip %s and port %u server success", _infoConn.serverADDR.c_str(), _infoConn.serverPort);
    }

    return true;
}

void TCPConnection::close_connection()
{
    if (_sockfd >= 0){
        shutdown(_sockfd, SHUT_RDWR);
        close(_sockfd);
        _sockfd = -1;
    }
}

void TCPConnection::stop()
{
    close_connection();
    _stopFlag.store(true);
    if (_rxThread.joinable()){
        _rxThread.join();
    }
}

void TCPConnection::start()
{
    DEBUG_LOG("start connection client: %s %u -server %s %u", _infoConn.clientADDR.c_str(), 
                                                            _infoConn.clientPort,
                                                            _infoConn.serverADDR.c_str(),
                                                            _infoConn.serverPort);
    if (!_stopFlag.load()){
        DEBUG_LOG("connection already running");
        return;
    }                                                        
    _stopFlag.store(false);
    _rxThread = std::thread(&TCPConnection::rxWorker, this);
}

void TCPConnection::setState(ESTATE_CONNECTIONS state)
{
    if (state != _state){
        _state = state;
    }
}

void TCPConnection::rxWorker()
{
    setCurrentThreadName(std::string("rxWorker"));
    while(true){
        if (_stopFlag.load()) return;
        switch(_state){
            case ESTATE_CONNECTIONS::INIT:
                close_connection();
                if (!open_connection()){
                    continue;
                }
                setState(ESTATE_CONNECTIONS::CONNECTED);
                break;
            case ESTATE_CONNECTIONS::CONNECTED:
                if(!receive_from_server()){
                    setState(ESTATE_CONNECTIONS::CLOSED);
                }
                break;
            case ESTATE_CONNECTIONS::CLOSED:
                close_connection();
                setState(ESTATE_CONNECTIONS::INIT);
                break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void TCPConnection::config_optional_socket()
{
    int opt = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        DEBUG_LOG("setsockopt SO_REUSEADDR fail errno=%d error=%s",errno, strerror(errno));
    }
}
