#pragma once

/**
 * @file json_helpers.hpp
 * @brief Header-only helpers for nlohmann::json.
 *
 * This library provides small, predictable utilities:
 *  - safe typed getters with defaults
 *  - strict getters with clear error messages
 *  - safe parsing helpers (no throw version)
 *  - object/array helpers
 *
 * Requirements:
 *  - C++17
 *  - nlohmann::json available as <nlohmann/json.hpp>
 */

#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <cstdint>

namespace json_helpers
{
  using json = nlohmann::json;

  /**
   * @brief Parse JSON string safely (no throw).
   *
   * @param s Input string
   * @return Parsed json on success, std::nullopt on parse error
   */
  inline std::optional<json> parse(const std::string &s) noexcept
  {
    try
    {
      return json::parse(s);
    }
    catch (...)
    {
      return std::nullopt;
    }
  }

  /**
   * @brief Parse JSON string strictly.
   *
   * @throws std::runtime_error on parse error
   */
  inline json parse_or_throw(const std::string &s)
  {
    try
    {
      return json::parse(s);
    }
    catch (const std::exception &e)
    {
      throw std::runtime_error(std::string("json_helpers::parse_or_throw: ") + e.what());
    }
  }

  /**
   * @brief Convert json to compact string.
   */
  inline std::string dump_compact(const json &j)
  {
    return j.dump();
  }

  /**
   * @brief Convert json to pretty string.
   *
   * @param indent Indentation spaces (default 2).
   */
  inline std::string dump_pretty(const json &j, int indent = 2)
  {
    return j.dump(indent);
  }

  /**
   * @brief Check if key exists and is not null.
   */
  inline bool has(const json &obj, const std::string &key) noexcept
  {
    if (!obj.is_object())
      return false;
    auto it = obj.find(key);
    return it != obj.end() && !it->is_null();
  }

  /**
   * @brief Get value if present and convertible, else return default.
   *
   * @tparam T requested type
   * @param obj JSON object
   * @param key field name
   * @param def default value when missing or invalid
   */
  template <class T>
  inline T get_or(const json &obj, const std::string &key, T def) noexcept
  {
    try
    {
      if (!obj.is_object())
        return def;
      auto it = obj.find(key);
      if (it == obj.end() || it->is_null())
        return def;
      return it->get<T>();
    }
    catch (...)
    {
      return def;
    }
  }

  /**
   * @brief Strict getter: requires object[key] to exist and be convertible.
   *
   * @throws std::runtime_error if missing or invalid type
   */
  template <class T>
  inline T require(const json &obj, const std::string &key)
  {
    if (!obj.is_object())
      throw std::runtime_error("json_helpers::require: input is not an object");

    auto it = obj.find(key);
    if (it == obj.end())
      throw std::runtime_error("json_helpers::require: missing key: " + key);

    if (it->is_null())
      throw std::runtime_error("json_helpers::require: null key: " + key);

    try
    {
      return it->get<T>();
    }
    catch (const std::exception &e)
    {
      throw std::runtime_error("json_helpers::require: invalid type for key '" + key + "': " + e.what());
    }
  }

  /**
   * @brief Get string with default (handles non-string by dump when wanted).
   */
  inline std::string get_string_or(const json &obj, const std::string &key, const std::string &def = "") noexcept
  {
    try
    {
      if (!obj.is_object())
        return def;
      auto it = obj.find(key);
      if (it == obj.end() || it->is_null())
        return def;

      if (it->is_string())
        return it->get<std::string>();

      return def;
    }
    catch (...)
    {
      return def;
    }
  }

  /**
   * @brief Get array elements as vector<T>, or empty vector if missing/invalid.
   */
  template <class T>
  inline std::vector<T> get_array_or_empty(const json &obj, const std::string &key) noexcept
  {
    try
    {
      if (!obj.is_object())
        return {};
      auto it = obj.find(key);
      if (it == obj.end() || !it->is_array())
        return {};

      std::vector<T> out;
      out.reserve(it->size());

      for (const auto &el : *it)
      {
        try
        {
          out.push_back(el.get<T>());
        }
        catch (...)
        {
          // skip invalid element
        }
      }

      return out;
    }
    catch (...)
    {
      return {};
    }
  }

  /**
   * @brief Ensure value is an object and return reference.
   *
   * @throws std::runtime_error if not an object.
   */
  inline const json &require_object(const json &j)
  {
    if (!j.is_object())
      throw std::runtime_error("json_helpers::require_object: expected object");
    return j;
  }

  /**
   * @brief Ensure value is an array and return reference.
   *
   * @throws std::runtime_error if not an array.
   */
  inline const json &require_array(const json &j)
  {
    if (!j.is_array())
      throw std::runtime_error("json_helpers::require_array: expected array");
    return j;
  }

} // namespace json_helpers
