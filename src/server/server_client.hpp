////
//// Created by shriller44 on 12/8/23.
////
//
//#ifndef SOCKETS_SERVER_CLIENT_HPP
//#define SOCKETS_SERVER_CLIENT_HPP
//#include <cstdio>
//#include <cstdlib>
//#include <unistd.h>
//#include <cstring>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <memory>
//#include <fmt/format.h>
//#include <arpa/inet.h>
//#include <sstream>
//#include <queue>
//#include <optional>
//
//#include "../endpoint/endpoint.hpp"
//
//namespace shriller {
//    struct server_client {
//        explicit server_client(int socket, std::unique_ptr<endpoint> endpoint):
//            client_socket_{socket},
//            endpoint_{std::move(endpoint)}
//        {}
//
//        [[nodiscard]] std::string address() const noexcept { return endpoint_->address(); }
//        [[nodiscard]] std::string hostname() const noexcept { return endpoint_->hostname().value(); }
//        [[nodiscard]] uint16_t port() const noexcept { return endpoint_->port(); }
//
//        void send_message(std::string_view message) const {
//            const char *data = message.data();
//            size_t length = message.length();
//            if (send(client_socket_, data, length, 0) == -1)
//                fmt::print(stderr, "send()");
//            fmt::print("--sent---\r\n{}", message);
//        }
//
//        int client_socket_;
//        std::unique_ptr <endpoint> endpoint_;
//    };
//}
//
//#endif //SOCKETS_SERVER_CLIENT_HPP
