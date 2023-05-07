#include <iostream>
#include <deque>
#include <memory>
#include <set>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
const int DEFAULT_PORT = 5000;

class ClientConnection;

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        accept();
    }

private:
    void accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    auto connection = std::make_shared<ClientConnection>(std::move(socket), clients_);
                    connection->start();
                    clients_.insert(connection);
                }

                accept();
            });
    }

    tcp::acceptor acceptor_;
    std::set<std::shared_ptr<ClientConnection>> clients_;
};

class ClientConnection : public std::enable_shared_from_this<ClientConnection> {
public:
    ClientConnection(tcp::socket socket, std::set<std::shared_ptr<ClientConnection>>& clients)
        : socket_(std::move(socket)), clients_(clients) {}

    void start() {
        read();
    }

    void deliver(const std::string& msg) {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_, boost::asio::buffer(msg),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    clients_.erase(shared_from_this());
                }
            });
    }

private:
    void read() {
        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string msg(buffer_.data(), bytes_transferred);
                    for (auto& client : clients_) {
                        if (client != shared_from_this()) {
                            client->deliver(msg);
                        }
                    }
                    read();
                }
                else {
                    clients_.erase(shared_from_this());
                }
            });
    }

    tcp::socket socket_;
    std::set<std::shared_ptr<ClientConnection>>& clients_;
    std::array<char, 1024> buffer_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        short port = DEFAULT_PORT;

        ChatServer server(io_context, port);

        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
