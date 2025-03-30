#include <SerializeX.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <iostream>

struct Address {
  std::optional<std::string> street;
  std::string city; // `json:"name,omitempty"`
  int zip;
};

struct Person {
  std::string name;
  int age;
  std::unique_ptr<Address> address;
};

TEST_CASE("Fieds are vistied", "[json]") {
  auto person =
      Person{"John Doe", 30,
             std::make_unique<Address>("123 Main St", "Anytown", 12345)};
  auto serializer = SerializeX::Json::Serializer(person);

  std::cout << std::string(serializer) << std::endl;

  REQUIRE(true);
}
