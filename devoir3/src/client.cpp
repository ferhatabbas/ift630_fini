#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>

#include <boost/asio.hpp>

#include "utils.h"

typedef void Sigfunc(int);

Sigfunc *signal(int, Sigfunc *);

void callback_signal(int no) {
    switch (no) {
        case SIGINT:
            std::cout << "SIGNAL " << SIGINT << " : Il y a eu un crtl-c, je ne termine pas.\n";
            break;
        case SIGTSTP:
            std::cout << "SIGNAL " << SIGTSTP << " : Il y a eu un crtl-z, je ne termine pas.\n";
    }
}

using boost::asio::ip::tcp;
enum {
    max_length = 1024
};

int main(int argc, char *argv[]) {
    try {
        signal(SIGINT, callback_signal);
        signal(SIGTSTP, callback_signal);
        if (argc != 3) {
            std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(s, resolver.resolve({argv[1], argv[2]}));

        Util::msgtrame trame;
        int length = sizeof(Util::msgtrame);
        boost::asio::read(s, boost::asio::buffer(&trame, length));
        std::cout << "Trame length " << trame.length << "\n"
        << "Trame data " << trame.data << "\n";

        std::ofstream file(trame.data, std::ios::out | std::ios::binary);
        while (true) {
            boost::asio::read(s,
                              boost::asio::buffer(&trame, length));
            file.write(trame.data, trame.length);
            if (trame.mtype == Util::END)
                break;
        }
        file.close();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
