#include <gkit/core/value.hpp>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <map>

using gkit::core::Value;
using gkit::core::Null;

auto test_value_construction() -> void {
    std::cout << "=== Test: Value Construction ===" << std::endl;

    Value v_null;
    assert(v_null.is<Null>());
    std::cout << "  Null: OK" << std::endl;

    Value v_bool(true);
    assert(v_bool.is<bool>());
    assert(v_bool.get<bool>().value() == true);
    std::cout << "  Bool: OK" << std::endl;

    Value v_i64(static_cast<std::int64_t>(42));
    assert(v_i64.is<std::int64_t>());
    assert(v_i64.get<std::int64_t>().value() == 42);
    std::cout << "  int64_t: OK" << std::endl;

    Value v_u64(static_cast<std::uint64_t>(100));
    assert(v_u64.is<std::uint64_t>());
    assert(v_u64.get<std::uint64_t>().value() == 100);
    std::cout << "  uint64_t: OK" << std::endl;

    Value v_f32(3.14f);
    assert(v_f32.is<float>());
    std::cout << "  float: OK" << std::endl;

    Value v_f64(2.718);
    assert(v_f64.is<double>());
    std::cout << "  double: OK" << std::endl;

    Value v_str("hello");
    assert(v_str.is<std::string>());
    assert(v_str.get<std::string>().value() == "hello");
    std::cout << "  string: OK" << std::endl;

    Value v_arr(std::vector<Value>{});
    assert(v_arr.is<std::vector<Value>>());
    std::cout << "  array: OK" << std::endl;

    using MapType = std::map<std::string, Value>;
    Value v_map(MapType{});
    assert(v_map.is<MapType>());
    std::cout << "  map: OK" << std::endl;

    std::cout << "All construction tests passed!" << std::endl << std::endl;
}

auto test_value_type_mismatch() -> void {
    std::cout << "=== Test: Value Type Mismatch ===" << std::endl;

    Value v(42);

    assert(!v.is<bool>());
    assert(!v.is<std::string>());

    assert(v.get<bool>() == std::nullopt);
    assert(v.get<std::string>() == std::nullopt);

    std::cout << "  Type mismatch returns nullopt: OK" << std::endl;
    std::cout << "All type mismatch tests passed!" << std::endl << std::endl;
}

auto test_value_nested() -> void {
    std::cout << "=== Test: Value Nested Types ===" << std::endl;

    using MapType = std::map<std::string, Value>;
    Value obj(MapType{});
    obj["name"] = Value("test");
    obj["count"] = Value(static_cast<std::int64_t>(10));
    obj["active"] = Value(true);

    assert(obj.is<MapType>());
    assert(obj["name"].is<std::string>());
    assert(obj["name"].get<std::string>().value() == "test");
    assert(obj["count"].is<std::int64_t>());
    assert(obj["count"].get<std::int64_t>().value() == 10);
    assert(obj["active"].is<bool>());
    assert(obj["active"].get<bool>().value() == true);

    std::cout << "  Nested object: OK" << std::endl;

    using ArrayType = std::vector<Value>;
    Value arr(ArrayType{});
    arr.emplace_back(Value(static_cast<std::int64_t>(1)));
    arr.emplace_back(Value(static_cast<std::int64_t>(2)));
    arr.emplace_back(Value("three"));

    assert(arr.is<ArrayType>());
    assert(arr[0].get<std::int64_t>().value() == 1);
    assert(arr[2].get<std::string>().value() == "three");

    std::cout << "  Nested array: OK" << std::endl;
    std::cout << "All nested tests passed!" << std::endl << std::endl;
}

auto main() -> int {
    std::cout << "========================================" << std::endl;
    std::cout << "       gkit::core::Value Tests       " << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    test_value_construction();
    test_value_type_mismatch();
    test_value_nested();

    std::cout << "========================================" << std::endl;
    std::cout << "         ALL TESTS PASSED!             " << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}