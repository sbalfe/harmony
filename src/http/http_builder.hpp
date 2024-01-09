//
// Created by shriller44 on 12/8/23.
//

#ifndef SOCKETS_HTTP_BUILDER_HPP
#define SOCKETS_HTTP_BUILDER_HPP

#include <fmt/format.h>

namespace harmony {
    class http_builder {
    public:
        explicit http_builder(std::string_view error_code, std::string_view body) : error_code_{error_code},
                                                                                   body_{body} {}
        std::string_view get_response_data() {
            http_response_ += fmt::format("HTTP/{} {} Not Found\r\n", version_, error_code_);
            http_response_ += fmt::format("Content-Type: {}\r\n", "text/html");
            http_response_ += fmt::format("Content-Length: {}\r\n\r\n", body_.size());
            http_response_ += fmt::format("{}\n", body_);
            return http_response_;
        }

    private:
        std::string_view error_code_;
        std::string_view body_;
        std::string_view version_ = "1.1";
        std::string http_response_;
    };
}

#endif //SOCKETS_HTTP_BUILDER_HPP
