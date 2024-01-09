//
// Created by shriller44 on 1/9/24.
//

#ifndef SOCKETS_ASYNC_SOCKET_HPP
#define SOCKETS_ASYNC_SOCKET_HPP

#include <netinet/in.h>
#include <liburing.h>
#include <cstring>
#include <fmt/format.h>
#include <deque>
#include <thread>
#include <utility>

#include "../async_runtime/io_uring.hpp"
#include "../async_runtime/io_context.hpp"


namespace harmony {
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

    struct socket {

        recv_awaitable async_recv(io_context& io_ctx ,int client_fd, char *buf) {
            return recv_awaitable{io_ctx.get_ring(), client_fd, buf};
        }

        send_awaitable async_send(io_context& io_ctx , int client_fd, const char *send_buf) {
            return send_awaitable{io_ctx.get_ring(), client_fd, send_buf};
        }

    };
}

#endif //SOCKETS_ASYNC_SOCKET_HPP
