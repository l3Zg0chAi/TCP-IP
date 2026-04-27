#ifndef TCP_COMMUNICATOR
#define TCP_COMMUNICATOR

#include "TCPConnection.h"
#include <unordered_map>
#include <memory>

class TCPCommunicator {
public:
    static TCPCommunicator* get_instance(){
        static TCPCommunicator m_instance;
        return &m_instance;
    }

    TCPCommunicator();
    void start();
    void pushToQueue(Packet value);
    bool receive_packet(Packet& value);
private:
    std::unordered_map<ConnectionID, std::unique_ptr<TCPConnection>> connections;
    ThreadSafeQueue<Packet> _rxQueueAllConn;    
};

#endif // TCP_COMMUNICATOR