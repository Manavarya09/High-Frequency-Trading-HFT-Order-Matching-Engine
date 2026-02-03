#pragma once

#include "Gateway.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <iostream>

class TCPServer {
public:
    TCPServer(int port, Gateway& gateway) : port_(port), gateway_(gateway), running_(true) {}

    void start() {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd_ == 0) {
            std::cerr << "Socket failed" << std::endl;
            return;
        }

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port_);

        if (bind(server_fd_, (sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            return;
        }

        if (listen(server_fd_, 10) < 0) {
            std::cerr << "Listen failed" << std::endl;
            return;
        }

        std::cout << "Server listening on port " << port_ << std::endl;

        while (running_) {
            int client_fd = accept(server_fd_, nullptr, nullptr);
            if (client_fd < 0) continue;

            std::thread(&TCPServer::handleClient, this, client_fd).detach();
        }
    }

    void stop() {
        running_ = false;
        close(server_fd_);
    }

private:
    void handleClient(int client_fd) {
        char buffer[1024];
        while (running_) {
            int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read <= 0) break;
            buffer[bytes_read] = '\0';
            std::string message(buffer);
            // Assume one message per read
            gateway_.parseAndQueueOrder(message);
        }
        close(client_fd);
    }

    int port_;
    Gateway& gateway_;
    int server_fd_;
    std::atomic<bool> running_;
};