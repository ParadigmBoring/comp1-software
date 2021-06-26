#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Message.h"
#include "Paradigm.pb.h"

#include "easylogging++.h"
#include <iostream>
#include <vector>

INITIALIZE_EASYLOGGINGPP

TEST_CASE("Check that sizes are correct in message struct")
{
	Person person;
	person.set_name("Daniel Burke");
	person.set_email("danielseanburke@gmail.com");
	auto serialized = person.SerializeAsString();
	char size[5];
	sprintf(size, "%4d", static_cast<uint32_t>(serialized.length()));

	Message<Person> message(person);

	SECTION("Check sizes are equal")
	{
		REQUIRE(serialized.size() == message.bodyLength());
		REQUIRE(serialized.size() + sizeof(uint32_t) == message.length());
	}

	SECTION("Check contents are equal")
	{
		REQUIRE(*(serialized.data()) == *(message.body()));
	}

	SECTION("Check that protobuf is deserialized correctly")
	{
		Person person1;
		REQUIRE(message.deserialize(person1) == true);
	}
}
