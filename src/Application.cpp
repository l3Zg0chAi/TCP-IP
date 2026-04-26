#include "Application.h"
#include "TCPCommunicator.h"
#include "Logger.h"
#include <thread>
#include <chrono>

void Application::init()
{
    TCPCommunicator::get_instance()->start();
}

void Application::execute()
{
    while (true){
        DEBUG_LOG("start execute");
        receive_from_server();
        DEBUG_LOG("end execute");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Application::receive_from_server()
{
    Packet packet;
    while(TCPCommunicator::get_instance()->receive_packet(packet)){
        // handle data here
    }
    DEBUG_LOG("not element in queue any more");
}

int main() {
    Application::get_instance()->init();
    Application::get_instance()->execute();
    return 0;
}