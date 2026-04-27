#ifndef __COMMON_DEF__
#define __COMMON_DEF__
#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>

typedef std::uint8_t UI_8;
typedef std::uint16_t UI_16;
typedef std::uint32_t UI_32;
typedef UI_8 ConnectionID;

#define ConnectionID_ONE 1U
#define ConnectionID_TWO 2U
#define ConnectionID_THREE 3U

#define CLIENT_IP "192.168.24.128"
#define SERVER_IP ""
struct ConnectionInfo {
    std::string serverADDR;
    UI_32 serverPort;
    std::string clientADDR;
    UI_32 clientPort;
};

struct Packet {

};

const std::unordered_map<ConnectionID, ConnectionInfo> ConnectionInfoTable = {
    {ConnectionID_ONE,   {SERVER_IP, 10001, CLIENT_IP, 9001}},
    {ConnectionID_TWO,   {SERVER_IP, 10002, CLIENT_IP, 9002}},
    {ConnectionID_THREE, {SERVER_IP, 10003, CLIENT_IP, 9003}}
}; 

#endif // __COMMON_DEF__