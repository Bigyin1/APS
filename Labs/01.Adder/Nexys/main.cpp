#include <ixwebsocket/IXWebSocketServer.h>

#include <iostream>
#include <thread>

#include "Nexys.hpp"

int main()
{
    int         port = 8081;
    std::string host("127.0.0.1");

    ix::WebSocketServer server(port, host);

    std::unique_ptr<Nexys> board = nullptr;

    std::thread boardLoop;


    server.setOnClientMessageCallback(
        [&board](std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket& webSocket,
                 const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Open)
            {
                std::cout << "New connection" << std::endl;

                if (board != nullptr)
                    return;

                board = std::make_unique<Nexys>(webSocket);


            }
            else if (msg->type == ix::WebSocketMessageType::Close) {

                Verilated::ver
                board.reset(nullptr);
            }
            else if (msg->type == ix::WebSocketMessageType::Message)
            {
                webSocket.send(msg->str, msg->binary);
            }
        });

    auto res = server.listen();
    if (!res.first)
        return 1;

    server.start();

    server.wait();
}
