#include "gkit/core/scene/unit.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>


gkit::core::scene::Unit::Unit() noexcept : 
    children(std::vector<std::unique_ptr<Unit>>()),
    children_rw_mutex() { }

    
gkit::core::scene::Unit::Unit(std::string name) noexcept : gkit::core::scene::Unit() {
    this->name = name;
}


auto gkit::core::scene::Unit::ready()            -> void { }
auto gkit::core::scene::Unit::process()          -> void { }
auto gkit::core::scene::Unit::physics_process()  -> void { }
auto gkit::core::scene::Unit::exit()             -> void { }


auto gkit::core::scene::Unit::ready_handler() noexcept -> void {
    std::shared_lock<std::shared_mutex> r_lock(this->children_rw_mutex);
    for (auto& child_ptr : this->children) {
        if (child_ptr == nullptr) continue;
        child_ptr->ready_handler();
    }
    this->ready();
}


auto gkit::core::scene::Unit::process_handler() noexcept -> void {
    std::shared_lock<std::shared_mutex> r_lock(this->children_rw_mutex);
    for (auto& child_ptr : this->children) {
        if (child_ptr == nullptr || !child_ptr->process_enabled) continue;
        child_ptr->process_handler();
    }
    this->process();
    this->drop_children();
}


auto gkit::core::scene::Unit::physics_process_handler() noexcept -> void {

}


auto gkit::core::scene::Unit::exit_handler() noexcept -> void {
    std::shared_lock<std::shared_mutex> r_lock(this->children_rw_mutex);
    for (auto& child_ptr : this->children) {
        if (child_ptr == nullptr) continue;
        child_ptr->exit_handler();
    }
    this->exit();
}


auto gkit::core::scene::Unit::add_child(std::unique_ptr<Unit>&& child_ptr) -> void {
    if (child_ptr == nullptr) {
        throw std::invalid_argument("child_ptr is nullptr");
    }

    if (child_ptr->name.empty()) {
        throw std::invalid_argument("child_ptr name is empty");
    }

    child_ptr->ready();
    {
        std::unique_lock<std::shared_mutex> w_lock(this->children_rw_mutex);
        child_ptr->parent = this;
        children.push_back(std::move(child_ptr));
    }
    {
        std::shared_lock<std::shared_mutex> r_lock(this->children_rw_mutex);
        std::unique_lock<std::shared_mutex> w_lock(this->name_map_cache_rw_mutex);
        auto& child_name = this->children.back()->name;
        auto* new_child_ptr = this->children.back().get();
        if (this->name_map_cache.contains(child_name)) {
            throw std::invalid_argument("child_ptr name is already exist");
        }
        this->name_map_cache.emplace(std::make_pair(child_name, new_child_ptr));
    }
}


auto gkit::core::scene::Unit::remove_child(uint32_t index) noexcept -> void {
    auto child_ptr = this->get_available_child(index);
    if (child_ptr == nullptr) return;
    child_ptr->ready_to_drop();
}


auto gkit::core::scene::Unit::remove_child(const std::string& child_name) noexcept -> void {
    auto child_ptr = this->get_child(child_name);
    if (child_ptr == nullptr) return;
    child_ptr->ready_to_drop();
}


auto gkit::core::scene::Unit::get_available_child(uint32_t index) noexcept -> Unit* {
    std::shared_lock<std::shared_mutex> r_lock(this->children_rw_mutex);

    if (index >= this->children.size()) {
        return nullptr;
    }
    return this->children[index].get();
}


auto gkit::core::scene::Unit::get_child(const std::string& child_name) noexcept -> Unit* {
    std::shared_lock<std::shared_mutex> r_lock(this->name_map_cache_rw_mutex);
    auto iter = this->name_map_cache.find(child_name);
    if (iter == this->name_map_cache.end()) {
        return nullptr;
    }
    return iter->second;
}


auto gkit::core::scene::Unit::drop_children() -> void {
    std::vector<std::unique_ptr<Unit>> to_exit;
    to_exit.reserve(this->children.size() / 2);

    std::erase_if(this->children, [&](std::unique_ptr<Unit>& p) -> bool {
        if (p && p->drop_flag.load() == true) {
            {
                std::unique_lock<std::shared_mutex> w_lock(this->name_map_cache_rw_mutex);
                this->name_map_cache.erase(p->name);
            }
            to_exit.push_back(std::move(p));
            return true;
        }
        return false;
    });

    for (auto& child : to_exit) {
        child->exit_handler();
    }
}


template<>
auto gkit::core::scene::Unit::get_parent<gkit::core::scene::Unit>() noexcept -> std::optional<std::reference_wrapper<Unit>> {
    if (parent == nullptr) return std::nullopt;
    return std::ref(*parent);
}


// iterator part use
gkit::core::scene::Unit::iterator::iterator(Unit* owner, size_t pos) : m_owner(owner), m_pos(pos) {}
auto gkit::core::scene::Unit::iterator::operator*() const -> reference {
auto child_ptr = m_owner->get_available_child(static_cast<uint32_t>(m_pos));
    return *child_ptr;
}
auto gkit::core::scene::Unit::iterator::operator->() const -> pointer {
    auto child_ptr = m_owner->get_available_child(static_cast<uint32_t>(m_pos));
    return child_ptr;
}
auto gkit::core::scene::Unit::iterator::operator++() -> iterator& {
    ++m_pos;
    return *this;
}
auto gkit::core::scene::Unit::iterator::operator++(int) -> iterator {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}
auto gkit::core::scene::Unit::iterator::operator--() -> iterator& {
    --m_pos;
    return *this;
}
auto gkit::core::scene::Unit::iterator::operator--(int) -> iterator {
    iterator tmp = *this;
    --(*this);
    return tmp;
}
auto gkit::core::scene::Unit::iterator::operator==(const iterator& other) const -> bool  { return m_owner == other.m_owner && m_pos == other.m_pos; }
auto gkit::core::scene::Unit::iterator::operator!=(const iterator& other) const -> bool  { return !(*this == other); }

auto gkit::core::scene::Unit::begin() -> iterator {
    return iterator(this, 0);
}

auto gkit::core::scene::Unit::end() -> iterator {
    return iterator(this, children.size());
}

// now is const_iterator use
gkit::core::scene::Unit::const_iterator::const_iterator(const Unit* owner, size_t pos) : m_owner(owner), m_pos(pos) {}

auto gkit::core::scene::Unit::const_iterator::operator*() const -> reference {
    auto child_ptr = const_cast<Unit*>(m_owner)->get_available_child(static_cast<uint32_t>(m_pos));
    return *child_ptr;
}

auto gkit::core::scene::Unit::const_iterator::operator->() const -> pointer {
    auto child_ptr = const_cast<Unit*>(m_owner)->get_available_child(static_cast<uint32_t>(m_pos));
    return child_ptr;
}

auto gkit::core::scene::Unit::const_iterator::operator++() -> const_iterator& {
    ++m_pos;
    return *this;
}
auto gkit::core::scene::Unit::const_iterator::operator++(int) -> const_iterator {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
}
auto gkit::core::scene::Unit::const_iterator::operator--() -> const_iterator& {
    --m_pos;
    return *this;
}
auto gkit::core::scene::Unit::const_iterator::operator--(int) -> const_iterator {
    const_iterator tmp = *this;
    --(*this);
    return tmp;
}

auto gkit::core::scene::Unit::const_iterator::operator==(const const_iterator& other) const -> bool { return m_owner == other.m_owner && m_pos == other.m_pos; }
auto gkit::core::scene::Unit::const_iterator::operator!=(const const_iterator& other) const -> bool { return !(*this == other); }

auto gkit::core::scene::Unit::begin() const -> const_iterator {
    return const_iterator(const_cast<Unit*>(this), 0);
}

auto gkit::core::scene::Unit::end() const -> const_iterator {
    return const_iterator(const_cast<Unit*>(this), children.size());
}

auto gkit::core::scene::Unit::cbegin() const -> const_iterator { return begin(); }
auto gkit::core::scene::Unit::cend() const -> const_iterator { return end(); }

// This is a reverse iterator, implemented using std::reverse_iterator.
// using at here maybe have some problem, just I guess,
// So I deleted it

// using reverse_iterator = std::reverse_iterator<gkit::core::scene::Unit::iterator>;
// using const_reverse_iterator = std::reverse_iterator<gkit::core::scene::Unit::const_iterator>;

auto gkit::core::scene::Unit::rbegin() -> reverse_iterator { return reverse_iterator(end()); }
auto gkit::core::scene::Unit::rend() -> reverse_iterator { return reverse_iterator(begin()); }
auto gkit::core::scene::Unit::rbegin() const -> const_reverse_iterator { return const_reverse_iterator(end()); }
auto gkit::core::scene::Unit::rend() const -> const_reverse_iterator { return const_reverse_iterator(begin()); }
auto gkit::core::scene::Unit::crbegin() const -> const_reverse_iterator { return rbegin(); }
auto gkit::core::scene::Unit::crend() const -> const_reverse_iterator { return rend(); }
