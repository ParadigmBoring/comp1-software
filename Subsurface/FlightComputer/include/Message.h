#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

// Template parameter must be a protobuf message

template<typename Protobuf>
class Message
{
public:
	static constexpr int headerLength = sizeof(uint32_t);
	static constexpr int maxBodyLength = 1024;

	Message()
		: m_bodyLength(0)
	{
		m_data.resize(256);
	}

	Message(const Protobuf& message)
	{
		m_data.resize(256);
		serialize(message);
	}

	Message(const Message&) = default;

	std::size_t length() const { return headerLength + m_bodyLength; }

	const char* data() const { return m_data.data(); }
	char* data() { return m_data.data(); }

	std::size_t bodyLength() const { return m_bodyLength; }

	const char* body() const { return m_data.data() + headerLength; }
	char* body() { return m_data.data() + headerLength; }

	bool decodeHeader()
	{
		std::memcpy(&m_bodyLength, m_data.data(), headerLength);
		if (m_bodyLength > maxBodyLength)
		{
			m_bodyLength = 0;
			return false;
		}
		return true;
	}

	bool serialize(const Protobuf& message)
	{
		std::string serialized;
		bool success = message.SerializeToString(&serialized);
		m_bodyLength = serialized.length();

		std::memcpy(m_data.data(), &m_bodyLength, sizeof(m_bodyLength));

		m_data.resize(headerLength + m_bodyLength);
		std::memcpy(m_data.data() + headerLength, serialized.data(), serialized.length());

		return success;
	}

	bool deserialize(Protobuf& message) const
	{
		return message.ParseFromString(std::string(body(), body() + bodyLength()));
	}

private:
	std::vector<char> m_data;
	std::size_t m_bodyLength;
};
