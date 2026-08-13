#include "gkit/scene/node.hpp"

#include "core/object_pool.hpp"
#include "gkit/core/object_id.hpp"
#include "gkit/core/reflect/registry.hpp"
#include "gkit/core/unique_object.hpp"

#include <cstdint>
#include <format>
#include <mutex>
#include <stdexcept>
#include <utility>

namespace gkit::scene {
    /**
     * Node
     */
    static auto regist_holder = core::reflect::RegistHolder([]() -> void {
        Node::regist_method();
    });

    auto Node::regist_method() -> void {
        auto& db = core::reflect::ClassDB::instance();
        db.regist<Node>("Node").add_field("Node", "name", &Node::name);
        // "parent" is an ObjectId, which the reflection system cannot represent as a field
        // type yet (see the IsValueType concept in core/value.hpp); register it once supported.
    }

    Node::Node(Node&& other) noexcept {
        this->name           = std::move(other.name);
        this->parent         = other.parent; // Not sure, because relationship always changes when the node is moved
        this->children       = std::move(other.children);
        this->child_name_map = std::move(other.child_name_map);
        // Note: children's `parent` still point back to `other`; this cannot be fixed here
        // because `this` is not registered yet (get_id() returns an invalid id). Nodes are
        // pool-owned via UniqueObject and are not moved in practice.
    }

    [[nodiscard]] auto Node::operator=(Node&& other) noexcept -> Node& {
        if (this != &other) {
            this->name           = std::move(other.name);
            this->parent         = other.parent;
            this->children       = std::move(other.children);
            this->child_name_map = std::move(other.child_name_map);
        }
        return *this;
    }

    auto Node::get_parent() const noexcept -> core::ObjectId {
        return this->parent;
    }

    auto Node::get_child(uint32_t index) const noexcept -> core::ObjectId {
        auto none = core::ObjectId();
        std::lock_guard children_locker(this->children_mutex);
        if (index >= this->children.size()) {
            return none;
        }

        return this->children[index].get_id();
    }

    auto Node::get_child(const std::string& name) const noexcept -> core::ObjectId {
        auto none = core::ObjectId();
        std::lock_guard children_locker(this->children_mutex);
        auto find_it = this->child_name_map.find(name);
        if (find_it == this->child_name_map.end()) {
            return none;
        }

        return find_it->second;
    }

    auto Node::add_child(core::UniqueObject&& child) -> Node& {
        const auto& obj_id = child.get_id();
        auto* obj_ptr      = core::ObjectPool::instance().deref_from(obj_id);
        auto* node_ptr     = dynamic_cast<Node*>(obj_ptr);

        if (node_ptr == nullptr) {
            throw std::invalid_argument("child is not Node type");
        }
        node_ptr->parent = this->get_id();

        auto& child_name = node_ptr->name;

        std::lock_guard children_lock(this->children_mutex);
        auto it = this->child_name_map.find(child_name);
        if (it != this->child_name_map.end()) {
            // Name conflict, rename as <name#id64>
            const auto id = obj_id.get_id(), gen = obj_id.get_generation();
            uint64_t id64 = (static_cast<uint64_t>(id) << 32) | gen;
            child_name    = child_name + std::format("#{}", id64);
        }

        this->child_name_map[child_name] = obj_id;
        this->children.emplace_back(std::move(child));

        return *this;
    }

    auto Node::remove_child(uint32_t index) -> Node& {
        std::lock_guard children_lock(this->children_mutex);
        if (index >= this->children.size()) {
            throw std::invalid_argument("Index is out of range");
        }

        auto& dropped_child = this->children[index];
        auto* obj_ptr       = core::ObjectPool::instance().deref_from(dropped_child.get_id());
        auto* node_ptr      = dynamic_cast<Node*>(obj_ptr);
        if (node_ptr == nullptr) {
            // noreachable
            throw std::runtime_error("Error type cast");
        }
        node_ptr->parent = core::ObjectId();

        const auto& child_name = node_ptr->name;
        this->child_name_map.erase(child_name);
        this->children.erase(this->children.begin() + index);

        return *this;
    }

    auto Node::remove_child(const std::string& name) -> Node& {
        std::lock_guard children_lock(this->children_mutex);
        auto obj_it = this->child_name_map.find(name);
        if (obj_it == this->child_name_map.end()) {
            throw std::invalid_argument(std::format("Node named {} is not exist", name));
        }

        auto obj_id    = obj_it->second;
        auto* obj_ptr  = core::ObjectPool::instance().deref_from(obj_id);
        auto* node_ptr = dynamic_cast<Node*>(obj_ptr);
        if (node_ptr != nullptr) {
            node_ptr->parent = core::ObjectId();
        }
        for (auto it = this->children.begin(); it != this->children.end(); ++it) {
            if (it->get_id() == obj_id) {
                this->children.erase(it);
                this->child_name_map.erase(name);
                break;
            }
        }

        return *this;
    }

    /**
     * NodeIterator
     */
    template<bool IsConst>
    Node::NodeIterator<IsConst>::NodeIterator(pointer owner, size_t pos) : m_owner(owner), m_pos(pos) {
        this->refresh();
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::refresh() -> void {
        if (this->m_owner == nullptr) {
            this->m_current = nullptr;
            return;
        }
        auto child_id   = this->m_owner->get_child(static_cast<uint32_t>(this->m_pos));
        this->m_current = dynamic_cast<Node*>(core::ObjectPool::instance().deref_from(child_id));
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::operator*() const -> reference {
        return *this->m_current;
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::operator->() const -> pointer {
        return this->m_current;
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::operator++() -> NodeIterator& {
        ++this->m_pos;
        this->refresh();
        return *this;
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::operator++(int) -> NodeIterator {
        NodeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::operator--() -> NodeIterator& {
        --this->m_pos;
        this->refresh();
        return *this;
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::operator--(int) -> NodeIterator {
        NodeIterator tmp = *this;
        --(*this);
        return tmp;
    }

    template<bool IsConst>
    auto Node::NodeIterator<IsConst>::operator==(const NodeIterator& other) const -> bool {
        return this->m_owner == other.m_owner && this->m_pos == other.m_pos;
    }

    template class Node::NodeIterator<true>;
    template class Node::NodeIterator<false>;

    auto Node::begin() -> iterator {
        return iterator(this, 0);
    }
    auto Node::end() -> iterator {
        std::lock_guard children_lock(this->children_mutex);
        return iterator(this, this->children.size());
    }
    auto Node::begin() const -> const_iterator {
        return const_iterator(this, 0);
    }
    auto Node::end() const -> const_iterator {
        std::lock_guard children_lock(this->children_mutex);
        return const_iterator(this, this->children.size());
    }
    auto Node::cbegin() const -> const_iterator {
        return begin();
    }
    auto Node::cend() const -> const_iterator {
        return end();
    }

    auto Node::rbegin() -> reverse_iterator {
        return reverse_iterator(end());
    }
    auto Node::rend() -> reverse_iterator {
        return reverse_iterator(begin());
    }
    auto Node::rbegin() const -> const_reverse_iterator {
        return const_reverse_iterator(end());
    }
    auto Node::rend() const -> const_reverse_iterator {
        return const_reverse_iterator(begin());
    }
    auto Node::crbegin() const -> const_reverse_iterator {
        return rbegin();
    }
    auto Node::crend() const -> const_reverse_iterator {
        return rend();
    }

} // namespace gkit::scene