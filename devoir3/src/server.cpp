#include <cstdlib>
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <chrono>
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
const int max_length = 1024;

void session(tcp::socket sock) {
    try {
        Util::msgtrame trame;
        trame.mtype = Util::CONTINUE;
        int length = sizeof(Util::msgtrame);
        /*Read file*/
        std::ifstream file("petit_fichier", std::ios::binary | std::ios::in);
        while (true) {
            unsigned int i = 0;
            for (i = 0; i < Util::BUF && file.get(trame.data[i]); ++i);
            trame.length = i;
            if (i < Util::BUF - 1) {
                trame.mtype = Util::END;
                boost::asio::write(sock, boost::asio::buffer(&trame, length));
                std::cout << "write done.\n";
                break;
            }
            boost::asio::write(sock, boost::asio::buffer(&trame, length));
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        file.close();
    }
    catch (std::exception &e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_service &io_service, unsigned short port) {
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    Util::msgtrame trame;
    int copie = 0;
    for (; ;) {
        std::stringstream ss;
        ss << "fichier_copie_";
        tcp::socket sock(io_service);
        a.accept(sock);
        /*Ajout du nom de fichier en send*/
        ss << copie;
        std::cout << "Connection start.\n";
        std::string str = ss.str();
        std::copy(str.begin(), str.end(), trame.data);
        trame.mtype = Util::BEGIN;
        trame.length = ss.str().size();
        boost::asio::write(sock, boost::asio::buffer(&trame, sizeof(Util::msgtrame)));
        std::thread(session, std::move(sock)).detach();
        ++copie;
    }
}

int main(int argc, char *argv[]) {
    signal(SIGTSTP, callback_signal);
    try {
        if (argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        server(io_service, std::atoi(argv[1]));
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
