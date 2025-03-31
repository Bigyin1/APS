#include <ixwebsocket/IXWebSocketServer.h>

#include <iostream>
#include <thread>

#include "Nexys.hpp"

int main()
{

    int         port = 8081;
    std::string host("127.0.0.1"); // If you need this server to be accessible on a different
                                   // machine, use "0.0.0.0"
    ix::WebSocketServer server(port, host);

    server.setOnClientMessageCallback(
        [](std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket& webSocket,
           const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Open)
            {
                std::cout << "New connection" << std::endl;
            }
            else if (msg->type == ix::WebSocketMessageType::Message)
            {

                webSocket.send(msg->str, msg->binary);
            }
        });

    auto res = server.listen();
    if (!res.first)
    {
        return 1;
    }

    // Run the server in the background. Server can be stoped by calling server.stop()
    server.start();

    // Block until server.stop() is called.
    server.wait();
}
