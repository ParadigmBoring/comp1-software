#include "FlightComputer.h"
#include "Message.h"
#include "NetClient.h"
#include "Paradigm.pb.h"

#include "easylogging++.h"
#include <iostream>
#include <vector>

INITIALIZE_EASYLOGGINGPP

using asio::ip::tcp;
using asio::ip::udp;

int main(int argc, char* argv[])
{
	asio::io_context context;

	// UDP Test
	/*udp::resolver resolver(context);
	auto endpoints = resolver.resolve(udp::v4(), "127.0.0.1", "10000");
	UdpClient<Person> client(context, endpoints);

	std::thread t1([&context]() { context.run(); });
	while (true)
	{
		static int count = 0;
		Person person;
		person.set_name("Daniel Burke");
		person.set_email("danielseanburke@gmail.com");
		auto phone = person.add_phones();

		Message<Person> message(person);
		client.write(message);
		LOG(INFO) << client.getReadMessage().body();
		std::this_thread::sleep_for(std::chrono::duration(std::chrono::milliseconds(1000)));
	}*/

	// TCP Test
	tcp::resolver resolver(context);
	auto endpoints = resolver.resolve(tcp::v4(), "127.0.0.1", "10000");
	TcpClient<Person> client(context, endpoints);

	std::thread t1([&context]() { context.run(); });
	while (true)
	{
		static int count = 0;
		Person person;
		person.set_name("Daniel Burke");
		person.set_email("danielseanburke@gmail.com");
		auto phone = person.add_phones();

		Message<Person> message(person);
		client.write(message);
		LOG(INFO) << client.getReadMessage().body();
		std::this_thread::sleep_for(std::chrono::duration(std::chrono::milliseconds(1000)));
	}
	
	client.close();
	t1.join();

	return 0;
}
