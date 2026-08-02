#include "gkit/core/unique_object.hpp"
#include "gkit/core/value.hpp"

#include <iostream>

#include <gkit/core/reflect/registry.hpp>
#include <gkit/core/reflect/serdebase.hpp>

using gkit::core::Value;
using gkit::core::reflect::SerdeBase;

class SeralizeObject : public gkit::core::Object {
    int64_t num           = 10;
    float f_num           = 20.f;
    std::string str       = "Hello my girl";
    gkit::core::Array arr = {Value(100), Value(200.f), Value("300")};

    gkit::core::Map map = {{"map_num", Value(100)}, {"map_fnum", Value(200.f)}, {"map_str", Value("")}};

public:
    static const char* c_name;
    static auto regist_method() -> void {
        auto& db = gkit::core::reflect::ClassDB::instance();
        db.regist<SeralizeObject>(c_name)
            .add_field(c_name, "num", &SeralizeObject::num)
            .add_field(c_name, "f_num", &SeralizeObject::f_num)
            .add_field(c_name, "str", &SeralizeObject::str)
            .add_field(c_name, "arr", &SeralizeObject::arr)
            .add_field(c_name, "map", &SeralizeObject::map);
    }
};

const char* SeralizeObject::c_name = "SeralizeObject";

class Json : public SerdeBase {
    auto kv_connect() -> std::string override { return ":"; }

    auto key_wrapper(const std::string& name) -> WrapperPair override { return {"\"", "\""}; }

    auto array_wrapper(const std::string& name) -> WrapperPair override { return {"[", "]"}; }

    auto map_wrapper(const std::string& name) -> WrapperPair override { return {"{", "}"}; }

    auto object_wrapper(const std::string& name) -> WrapperPair override { return {"{", "}"}; }

    auto array_ele_gap() -> std::string override { return ","; }

    auto map_ele_gap() -> std::string override { return ","; }

    auto object_ele_gap() -> std::string override { return ","; }

    auto value_wrapper(const std::string& raw_val, gkit::core::Type type) -> std::string override {
        switch (type) {
        case gkit::core::Type::String: {
            return "\"" + raw_val + "\"";
        }
        case gkit::core::Type::Null: {
            return "null";
        }
        default: {
            return raw_val;
        }
        }
    }
};

auto main() -> int {
    SeralizeObject::regist_method();
    auto json      = Json();
    auto obj       = gkit::core::UniqueObject::create<SeralizeObject>();
    auto serde_str = json.seralize(obj.get_id());
    std::cout << serde_str << '\n';

    const auto expect =
        R"({"num":10,"f_num":20.000000,"str":"Hello my girl","arr":[100,200.000000,"300"],"map":{"map_fnum":200.000000,"map_num":100,"map_str":""}})";
    if (serde_str != expect) {
        std::cerr << "unexpected output:\n  got:      " << serde_str << '\n' << "  expected: " << expect << '\n';
        return 1;
    }

    return 0;
}