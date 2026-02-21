#include <json_helpers/json_helpers.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace jh = json_helpers;

static void test_parse_ok()
{
  const std::string s = R"({"name":"Ada","age":30,"tags":["c++","json"],"ok":true})";

  auto parsed = jh::parse(s);
  assert(parsed.has_value() && "parse should succeed");

  const auto &j = *parsed;
  assert(j.is_object());
  assert(jh::has(j, "name"));
  assert(jh::has(j, "age"));
  assert(jh::has(j, "tags"));
}

static void test_parse_fail()
{
  const std::string s = R"({"name":)";
  auto parsed = jh::parse(s);
  assert(!parsed.has_value() && "parse should fail");
}

static void test_parse_or_throw()
{
  const std::string s = R"({"x":1})";
  auto j = jh::parse_or_throw(s);
  assert(j.is_object());
  assert(jh::require<int>(j, "x") == 1);

  bool threw = false;
  try
  {
    (void)jh::parse_or_throw("{bad");
  }
  catch (...)
  {
    threw = true;
  }
  assert(threw && "parse_or_throw must throw on invalid JSON");
}

static void test_dump()
{
  jh::json j;
  j["a"] = 1;
  j["b"] = "x";

  const auto compact = jh::dump_compact(j);
  const auto pretty = jh::dump_pretty(j, 2);

  assert(!compact.empty());
  assert(!pretty.empty());
  assert(compact.find('\n') == std::string::npos);
  assert(pretty.find('\n') != std::string::npos);
}

static void test_get_or_and_require()
{
  const std::string s = R"({"name":"Ada","age":30,"active":true})";
  auto j = jh::parse_or_throw(s);

  const auto name = jh::get_or<std::string>(j, "name", "none");
  const auto age = jh::get_or<int>(j, "age", -1);
  const auto missing = jh::get_or<int>(j, "missing", 42);

  assert(name == "Ada");
  assert(age == 30);
  assert(missing == 42);

  assert(jh::require<std::string>(j, "name") == "Ada");
  assert(jh::require<int>(j, "age") == 30);

  bool threw_missing = false;
  try
  {
    (void)jh::require<int>(j, "missing");
  }
  catch (...)
  {
    threw_missing = true;
  }
  assert(threw_missing && "require must throw on missing key");

  bool threw_type = false;
  try
  {
    (void)jh::require<int>(j, "name");
  }
  catch (...)
  {
    threw_type = true;
  }
  assert(threw_type && "require must throw on invalid type");
}

static void test_get_string_or()
{
  auto j = jh::parse_or_throw(R"({"s":"ok","n":123})");
  assert(jh::get_string_or(j, "s", "no") == "ok");
  assert(jh::get_string_or(j, "missing", "def") == "def");
  assert(jh::get_string_or(j, "n", "def") == "def");
}

static void test_get_array_or_empty()
{
  auto j = jh::parse_or_throw(R"({"tags":["c++","vix",123,null,"ok"]})");

  auto tags = jh::get_array_or_empty<std::string>(j, "tags");
  assert(tags.size() == 3);
  assert(tags[0] == "c++");
  assert(tags[1] == "vix");
  assert(tags[2] == "ok");

  auto missing = jh::get_array_or_empty<std::string>(j, "missing");
  assert(missing.empty());
}

static void test_require_object_array()
{
  auto obj = jh::parse_or_throw(R"({"a":1})");
  auto arr = jh::parse_or_throw(R"([1,2,3])");

  (void)jh::require_object(obj);
  (void)jh::require_array(arr);

  bool threw = false;
  try
  {
    (void)jh::require_array(obj);
  }
  catch (...)
  {
    threw = true;
  }
  assert(threw && "require_array must throw if not array");
}

int main()
{
  try
  {
    test_parse_ok();
    test_parse_fail();
    test_parse_or_throw();
    test_dump();
    test_get_or_and_require();
    test_get_string_or();
    test_get_array_or_empty();
    test_require_object_array();

    std::cout << "[json_helpers] all tests passed\n";
    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "[json_helpers] test failed: " << e.what() << "\n";
    return 1;
  }
}
