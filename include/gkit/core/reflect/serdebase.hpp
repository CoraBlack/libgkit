#pragma once

#include "core/reflect/seralize.hpp"
#include "gkit/core/object_id.hpp"
#include "gkit/core/value.hpp"

#include <string>
#include <utility>

namespace gkit::core::reflect {

    /**
     * @brief Format-agnostic serializer base.
     *
     * Builds a @ref SerdeStruct tree from reflection data, then renders the tree
     * through pure-virtual hooks. Derived classes implement a target format
     * (Json / Xml / Toml ...) by overriding the wrapper / gap hooks.
     */
    class SerdeBase {
    public:
        using WrapperPair = std::pair<std::string, std::string>;

        virtual ~SerdeBase() = default;

        /**
         * @brief Serialize an object (by id) into the target format.
         * @throws std::invalid_argument if the id is not available.
         * @return Empty string when the object tree cannot be built.
         */
        [[nodiscard]] auto seralize(const ObjectId& id) -> std::string;

        /**
         * @brief Serialize a single value node (with optional key).
         */
        [[nodiscard]] auto seralize(const std::string& key, const Value& v) -> std::string;

        // =====================================================================
        // Format hooks (pure virtual)
        // =====================================================================

        /// @brief Delimiters placed around a key.
        virtual auto key_wrapper(const std::string& name) -> WrapperPair = 0;

        /// @brief Separator between a key and its value.
        virtual auto kv_connect() -> std::string = 0;

        /// @brief Begin/end delimiters of a container value.
        virtual auto array_wrapper(const std::string& name) -> WrapperPair  = 0;
        virtual auto map_wrapper(const std::string& name) -> WrapperPair    = 0;
        virtual auto object_wrapper(const std::string& name) -> WrapperPair = 0;

        /// @brief Separator between elements of a container.
        virtual auto array_ele_gap() -> std::string  = 0;
        virtual auto map_ele_gap() -> std::string    = 0;
        virtual auto object_ele_gap() -> std::string = 0;

        /// @brief Encode a leaf value (raw string + type) into the target format.
        virtual auto value_wrapper(const std::string& raw_val, Type type) -> std::string = 0;

    protected:
        /// @brief Render a single tree node (format-agnostic traversal).
        [[nodiscard]] auto render(const SerdeNode& node) -> std::string;

        [[nodiscard]] auto container_wrapper(Type type, const std::string& name) -> WrapperPair;
        [[nodiscard]] auto ele_gap(Type type) -> std::string;
    };

} // namespace gkit::core::reflect
