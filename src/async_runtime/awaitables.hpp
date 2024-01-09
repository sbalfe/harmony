//
// Created by shriller44 on 1/8/24.
//

#ifndef SOCKETS_AWAITABLES_HPP
#define SOCKETS_AWAITABLES_HPP

#include <netinet/in.h>
#include <liburing.h>
#include <cstring>
#include <fmt/format.h>
#include <deque>
#include <thread>
#include <utility>

#include "task.hpp"
#include "base_task.hpp"
#include "io_uring.hpp"
#include "io_context.hpp"

namespace harmony {
    struct accept_awaitable {
        explicit accept_awaitable(iouring &uring) : server_socket_{uring.server_socket_} {
            sqe_ = io_uring_get_sqe(&uring.ring_);
            fmt::print("prepping new awaitable\n");
            io_uring_prep_accept(sqe_, server_socket_, (struct sockaddr *) &client_addr_, &client_addr_len_, 0);
        }

        auto operator
        co_await (){
            struct awaiter {
                io_uring_sqe *entry;
                request_data req_data_;

                explicit awaiter(io_uring_sqe *sqe) : entry{sqe} {}

                bool await_ready() { return false; }

                void await_suspend(
                        std::coroutine_handle<> handle) noexcept { // this is the parent handle i.e the task handle
                    req_data_.handle_ = handle;
                    req_data_.op_ = operation::accept;
                    io_uring_sqe_set_data(entry, &req_data_);
                }

                [[nodiscard]] int await_resume() const {
                    return req_data_.client_fd_;
                }
            };

            return awaiter{sqe_};
        }
    private:
        io_uring_sqe *sqe_;
        sockaddr_in client_addr_;
        socklen_t client_addr_len_;
        int server_socket_;
    };
    struct recv_awaitable {
        explicit recv_awaitable(iouring &uring, int client_fd_, char *buf) {
            sqe_ = io_uring_get_sqe(&uring.ring_);
            io_uring_prep_recv(sqe_, client_fd_, buf, 1024, 0);
        }

        auto operator
        co_await (){
            struct awaiter {
                io_uring_sqe *entry;
                request_data req_data_;

                explicit awaiter(io_uring_sqe *sqe) : entry{sqe} {}

                bool await_ready() { return false; }

                void await_suspend(std::coroutine_handle<> handle) noexcept {
                    fmt::print("suspending awaitable (recv)\n");
                    req_data_.handle_ = handle;
                    req_data_.op_ = operation::recv;
                    io_uring_sqe_set_data(entry, &req_data_);
                }

                [[nodiscard]] ssize_t await_resume() const {
                    return req_data_.bytes_;
                }
            };

            return awaiter{sqe_};
        }
    private:
        struct __kernel_timespec ts;
        io_uring_sqe *sqe_;
    };
    struct send_awaitable {
        explicit send_awaitable(iouring &uring, int client_fd_, const char *buf) {
            sqe_ = io_uring_get_sqe(&uring.ring_);
            io_uring_prep_send(sqe_, client_fd_, buf, strlen(buf), 0);
        }

        auto operator
        co_await (){
            struct awaiter {
                io_uring_sqe *entry;
                request_data req_data_;

                explicit awaiter(io_uring_sqe *sqe) : entry{sqe} {}

                bool await_ready() { return false; }

                void await_suspend(std::coroutine_handle<> handle) noexcept {
                    fmt::print("suspending awaitable (send)\n");
                    req_data_.handle_ = handle;
                    req_data_.op_ = operation::send;
                    io_uring_sqe_set_data(entry, &req_data_);
                }

                [[nodiscard]] ssize_t await_resume() const {
                    return req_data_.bytes_;
                }
            };

            return awaiter{sqe_};
        }
    private:
        io_uring_sqe *sqe_;
    };
    struct acceptor_ {

        acceptor(harmony::io_context &ioc) : ioc_{ioc} {}

        accept_awaitable async_accept() {
            return accept_awaitable{ioc_.get_ring()};
        }

        harmony::io_context &ioc_;
    };
    struct socket {

        socket(harmony::io_context &ioc) : ioc_{ioc} {}

        recv_awaitable async_recv(int client_fd_, char *buf) {
            return recv_awaitable{ioc_.get_ring(), client_fd_, buf};
        }

        send_awaitable async_send(int client_fd_, const char *send_buf) {
            return send_awaitable{ioc_.get_ring(), client_fd_, send_buf};
        }

        harmony::io_context &ioc_;
    };
}

#endif //SOCKETS_AWAITABLES_HPP
