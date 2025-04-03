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

    server.setOnClientMessageCallback(
        [&board](std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket& webSocket,
                 const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Open)
            {
                if (board != nullptr)
                {
                    webSocket.close();
                    return;
                }

                board = std::make_unique<Nexys>(webSocket);

                std::thread mloop(&Nexys::StartMainLoop, board.get());
                mloop.detach();
            }
            else if (msg->type == ix::WebSocketMessageType::Close)
            {
                board.get()->StopMainLoop();
                std::this_thread::sleep_for(5ms);
                board.reset(nullptr);
            }
            else if (msg->type == ix::WebSocketMessageType::Message)
            {
                auto j = json::parse(msg->str);

                std::cout << j.dump(4) << std::endl;
                
                board.get()->UpdateInputs(j);
            }
        });

    auto res = server.listenAndStart();
    if (!res)
        return 1;

    std::cout << "Server Started!" << std::endl;

    server.wait();
}
