//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>

using boost::asio::ip::tcp;



class session;


std::list<session*> sessionsList;
std::mutex sessionListMutex;



class session
        : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
            : socket_(std::move(socket))
    {
        sessionListMutex.lock();
        sessionsList.push_back(this);
        sessionListMutex.unlock();
        std::cout << "Session opened.\n";
    }

    ~session(){
        sessionListMutex.lock();
        sessionsList.remove(this);
        sessionListMutex.unlock();
        std::cout << "Session closed.\n";
    }

    void start()
    {
        do_read();
    }

    inline void writeData(uint8_t const *data, std::size_t length) {
        boost::asio::async_write(socket_, boost::asio::buffer(data, length),
                [this](boost::system::error_code ec, std::size_t){});
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                [this, self](boost::system::error_code ec, std::size_t length){});
//        if (data_[0] == 'E' && data_[1] == 'O' && data_[2] == 'F')
    }

    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                {
                    if (!ec)
                    {
                        do_read();
                    }
                });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class server
{
public:
    server(boost::asio::io_service& io_service, short port)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
              socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                [this](boost::system::error_code ec)
                {
                    if (!ec)
                    {
                        std::make_shared<session>(std::move(socket_))->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};





void sendingThread() {
    uint8_t buffer[200] = {};

    buffer[0] = 'a';
    buffer[1] = 'r';
    buffer[2] = 't';
    buffer[3] = 'u';
    buffer[4] = 'r';

    while (true) {
        sessionListMutex.lock();
        if (sessionsList.empty()) {
            sessionListMutex.unlock();
            Sleep(1000);
            continue;
        }
        for (auto &elem : sessionsList) {
            elem->writeData(buffer,5);
        }
        sessionListMutex.unlock();
        Sleep(1000);
    }
}



int main(int argc, char* argv[])
{
    std::thread Thread(sendingThread);
    Thread.detach();

    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        server s(io_service, std::atoi(argv[1]));

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}



