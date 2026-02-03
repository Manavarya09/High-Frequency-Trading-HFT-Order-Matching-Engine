#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    std::cout << "Connected to server. Sending orders..." << std::endl;

    // Send some sample orders
    std::string orders[] = {
        "ORDER|1|AAPL|B|10000|100\n",
        "ORDER|2|AAPL|S|10000|50\n",
        "ORDER|3|AAPL|B|10050|200\n",
        "ORDER|4|AAPL|S|10050|150\n"
    };

    for (const auto& order : orders) {
        send(sock, order.c_str(), order.size(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Orders sent. Closing connection." << std::endl;
    close(sock);
    return 0;
}