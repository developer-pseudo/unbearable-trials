// includes
#include <iostream>
#include <chrono>

// windows will scream at you if you dont add this since every version interpretates networking differently
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

// asio includes
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(1 * 1024); // create a reasonably large buffer since we have no idea how big the data might be

void GrabSomeData(asio::ip::tcp::socket& socket) { // create a function to grab data from our socket
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length) { // prototype of a lambda function
			if (!ec) { // if there is no errors we ...
				std::cout << "\n\nread " << length << " bytes\n\n"; // ... send the length of bytes that we got from the server

				for (int i = 0; i < length; i++) // a wild for loop has appeared!
					std::cout << vBuffer[i]; // dont forget to print out the data :smart:

				GrabSomeData(socket); // no this is not a recursive nightmare, it just primes it so it runs in the background :smart:
			}
		}
	);
}

int main() { // the main function of our program
	asio::error_code ec; // use ec to get specific errors
	asio::io_context context; // create our platform specific interface context
	asio::io_context::work idleWork(context); // torture our context so asio doesnt finish

	std::thread thrContext = std::thread([&]() {context.run(); }); // run our context in its own thread so it wont block our program

	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80); // declare where asio expects people to connect to (im using the ip address of https://example.com)
	asio::ip::tcp::socket socket(context); // create our socket which context will implement

	std::cout << "attempting to connect" << std::endl;
	socket.connect(endpoint, ec); // try connecting to our TCP server

	// check for connection errors
	if (!ec) {
		std::cout << "connected to the socket" << std::endl;
	}
	else {
		std::cout << "failed to connect to the socket, error message below" << std::endl << ec.message() << std::endl; // send our error so we know what happened
	}

	if (socket.is_open()) { // check if the socket is opened
		GrabSomeData(socket); // prime our data getter function BEFORE we send the request

		std::string sRequest = // send a request to a server
			"GET /index.html HTTP/1.1\r\n"
			"Host: david-barr.co.uk\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec); // send as much data as the socket can

		// give our program some sleep pills ;)
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20000ms);
	}

	return 0;
}