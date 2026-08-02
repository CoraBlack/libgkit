#pragma once

#include "gkit/core/object_id.hpp"
#include "gkit/core/value.hpp"

#include <string>
#include <utility>

namespace gkit::core::reflect {

    /**
     * @brief Format-agnostic serializer base.
     *
     * Builds a private intermediate tree from reflection data, then renders the
     * tree through pure-virtual hooks. Derived classes implement a target format
     * (Json / Xml / Toml ...) by overriding the wrapper / gap / leaf hooks.
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

        /// @brief Begin/end delimiters around a node (keyed or unkeyed).
        virtual auto wrapper(Type type, const std::string& key) -> WrapperPair = 0;

        /// @brief Separator between sibling elements of a container.
        virtual auto element_gap(Type type) -> std::string = 0;

        /// @brief Encode a leaf value into the target format.
        virtual auto leaf_value(const Value& v) -> std::string = 0;
    };

} // namespace gkit::core::reflect
