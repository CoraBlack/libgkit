#pragma once

#include "singleton.hpp"

#include <cstdint>
#include <functional>
#include <mutex>
#include <stack>
#include <unordered_map>
#include <utility>

namespace gkit::core::templates {
    /**
     * @brief Generic ID value type with a generation counter.
     *
     * An id is identified by the pair (id, gen). The generation is bumped on
     * every recycle so a stale reference from a previous incarnation of the id
     * never aliases a newly allocated one (prevents the ABA problem).
     *
     * @tparam T tag type used only to give each id flavor a distinct type.
     */
    template<class T>
    class GenId {
        std::uint32_t id  = 0u;
        std::uint32_t gen = 0u;

    public:
        class IdAllocator;

        constexpr GenId() noexcept = default;
        ~GenId()                   = default;

        GenId(const GenId<T>& other) noexcept                       = default;
        GenId(GenId<T>&& other) noexcept                            = default;
        auto operator=(const GenId<T>& other) noexcept -> GenId<T>& = default;
        auto operator=(GenId<T>&& other) noexcept -> GenId<T>&      = default;

        /**
         * @brief check the id is valid
         * @return bool - both id and generation are non-zero.
         */
        [[nodiscard]] constexpr auto available() const noexcept -> bool { return this->id != 0 && this->gen != 0; }

        /**
         * @brief the raw id number
         */
        [[nodiscard]] constexpr auto get_id() const noexcept -> std::uint32_t { return this->id; }

        /**
         * @brief the generation counter of the id
         */
        [[nodiscard]] constexpr auto get_generation() const noexcept -> std::uint32_t { return this->gen; }

        friend constexpr auto operator==(const GenId<T>& lhs, const GenId<T>& rhs) noexcept -> bool {
            return lhs.id == rhs.id && lhs.gen == rhs.gen;
        }

        friend constexpr auto operator!=(const GenId<T>& lhs, const GenId<T>& rhs) noexcept -> bool {
            return !(lhs == rhs);
        }
    }; // class GenId

    template<class T>
    class GenId<T>::IdAllocator : public Singleton<GenId<T>::IdAllocator> {
        friend Singleton<GenId<T>::IdAllocator>;
        std::mutex alloc_mutex{};
        std::stack<std::uint32_t> id_pool{{1}};
        std::unordered_map<std::uint32_t, std::uint32_t> id_gen_map{};

        IdAllocator() noexcept  = default;
        ~IdAllocator() noexcept = default;

    public:
        auto new_one() -> GenId<T>;
        auto drop(const GenId<T>& dropped_id) -> void;
    }; // class GenId<T>::IdAllocator

    template<class T>
    auto GenId<T>::IdAllocator::new_one() -> GenId<T> {
        auto new_id = GenId<T>();
        {
            std::unique_lock alloc_locker(this->alloc_mutex);
            if (this->id_pool.size() <= 1) {
                // Final element in id_pool is not alloc before be called top().
                // So the version of final element always is one.
                new_id.id  = id_pool.top()++;
                new_id.gen = 1;
            } else {
                // version of recycle id is not always one
                // and is recorded to gen map when the id is dropped.
                new_id.id = id_pool.top();
                id_pool.pop();
                new_id.gen = ++this->id_gen_map[new_id.id];
            }
        }

        return new_id;
    }

    template<class T>
    auto GenId<T>::IdAllocator::drop(const GenId<T>& dropped_id) -> void {
        auto old_id  = dropped_id.id;
        auto old_gen = dropped_id.gen;

        {
            std::unique_lock alloc_locker(this->alloc_mutex);
            this->id_pool.push(old_id);

            // Record the last-used generation so the next allocation of this id
            // gets old_gen + 1, keeping the generation strictly increasing.
            this->id_gen_map[old_id] = old_gen;
        }
    }
} // namespace gkit::core::templates

template<class T>
struct std::hash<gkit::core::templates::GenId<T>> {
    auto operator()(const gkit::core::templates::GenId<T>& obj_id) const -> std::size_t {
        auto uint_hash = std::hash<std::uint32_t>();
        return uint_hash(obj_id.get_id()) ^ (uint_hash(obj_id.get_generation()) << 1);
    }
};
