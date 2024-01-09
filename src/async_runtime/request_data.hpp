//
// Created by shriller44 on 1/8/24.
//

#ifndef SOCKETS_REQUEST_DATA_HPP
#define SOCKETS_REQUEST_DATA_HPP

#include <coroutine>

namespace harmony {
    enum class operation {
        accept, recv, send
    };


    struct request_data {
        std::coroutine_handle<> handle_;
        int client_fd_;
        operation op_;
        std::size_t bytes_;
        sockaddr_storage_t address_;
    };

    struct queue_item {
        std::coroutine_handle<> handle_;
    };
}


#endif //SOCKETS_REQUEST_DATA_HPP
