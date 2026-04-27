#include "TCPCommunicator.h"
#include "Logger.h"

TCPCommunicator::TCPCommunicator()
{
    setCurrentThreadName("TCPCommunicator");
    DEBUG_LOG("create all tcp connections object");
    for (const auto& connection : ConnectionInfoTable){
        connections[connection.first] = std::make_unique<TCPConnection>(connection.second);
    }
}

void TCPCommunicator::start()
{
    for (const auto& connection : connections){
        connection.second->start();
    }
}

void TCPCommunicator::pushToQueue(Packet value)
{
    _rxQueueAllConn.push(value);
}

bool TCPCommunicator::receive_packet(Packet &value)
{
    if (_rxQueueAllConn.try_pop(value)){
        return true;
    }
    return false;
}
