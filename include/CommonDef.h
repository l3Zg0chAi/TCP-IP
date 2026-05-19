#ifndef COMMON_DEF
#define COMMON_DEF
#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <arpa/inet.h>

typedef std::uint8_t UI_8;
typedef std::uint16_t UI_16;
typedef std::uint32_t UI_32;
typedef UI_8 ConnectionID;

#define ConnectionID_ONE 1U
#define ConnectionID_TWO 2U
#define ConnectionID_THREE 3U

#define CLIENT_IP "192.168.24.128"
#define SERVER_IP "192.168.24.128"
struct ConnectionInfo {
    std::string serverADDR;
    UI_32 serverPort;
    std::string clientADDR;
    UI_32 clientPort;
};    

const std::unordered_map<ConnectionID, ConnectionInfo> ConnectionInfoTable = {
    {ConnectionID_ONE,   {SERVER_IP, 10001, CLIENT_IP, 2001}},
    {ConnectionID_TWO,   {SERVER_IP, 10002, CLIENT_IP, 2002}},
    {ConnectionID_THREE, {SERVER_IP, 10003, CLIENT_IP, 2003}}
};  

struct Packet { 
    Packet() {}; 
    Packet(const std::vector<UI_8>& data) : _rawData(data) {} 

    std::vector<UI_8> _rawData; 
    UI_32 getPDU() const { 
        if (_rawData.size() < 8) { 
            return 0; // Invalid packet 
        } 
        return htonl(*(UI_32*)(&_rawData[0])); 
    } 
    UI_32 getPayloadLen() const{ 
        if(_rawData.size() < 8) { 
            return 0; // Invalid packet 
        } 
        return htonl(*(UI_32*)(&_rawData[4])); 
    } 
    bool operator == (const Packet& other) const { 
        return _rawData == other._rawData; 
    } 
    bool operator != (const Packet& other) const { 
        return _rawData != other._rawData;  
    }
}; 
#endif // COMMON_DEF