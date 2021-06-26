#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>

#include "Message.h"
#include "Paradigm.pb.h"

#include <deque>
#include <iostream>

using asio::ip::tcp;
using asio::ip::udp;

template<typename Proto>
class TcpClient
{
public:
	TcpClient(asio::io_context& ioContext, const tcp::resolver::results_type& endpoints)
		: m_ioContext(ioContext), m_socket(ioContext)
	{
		connect(endpoints);
	}

	void write(const Message<Proto>& message)
	{
		asio::post(m_ioContext,
			[this, message]()
			{
				bool writeInProgress = !m_writeMessages.empty();
				m_writeMessages.push_back(message);
				if (!writeInProgress)
				{
					internalWrite();
				}
			});
	}

	bool connected() const { return m_isConnected; }

	void close()
	{
		asio::post(m_ioContext, [this]() {m_socket.close(); });
		m_isConnected = false;
	}

	const Message<Proto>& getReadMessage() { return m_readMessage; }

private:
	void connect(const tcp::resolver::results_type& endpoints)
	{
		asio::async_connect(m_socket, endpoints,
			[this, &endpoints](std::error_code ec, tcp::endpoint)
			{
				if (!ec)
				{
					m_isConnected = true;
					readHeader();
				}
				else
				{
					std::cout << "Could Not Connect!" << std::endl;
					connect(endpoints);
				}
			});
	}

	void readHeader()
	{
		asio::async_read(m_socket,
			asio::buffer(m_readMessage.data(), Message<Proto>::headerLength),
			[this](std::error_code ec, std::size_t)
			{
				if (!ec && m_readMessage.decodeHeader())
				{
					readBody();
				}
				else if (ec)
				{
					std::cout << ec << std::endl;
				}
				else
				{
					readHeader();
				}
			});
	}

	void readBody()
	{
		asio::async_read(m_socket,
			asio::buffer(m_readMessage.body(), m_readMessage.bodyLength()),
			[this](std::error_code ec, std::size_t)
			{
				if (!ec)
				{
					std::cout << m_readMessage.data() << std::endl;
					m_readMessage.decodeHeader();
					Person person;
					if (m_readMessage.deserialize(person))
					{
						std::cout << "Successfully parsed data!" << std::endl;
						std::cout << "Email: " << person.email() << std::endl;
						std::cout << "Name: " << person.name() << std::endl;
					}
					readHeader();
				}
				else
				{
					m_socket.close();
				}
			});
	}

	void internalWrite()
	{
		asio::async_write(m_socket,
			asio::buffer(m_writeMessages.front().data(), m_writeMessages.front().length()),
			[this](std::error_code ec, std::size_t)
			{
				if (!ec)
				{
					m_writeMessages.pop_front();
					if (!m_writeMessages.empty())
					{
						internalWrite();
					}
				}
				else
				{
					m_socket.close();
				}
			});
	}

	asio::io_context& m_ioContext;
	tcp::socket m_socket;
	Message<Proto> m_readMessage;
	std::deque<Message<Proto>> m_writeMessages;
	bool m_isConnected = false;
};


// TODO change to serialize raw protobuf instead of message abstraction
template<typename Proto>
class UdpClient
{
public:
	UdpClient(asio::io_context& ioContext, const udp::resolver::results_type endpoints)
		: m_ioContext(ioContext), m_socket(ioContext, udp::endpoint(udp::v4(), 0)), m_endpoint(endpoints.begin()->endpoint())
	{
		connect();
	}

	void write(const Message<Proto>& message)
	{
		asio::post(m_ioContext,
			[this, message]()
			{
				bool writeInProgress = !m_writeMessages.empty();
				m_writeMessages.push_back(message);
				if (!writeInProgress)
				{
					internalWrite();
				}
			});
	}

	void close()
	{
		asio::post(m_ioContext, [this]() {m_socket.close(); });
	}

	const Message<Proto>& getReadMessage() { return m_readMessage; }

private:
	void connect()
	{
		read();
	}

	void read()
	{
		m_socket.async_receive_from(
			asio::buffer(m_readMessage.data(), Message<Proto>::maxBodyLength),
			m_senderEndpoint,
			[this](std::error_code ec, std::size_t)
			{
				if (!ec)
				{
					std::cout << m_readMessage.data() << std::endl;
					m_readMessage.decodeHeader();
					Person person;
					if (m_readMessage.deserialize(person))
					{
						std::cout << "Successfully parsed data!" << std::endl;
						std::cout << "Email: " << person.email() << std::endl;
						std::cout << "Name: " << person.name() << std::endl;
					}
					read();
				}
				else
				{
					std::cout << ec << std::endl;
					read();
				}
			});
	}

	void internalWrite()
	{
		m_socket.async_send_to(
			asio::buffer(m_writeMessages.front().data(), m_writeMessages.front().length()),
			m_endpoint,
			[this](std::error_code ec, std::size_t)
			{
				if (!ec)
				{
					m_writeMessages.pop_front();
					if (!m_writeMessages.empty())
					{
						internalWrite();
					}
				}
				else
				{
					m_socket.close();
				}
			});
	}
	asio::io_context& m_ioContext;
	udp::socket m_socket;
	Message<Proto> m_readMessage;
	std::deque<Message<Proto>> m_writeMessages;
	const udp::endpoint m_endpoint;
	udp::endpoint m_senderEndpoint;
};
