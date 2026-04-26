#ifndef __COMMON_DEF__
#define __COMMON_DEF__
#include <iostream>
#include <cstdint>
#include <string>

typedef std::uint8_t UI_8;
typedef std::uint16_t UI_16;
typedef std::uint32_t UI_32;
typedef UI_8 ConnectionID;
struct ConnectionInfo {
    std::string serverADDR;
    UI_8 serverPort;
    std::string clientADDR;
    UI_8 clientPort;
};

#endif // __COMMON_DEF__