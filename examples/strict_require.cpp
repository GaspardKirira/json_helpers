#include <json_helpers/json_helpers.hpp>
#include <iostream>
#include <string>

namespace jh = json_helpers;

static void run()
{
  auto j = jh::parse_or_throw(R"({
    "id": 7,
    "name": "Ada",
    "meta": { "role": "engineer" }
  })");

  const int id = jh::require<int>(j, "id");
  const std::string name = jh::require<std::string>(j, "name");

  std::cout << "id: " << id << "\n";
  std::cout << "name: " << name << "\n";

  const auto &meta = jh::require_object(j.at("meta"));
  std::cout << "role: " << jh::require<std::string>(meta, "role") << "\n";

  std::cout << "\nNow triggering a strict error...\n";
  (void)jh::require<int>(j, "name"); // invalid type, throws
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
    std::cerr << "Strict error caught: " << e.what() << "\n";
    return 0;
  }
}
