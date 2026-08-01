#pragma once

#include "gkit/core/object_id.hpp"
#include "gkit/core/value.hpp"

#include <memory>


namespace gkit::core::reflect {
    class SerdeNode;

    class SerdeStruct final {
        bool available_flag = false;
        std::unique_ptr<SerdeStruct> serde_root{};

    public:
        explicit SerdeStruct(const ObjectId v) noexcept;
        ~SerdeStruct() = default;

        auto to_string() const -> std::string;
        /* static auto from_string(const std::string& str) -> SerdeStruct; */

        inline auto available() -> bool { return this->available_flag; }
    }; // class SerdeStruct

    class SerdeNode {
        std::string key;
        std::string val;

    public:
    }; // class SerdeNode
} // namespace gkit::core::reflect