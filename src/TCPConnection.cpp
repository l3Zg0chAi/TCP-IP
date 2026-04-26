#include "TCPConnection.h"
#include "Logger.h"

TCPConnection::TCPConnection(ConnectionInfo info) 
    : _infoConn(info), _sockfd(-1), _stopFlag(true), _state(ESTATE_CONNECTIONS::CLOSED){
    DEBUG_LOG ("Constructor TCPConnection objejct");
}

TCPConnection::~TCPConnection()
{
    stop();
}

bool TCPConnection::receive_from_server()
{
    // nhận từ recv, push vào queue
    return false;
}

bool TCPConnection::open_connection()
{
    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        DEBUG_LOG ("open socket fail");
        return false;
    }

    // config ip và port của client sẽ dùng để connect
    struct sockaddr_in clientAddr{};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(_infoConn.clientPort);
    inet_aton(_infoConn.clientADDR.c_str(), &clientAddr.sin_addr);
    
    // config ip port server - nghĩa là kết nối tới ip và port nào của server
    struct sockaddr_in serverAddr{};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(_infoConn.serverPort);
    inet_aton(_infoConn.serverADDR.c_str(), &serverADDR.sin_addr);

    config_optional_socket();

    // bind client addr and port to socket
    if (bind(_sockfd, (struct sockaddr*)clientAddr, sizeof(clientAddr)) == -1){
        DEBUG_LOG("bind ip %s and port %u client fail", _infoConn.clientADDR.c_str(), _infoConn.clientPort);
        return false;
    } else {
        DEBUG_LOG("bind ip %s and port %u client success", _infoConn.clientADDR.c_str(), _infoConn.clientPort);
    }

    if (connect(_sockfd, (struct sockaddr)serverAddr, sizeof(serverAddr)) == -1){
        DEBUG_LOG("connect to ip %s and port %u server fail", _infoConn.serverADDR.c_str(), _infoConn.serverPort);
        return false;
    } else {
        DEBUG_LOG("connect to ip %s and port %u server success", _infoConn.serverADDR.c_str(), _infoConn.serverPort);
    }

    return true;
}

void TCPConnection::close_connection()
{
    close(_sockfd);
    _sockfd = -1;
}

void TCPConnection::stop()
{
    close_connection();
    _stopFlag = true;
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
    if (!_stopFlag){
        DEBUG_LOG("connection already running");
        return;
    }                                                        
    _stopFlag = false;
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
        if (_stopFlag) return;
        switch(_state){
            case ESTATE_CONNECTIONS::INIT:
                if(!open_connection()){
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
    }
}

void TCPConnection::config_optional_socket()
{
}
