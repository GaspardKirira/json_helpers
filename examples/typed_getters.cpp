#include <json_helpers/json_helpers.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace jh = json_helpers;

static void run()
{
  auto j = jh::parse_or_throw(R"({
    "name": "Ada",
    "age": 30,
    "active": true,
    "tags": ["c++", "json", "vix"]
  })");

  const auto name = jh::get_or<std::string>(j, "name", "unknown");
  const auto age = jh::get_or<int>(j, "age", 0);
  const auto city = jh::get_or<std::string>(j, "city", "Kampala");

  const auto tags = jh::get_array_or_empty<std::string>(j, "tags");

  std::cout << "name: " << name << "\n";
  std::cout << "age: " << age << "\n";
  std::cout << "city(default): " << city << "\n";

  std::cout << "tags:\n";
  for (const auto &t : tags)
    std::cout << "  - " << t << "\n";
}

int main()
{
  try
  {
    run();
    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
