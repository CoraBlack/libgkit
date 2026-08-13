#pragma once

#include "gkit/core/object.hpp"
#include "gkit/core/object_id.hpp"
#include "gkit/core/unique_object.hpp"

#include <concepts>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace gkit::scene {
    class Node;

    template<class T>
    concept IsNode = std::derived_from<T, Node>;

    class Node : public core::Object {
        std::string name = "";
        core::ObjectId parent{}; // Must be node type
        mutable std::mutex children_mutex{};
        std::vector<core::UniqueObject> children; // Must be node type
        // It will be messy after name of child node changes
        std::unordered_map<std::string, core::ObjectId> child_name_map;

    public:
        static auto regist_method() -> void;

    public:
        Node() noexcept = default;
        explicit Node(const std::string& name) : name(name) {};
        Node(const Node&)           = delete;
        auto operator=(const Node&) = delete;
        Node(Node&&) noexcept;
        [[nodiscard]] auto operator=(Node&&) noexcept -> Node&;

        // prue virtual method and flow control
        virtual auto ready() -> void                        = 0;
        virtual auto process(double delta) -> void          = 0;
        virtual auto physical_process(double delta) -> void = 0;
        virtual auto exit() -> void                         = 0;

        [[nodiscard]] auto get_parent() const noexcept -> core::ObjectId;
        [[nodiscard]] auto get_child(uint32_t index) const noexcept -> core::ObjectId;
        [[nodiscard]] auto get_child(const std::string& name) const noexcept -> core::ObjectId;

        auto add_child(core::UniqueObject&& child) -> Node&;
        auto remove_child(uint32_t index) -> Node&;
        auto remove_child(const std::string& name) -> Node&;

        inline auto get_name() const noexcept -> const std::string& { return this->name; }

    public: // Iterator
        template<bool IsConst>
        class NodeIterator {
        public:
            // NOLINTBEGIN(readability-identifier-naming)
            using value_type      = Node;
            using difference_type = std::ptrdiff_t;
            using pointer         = std::conditional_t<IsConst, const Node*, Node*>;
            using reference       = std::conditional_t<IsConst, const Node&, Node&>;
            // NOLINTEND(readability-identifier-naming)

        private:
            const Node* m_owner;
            size_t m_pos;

        public:
            NodeIterator() = default;
            NodeIterator(const Node* owner, size_t pos);
            auto operator*() const -> reference;
            auto operator->() const -> pointer;
            auto operator++() -> NodeIterator&;
            auto operator++(int) -> NodeIterator;
            auto operator--() -> NodeIterator&;
            auto operator--(int) -> NodeIterator;
            auto operator==(const NodeIterator&) const -> bool;
            friend class Node;
        }; // class NodeIterator<bool IsConst>

        using iterator       = NodeIterator<false>; // NOLINT(readability-identifier-naming)
        using const_iterator = NodeIterator<true>; // NOLINT(readability-identifier-naming)

        auto begin() -> iterator;
        auto end() -> iterator;

        auto begin() const -> const_iterator;
        auto end() const -> const_iterator;

        auto cbegin() const -> const_iterator;
        auto cend() const -> const_iterator;

    public:
        using reverse_iterator       = std::reverse_iterator<iterator>; // NOLINT(readability-identifier-naming)
        using const_reverse_iterator = std::reverse_iterator<const_iterator>; // NOLINT(readability-identifier-naming)

        auto rbegin() -> reverse_iterator;
        auto rend() -> reverse_iterator;

        auto rbegin() const -> const_reverse_iterator;
        auto rend() const -> const_reverse_iterator;

        auto crbegin() const -> const_reverse_iterator;
        auto crend() const -> const_reverse_iterator;
    };
} // namespace gkit::scene