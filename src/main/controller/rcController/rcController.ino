#include <iostream>
#include <string>
#include <cstring>
#include <chrono> 
#include <thread>
#include <WS2tcpip.h>
 
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    using socklen_t = int;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> 
#endif
 
const char* DRONE_IP = "192.168.240.124";
const unsigned int DRONE_UDP_PORT = 4210;
 
bool init_sockets() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}
 
void cleanup_sockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

void close_socket(int sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}
 
int main() {
    if (!init_sockets()) {
        std::cerr << "Failed to initialize sockets." << std::endl;
        return 1;
    }
 
    int sockfd;
    struct sockaddr_in servaddr;
 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        cleanup_sockets();
        return 1;
    }
 
    memset(&servaddr, 0, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(DRONE_UDP_PORT);
    if (inet_pton(AF_INET, DRONE_IP, &servaddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close_socket(sockfd);
        cleanup_sockets();
        return 1;
    }
 
    std::cout << "RC Controller Started." << std::endl;
    std::cout << "Sending commands to " << DRONE_IP << ":" << DRONE_UDP_PORT << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  'w': THROTTLE_UP" << std::endl;
    std::cout << "  's': THROTTLE_DOWN" << std::endl;
    std::cout << "  'a': MOVE_LEFT" << std::endl;
    std::cout << "  'd': MOVE_RIGHT" << std::endl;
    std::cout << "  'q': TURN_LEFT" << std::endl;
    std::cout << "  'e': TURN_RIGHT" << std::endl;
    std::cout << "  'l': LAND" << std::endl;
    std::cout << "  'p': PING (manual)" << std::endl;
    std::cout << "  'x': Exit" << std::endl;
    std::cout << "Press Enter after each command." << std::endl;
 
 
    std::string command_to_send;
    std::string input_char;
 
    std::thread ping_thread([&]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            const char* ping_cmd = "PING";
            sendto(sockfd, ping_cmd, strlen(ping_cmd), 0,
                   (const struct sockaddr*)&servaddr, sizeof(servaddr));
        }
    });
    ping_thread.detach();
 
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input_char);
 
        if (input_char == "w") {
            command_to_send = "THROTTLE_UP";
        } else if (input_char == "s") {
            command_to_send = "THROTTLE_DOWN";
        } else if (input_char == "a") {
            command_to_send = "MOVE_LEFT";
        } else if (input_char == "d") {
            command_to_send = "MOVE_RIGHT";
        } else if (input_char == "q") {
            command_to_send = "TURN_LEFT";
        } else if (input_char == "e") {
            command_to_send = "TURN_RIGHT";
        } else if (input_char == "l") {
            command_to_send = "LAND";
        } else if (input_char == "p") {
            command_to_send = "PING";
        } else if (input_char == "x") {
            std::cout << "Exiting controller." << std::endl;
            break;
        } else {
            std::cout << "Unknown command." << std::endl;
            continue;
        }
 
        sendto(sockfd, command_to_send.c_str(), command_to_send.length(), 0,
               (const struct sockaddr*)&servaddr, sizeof(servaddr));
        std::cout << "Sent: " << command_to_send << std::endl;
    }
 
    close_socket(sockfd);
    cleanup_sockets();
    return 0;
}