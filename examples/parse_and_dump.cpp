#include <json_helpers/json_helpers.hpp>
#include <iostream>

namespace jh = json_helpers;

static void run()
{
  const std::string s = R"({"name":"Ada","age":30,"tags":["c++","vix"],"ok":true})";

  auto parsed = jh::parse(s);
  if (!parsed)
  {
    std::cout << "Parse failed\n";
    return;
  }

  const auto &j = *parsed;

  std::cout << "Compact:\n"
            << jh::dump_compact(j) << "\n\n";
  std::cout << "Pretty:\n"
            << jh::dump_pretty(j, 2) << "\n";
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
