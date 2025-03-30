#include <SerializeX.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <optional>
#include <vector>

struct Address
{
  std::optional<std::string> street;
  std::string city; // `json:"name,omitempty"`
  int zip;
};

struct Person
{
  std::string name;
  int age;
  std::unique_ptr<Address> address;
  std::optional<double> salary;
  // std::vector<std::string> pets; // NOT SUPPORTED YET
};

TEST_CASE("Fieds are vistied", "[json]")
{
  auto person =
      Person{"John Doe", 30, std::make_unique<Address>("123 Main St", "Anytown", 12345), std::nullopt, {"cat", "dog"}};
  auto serializer = SerializeX::Json::Serializer(person);

  std::cout << serializer << std::endl;

  REQUIRE(true);
}
