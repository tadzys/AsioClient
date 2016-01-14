#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <random>
#include <string>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		tcp::socket s(io_service);
		tcp::resolver resolver(io_service);
		boost::asio::connect(s, resolver.resolve({ argv[1], argv[2] }));


		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 1024);
		
		decltype (dis(gen)) random_number;

		typedef decltype (dis(gen)) rtp;

		char buffer[100];
		
		while (true)
		{
			random_number = dis(gen);
			memcpy_s(buffer, sizeof(rtp), reinterpret_cast<const char*>(&random_number), sizeof(rtp));
			auto data = std::to_string(random_number);

			std::cout << "Sending: " << data << std::endl;
			//sboost::asio::write(s, boost::asio::buffer(data.data(), data.length()));
			boost::asio::write(s, boost::asio::buffer(buffer, sizeof(rtp)));

			char reply[max_length];

			size_t reply_length = boost::asio::read(s,
				boost::asio::buffer(reply, sizeof(double)));

			double back;
			memcpy_s(&back, sizeof(double), reply, sizeof(double));

			std::cout << "Reply is: ";
			std::cout<< back;
			std::cout << "\n";
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}