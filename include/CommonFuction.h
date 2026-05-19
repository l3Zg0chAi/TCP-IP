#ifndef COMMON_FUNCTION
#define COMMON_FUNCTION

#include <vector>
#include <string>
#include <cstdint>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void serializeHeader(std::vector<uint8_t>& vtBuff, const uint32_t& PDU, const uint32_t& PAYLOAD_LEN) { 
   vtBuff.resize(8 + PAYLOAD_LEN, 0); 
   uint32_t be = htonl(PDU); 
   uint8_t* buffer = &vtBuff[0]; 
   memcpy(buffer, &be, 4); 
   be = htonl(PAYLOAD_LEN); 
   memcpy(&buffer[4], &be, 4); 
}

void set_bits(uint8_t* buffer, int& bit_offset, uint32_t value, int bit_count) { 
   for (int i = bit_count - 1; i >= 0; --i) { 
       int byte_index = bit_offset / 8; 
       int bit_index = 7 - (bit_offset % 8); 

       // Extract the current bit from value and set it in the buffer 
       buffer[byte_index] |= ((value >> i) & 0x1) << bit_index; 

       // Move to the next bit position 
       bit_offset++; 
   } 
} 

void set_bits(uint8_t* buffer, int& bit_offset, const char* str, int str_len) { 
   for (int i = 0; i < str_len; ++i) { 
       char current_char = str[i]; 
       // For each character (1 byte), we set 8 bits 
       for (int bit = 7; bit >= 0; --bit) { 
           int byte_index = bit_offset / 8;  // Determine which byte to modify 
           int bit_index = 7 - (bit_offset % 8);  // Determine bit position in the byte 

           // Set or clear the bit in the buffer depending on the current bit in the char 
           buffer[byte_index] |= ((current_char >> bit) & 0x1) << bit_index; 

           ++bit_offset;  // Move to the next bit position 
       } 
   } 
} 

uint32_t get_bits(const uint8_t* buffer, int& bit_offset, int num_bits) { 
   uint32_t value = 0; 
   for (int i = 0; i < num_bits; ++i) { 
       int byte_index = bit_offset / 8; 
       int bit_index = 7 - (bit_offset % 8); 

       if (buffer[byte_index] & (1 << bit_index)) { 
           value |= (1 << (num_bits - 1 - i)); 
       } 
       ++bit_offset; 
   } 
   return value; 
} 

void get_bits(const uint8_t* buffer, int& bit_offset, char* str, int str_len) { 
   memset(str, 0, str_len); 
   for (int i = 0; i < str_len; ++i) {  // Read each character (8 bits at a time) 
       char ch = 0; 
       for (int bit = 7; bit >= 0; --bit) { 
           int byte_index = bit_offset / 8; 
           int bit_index = 7 - (bit_offset % 8); 

           if (buffer[byte_index] & (1 << bit_index)) { 
               ch |= (1 << bit); 
           } 
           ++bit_offset; 
       } 
       str[i] = ch; 
   } 
}

ssize_t EthernetTimeSyncRecv(int _sockfd, uint8_t* buffer, int length, int second, int microsecond){
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(_sockfd, &readfds);
    struct timeval timeout;
    timeout.tv_sec = second;   // 2 second
    timeout.tv_usec = microsecond;
    int ret = select(_sockfd + 1, &readfds, nullptr, nullptr, &timeout);
    if (ret == 0){
        DEBUG_LOG("no data comes");
    }
    else if (ret < 0){
        DEBUG_LOG("select fail errno=%d error=%s", errno, strerror(errno));
    } 
    else{
        ret = recv(_sockfd, buffer, length, 0);
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
#endif // COMMON_FUNCTION