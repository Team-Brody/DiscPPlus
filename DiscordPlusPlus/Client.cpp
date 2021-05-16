#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "json.hpp"
#include "Client.h"
#include <iostream>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>


#define active true

namespace json = nlohmann;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;
typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef client::connection_ptr connection_ptr;

std::string url = "wss://gateway.discord.gg/?v=8&encoding=json";
auto hbAck = json::json::parse(R"({"op": 1,"d": null})"); // heartbeat
std::int64_t hbInterval = 41250;
client c;
// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;
    auto jsg = json::json::parse(msg->get_payload());
    int opcode = jsg["op"];
    switch (opcode) {
        case 10 :
            hbInterval = jsg["d"]["heartbeat_interval"] - 75;
            std::cout << "New Hearbeat Interval is: " << hbInterval << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(hbInterval));
            c->send(hdl, hbAck.dump(), websocketpp::frame::opcode::text);
            std::cout << "heartbeat sent" << "\n";
            break;
        case 11 :
            std::this_thread::sleep_for(std::chrono::milliseconds(hbInterval));
            c->send(hdl, hbAck.dump(), websocketpp::frame::opcode::text);
            std::cout << "heartbeat sent" << "\n";
            break;
        default :
            break;
    }

    

}
void on_close(client* c, websocketpp::connection_hdl hdl) {
    try {
        // establishing a WSS connection so we can get opcode 10 and begin the heartbeat

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c->get_connection(url, ec);
        std::cout << "con altered" << '\n';



        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return;
        }

        std::cout << "Client is attempting connection" << '\n';
        c->connect(con);
        c->get_alog().write(websocketpp::log::alevel::app, "Connecting to " + url);
        std::cout << "Client has connected and is attempting to run" << '\n';
        c->run();
        std::cout << "Client has connected and has ran" << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Connection failed, error: " << e.what() << '\n';
        return;
    }
}




void DiscPPlus::Client::sendWSReq(json::json payload="", std::string wsUrl="") {
    try {
        // establishing a WSS connection so we can get opcode 10 and begin the heartbeat

        
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(wsUrl, ec);
        std::cout << "con altered" << '\n';
        


        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return;
        }
        if (payload != "") {
            con->send(payload.dump(), websocketpp::frame::opcode::text);
        }

        std::cout << "Client is attempting connection" << '\n';
        c.connect(con);
        c.get_alog().write(websocketpp::log::alevel::app, "Connecting to " + wsUrl);
        std::cout << "Client has connected and is attempting to run" << '\n';
        c.run();
        std::cout << "Client has connected and has ran" << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Connection failed, error: " << e.what() << '\n';
        return;
    }
}

bool DiscPPlus::Client::establishConnection()
{

    bool result{};
    c.init_asio();
    c.set_tls_init_handler([this](websocketpp::connection_hdl) {
        return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv1);
    });

    c.set_access_channels(websocketpp::log::alevel::all);
    c.set_error_channels(websocketpp::log::elevel::all);


    //c.clear_access_channels(websocketpp::log::alevel::frame_payload);


    c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));


    try
    {
        // getting gateway
        httplib::Client cli("https://discord.com");

        auto res = cli.Get("/api/v8/gateway");
        if (res) {
            std::cout << res->status << '\n';
            std::cout << res->body << '\n';
        }
        sendWSReq("", url);


    }
    catch (const std::exception& e)
    {
        std::cerr << "Connection failed, error: " << e.what() << '\n';
        result = false;
    }
    result = false;
    





    return result;
    std::cout << "loop end" << "\n";
}

