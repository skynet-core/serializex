#ifndef SERIALIZEX_HPP_
#define SERIALIZEX_HPP_

#include <array>
#include <boost/json.hpp>
#include <boost/pfr.hpp>
#include <boost/pfr/core_name.hpp>
#include <cmath>
#include <cstddef>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace SerializeX
{
  namespace Details
  {

    template <std::size_t N>
    struct ConstStr
    {
      std::array<char, N> value{0};

      constexpr ConstStr(const char (&str)[N])
      {
        for (std::size_t i = 0; i < N; ++i)
        {
          value[i] = str[i];
        }
      }

      constexpr ConstStr(std::string_view str)
      {
        for (std::size_t i = 0; i < N; ++i)
        {
          value[i] = str[i];
        }
      }

      constexpr ConstStr(std::array<char, N> data) : value(data) {}

      constexpr operator std::string_view() const
      {
        return std::string_view(value.data(), N);
      }

      constexpr operator std::string() const
      {
        return std::string(value.data(), N);
      }
    };

    template <std::size_t N>
    constexpr std::ostream &operator<<(std::ostream &os, const ConstStr<N> &str)
    {
      os << std::string_view(str);
      return os;
    }
  } // namespace Details

  namespace Json
  {
    using namespace SerializeX::Details;

    template <std::size_t I, ConstStr N, typename T>
    struct Reflection
    {
      static constexpr std::size_t Index = I;
      static constexpr std::string_view Name = N;
      using Type = T;
      constexpr Reflection(T &value) : Value(value) {}
      T &Value;
    };

    template <std::size_t I, ConstStr N, typename T>
    struct ConstReflection
    {
      static constexpr std::size_t Index = I;
      static constexpr std::string_view Name = N;
      using Type = T;
      constexpr ConstReflection(const T &value) : Value(value) {}
      const T &Value;
    };

    template <std::size_t I, typename T>
    constexpr static auto MakeReflection(T &obj)
    {
      constexpr static auto name = boost::pfr::get_name<I, T>();
      constexpr static ConstStr<name.size()> nameStr{name};
      auto &value = boost::pfr::get<I>(obj);
      auto reflection =
          Reflection<I, nameStr, std::decay_t<decltype(value)>>{value};
      return reflection;
    }

    template <std::size_t I, typename T>
    constexpr static auto MakeReflection(const T &obj)
    {
      constexpr static auto name = boost::pfr::get_name<I, T>();
      constexpr static ConstStr<name.size()> nameStr{name};
      const auto &value = boost::pfr::get<I>(obj);
      auto reflection =
          ConstReflection<I, nameStr, std::decay_t<decltype(value)>>{value};
      return reflection;
    }

    namespace Details
    {
      template <typename T>
      struct FieldVisitor
      {
        template <std::size_t... Is>
        static constexpr void Apply(T &obj, auto func, std::index_sequence<Is...>)
        {
          (func(MakeReflection<Is>(obj)), ...);
        }

        template <std::size_t... Is>
        static constexpr void Apply(const T &obj, auto func,
                                    std::index_sequence<Is...>)
        {
          (func(MakeReflection<Is>(obj)), ...);
        }
      };
    } // namespace Details

    template <typename T, typename Idx = std::make_index_sequence<
                              boost::pfr::tuple_size<T>::value>>
    constexpr void ForEachField(T &obj, auto func)
    {
      Details::FieldVisitor<T>::Apply(obj, func, Idx{});
    }

    template <typename T, typename Idx = std::make_index_sequence<
                              boost::pfr::tuple_size<T>::value>>
    constexpr void ForEachField(const T &obj, auto func)
    {
      Details::FieldVisitor<T>::Apply(obj, func, Idx{});
    }

    template <typename T>
    std::enable_if_t<std::is_trivial_v<T>, std::ostream &> Write(std::ostream &out,
                                                                 const T &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_same_v<std::string_view, T>, std::ostream &>
    Write(std::ostream &out, const T &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_convertible_v<T, std::string>, std::ostream &>
    Write(std::ostream &out, const T &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    struct Serializer;

    template <typename T>
    std::enable_if_t<std::is_aggregate_v<T>, std::ostream &>
    Write(std::ostream &out, const T &obj)
    {
      out << std::string(Serializer{obj});
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_trivial_v<T>, std::ostream &>
    Write(std::ostream &out, const std::optional<T> &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_trivial_v<T>, std::ostream &>
    Write(std::ostream &out, const std::unique_ptr<T> &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_trivial_v<T>, std::ostream &>
    Write(std::ostream &out, const std::shared_ptr<T> &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_convertible_v<T, std::string>, std::ostream &>
    Write(std::ostream &out, const std::optional<T> &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_convertible_v<T, std::string>, std::ostream &>
    Write(std::ostream &out, const std::unique_ptr<T> &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_convertible_v<T, std::string>, std::ostream &>
    Write(std::ostream &out, const std::shared_ptr<T> &obj)
    {
      out << boost::json::serialize(obj);
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_aggregate_v<T>, std::ostream &>
    Write(std::ostream &out, const std::optional<T> &obj)
    {
      if (obj)
      {
        out << std::string(Serializer{*obj});
      }
      else
      {
        out << "null";
      }
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_aggregate_v<T>, std::ostream &>
    Write(std::ostream &out, const std::unique_ptr<T> &obj)
    {
      if (obj)
      {
        out << std::string(Serializer{*obj});
      }
      else
      {
        out << "null";
      }
      return out;
    }

    template <typename T>
    std::enable_if_t<std::is_aggregate_v<T>, std::ostream &>
    Write(std::ostream &out, const std::shared_ptr<T> &obj)
    {
      if (obj)
      {
        out << std::string(Serializer{*obj});
      }
      else
      {
        out << "null";
      }
      return out;
    }

    template <typename T>
    struct Serializer
    {
      Serializer(const T &obj) : Obj(obj) {}
      const T &Obj;

      operator std::string() const
      {
        std::stringstream ss;
        ss << "{";
        ForEachField(Obj, [&](const auto &field)
                     {
      if constexpr (std::decay_t<decltype(field)>::Index > 0) {
        ss << ",";
      }
      Write(ss, std::decay_t<decltype(field)>::Name);
      ss << ":";
      Write(ss, field.Value); });
        ss << "}";
        return ss.str();
      }
    };

  } // namespace Json
} // namespace SerializeX

#endif
