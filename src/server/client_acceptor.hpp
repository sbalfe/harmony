////
//// Created by shriller44 on 12/8/23.
////
//
//#ifndef SOCKETS_CLIENT_ACCEPTOR_HPP
//#define SOCKETS_CLIENT_ACCEPTOR_HPP
//#include <cstdio>
//#include <cstdlib>
//#include <unistd.h>
//#include <cerrno>
//#include <cstring>
//#include <netdb.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <sys/socket.h>
//#include <string>
//#include <fmt/format.h>
//
//#include "../alias.hpp"
//#include "server_client.hpp"
//#include "../endpoint/ipv6.hpp"
//#include "../endpoint/ipv4.hpp"
//
//namespace shriller {
//
//    std::unique_ptr<endpoint> create_endpoint(sockaddr_storage_t* storage) {
//        auto sa = reinterpret_cast<struct sockaddr *>(storage);
//        if (sa->sa_family == AF_INET) {
//            return std::make_unique<ipv4>(reinterpret_cast<sockaddr_in *>(sa));
//        }
//        return std::make_unique<ipv6>(reinterpret_cast<sockaddr_in6 *>(sa));
//    }
//
//    class client_acceptor {
//    public:
//        client_acceptor() : backlog_{0}, socket_d_{0} {}
//
//        explicit client_acceptor(uint32_t socket_d, int backlog = SOMAXCONN) : socket_d_{socket_d}, backlog_{backlog} {
//            fmt::print("[Server] Accepting connections\n");
//            if (listen(static_cast<int>(socket_d_), static_cast<int>(backlog_)) == -1) {
//                fmt::print(stderr, "listen({}, {})", socket_d_, backlog_);
//                exit(1);
//            }
//        }
//        void accept_client() {
//            sockaddr_storage_t address;
//            socklen_t sin_size = sizeof(address);
//            int client_socket = accept(static_cast<int>(socket_d_), (struct sockaddr *) &(address), &sin_size);
//            if (client_socket == -1) {
//                fmt::print(stderr, "accept() error\n");
//                // throw exception here.
//            }
//            clients_.emplace(client_socket, create_endpoint(&address));
//        }
//
//        server_client& get_client() noexcept {
//            return clients_.front();
//        }
//
//    private:
//        uint32_t socket_d_;
//        int backlog_;
//        std::queue<server_client> clients_;
//    };
//}
//#endif //SOCKETS_CLIENT_ACCEPTOR_HPP
