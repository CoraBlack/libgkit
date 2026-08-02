#include "core/reflect/seralize.hpp"
#include "gkit/core/object.hpp"
#include "gkit/core/reflect/registry.hpp"
#include "gkit/core/unique_object.hpp"
#include <cstdint>
#include <iostream>
#include <string>

using gkit::core::UniqueObject;

class SeralizeObject : public gkit::core::Object {
    int64_t num = 10;
    float f_num = 20.f;
    std::string str = "Hello my girl";

public:
    static const char* c_name;
    static auto regist_method() -> void {
        auto& db = gkit::core::reflect::ClassDB::instance();
        db.regist<SeralizeObject>(c_name)
            .add_field(c_name, "num", &SeralizeObject::num)
            .add_field(c_name, "f_num", &SeralizeObject::f_num)
            .add_field(c_name, "str", &SeralizeObject::str);
    }
};

const char* SeralizeObject::c_name  = "SeralizeObject";

auto init() -> void {
    SeralizeObject::regist_method();
}

auto main() -> int {
    init();
    auto obj_opt = UniqueObject::create_with_classname(SeralizeObject::c_name);
    if (!obj_opt.has_value()) return -1;
    auto serde = gkit::core::reflect::SerdeStruct(obj_opt.value().get_id());
    std::cout << serde.to_string() << '\n';
    return 0;
}