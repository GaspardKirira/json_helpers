# json_helpers

Lightweight header-only C++ utilities for nlohmann::json.

Designed for modern C++ applications that need:

-   Safe typed getters
-   Strict field validation
-   Clean error messages
-   Minimal boilerplate when working with JSON

C++17 compatible.

## Installation

### Using Vix Registry

``` bash
vix add gaspardkirira/json_helpers
```

### Manual download

Download the latest release:

👉 https://github.com/GaspardKirira/json_helpers/releases

Or clone:

``` bash
git clone https://github.com/GaspardKirira/json_helpers.git
```

Add the `include/` directory to your project.

## Dependencies

This library requires:

-   C++17 or later
-   nlohmann/json

The header must be available as:

``` cpp
#include <nlohmann/json.hpp>
```

## Installing nlohmann/json

### Ubuntu

``` bash
sudo apt install nlohmann-json3-dev
```

### vcpkg

``` bash
vcpkg install nlohmann-json
```

### Single header

Download from:

https://github.com/nlohmann/json/releases

Place it in your include path:
```bash
    include/nlohmann/json.hpp
```
## Quick Example

``` cpp
#include <json_helpers/json_helpers.hpp>
#include <iostream>

int main()
{
  namespace jh = json_helpers;

  auto j = jh::parse_or_throw(R"({
    "name": "Gaspard",
    "age": 30,
    "tags": ["c++", "json"]
  })");

  std::string name = jh::get_or<std::string>(j, "name", "unknown");
  int age = jh::require<int>(j, "age");

  auto tags = jh::get_array_or_empty<std::string>(j, "tags");

  std::cout << "Name: " << name << "\n";
  std::cout << "Age: " << age << "\n";

  for (const auto& t : tags)
    std::cout << "Tag: " << t << "\n";
}
```

## Features

-   Header-only
-   Minimal wrapper over `nlohmann::json`
-   Safe parsing (`parse`)
-   Strict parsing (`parse_or_throw`)
-   Typed getters with defaults (`get_or`)
-   Strict required fields (`require`)
-   Safe array extraction
-   Object and array validation
-   Compact and pretty JSON dump
-   Clear error messages

## Safe Parsing

``` cpp
auto parsed = json_helpers::parse(input);
if (!parsed)
{
  // invalid JSON
}
```

Strict version:

``` cpp
auto j = json_helpers::parse_or_throw(input);
```

## Typed Getters

``` cpp
int age = json_helpers::get_or<int>(j, "age", 0);
std::string name = json_helpers::get_or<std::string>(j, "name", "unknown");
```

## Strict Required Fields

``` cpp
int id = json_helpers::require<int>(j, "id");
```

Throws if:

-   Key is missing
-   Value is null
-   Type mismatch

## Array Helpers

``` cpp
auto tags = json_helpers::get_array_or_empty<std::string>(j, "tags");
```

Invalid elements are skipped automatically.

## Dump Helpers

``` cpp
std::string compact = json_helpers::dump_compact(j);
std::string pretty  = json_helpers::dump_pretty(j, 2);
```

## Tests

Run tests with:

``` bash
vix tests
```

## Examples

See the `examples/` directory:

-   parse_and_dump.cpp
-   typed_getters.cpp
-   strict_require.cpp

Run:

``` bash
vix run examples/typed_getters.cpp
```

## Why use json_helpers?

-   Removes repetitive JSON boilerplate
-   Makes validation explicit
-   Clear separation between safe and strict access

Ideal for:

-   API parsing
-   Configuration loaders
-   CLI tools
-   Backend services
-   Structured data validation

## License

MIT License.

