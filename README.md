# SERIALIZEX

## C++20 serialization library based on Boost.PFR compile-time reflection features

## TODO:

* [ ] std::vector, std::array, std::variant, std::any, std::tuple support
* [ ] Allow RAW pointers usage ...
* [ ] Add meta-TAG types, e.g JsonTag<std::string, "Address", omitempty_policy>
* [ ] Json deserialization support ...

## Usage example
    // define your aggregates ...
    struct Address {
        std::optional<std::string> street;
        std::string city;
        int zip;
    };

    struct Person {
        std::string name;
        int age;
        std::unique_ptr<Address> address;
        std::optional<double> salary;
    };

    // now we can create an instance and serialize it to Json without any efforts
    auto person =
        Person{"John Doe", 30,
                std::make_unique<Address>("123 Main St", "Anytown", 12345),
                std::nullopt};
    auto serializer = SerializeX::Json::Serializer(person);

    std::cout << serializer << std::endl;
    // prints
    // {"name":"John Doe","age":30,"address":{"street":"123 Main St","city":"Anytown","zip":12345},"salary":null}
    