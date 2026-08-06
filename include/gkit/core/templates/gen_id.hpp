#pragma once

#include "singleton.hpp"

#include <cstdint>
#include <mutex>
#include <stack>
#include <unordered_map>
#include <utility>

namespace gkit::core::templates {
    template<class T>
    class GenId {
        std::uint32_t id  = 0u;
        std::uint32_t gen = 0u;

    public:
        class IdAllocator;

        GenId()          = default;
        virtual ~GenId() = default;
        GenId(const GenId<T>& other) : id(other.id), gen(other.gen) {}
        GenId(GenId<T>&& other) noexcept : id(other.id), gen(other.gen) {
            other.id  = 0u;
            other.gen = 0u;
        }

        auto operator=(const GenId<T>&) -> GenId<T>& = default;
        auto operator=(GenId<T>&& other) noexcept -> GenId<T>& {
            if (this != &other) {
                this->id  = other.id;
                this->gen = other.gen;
                other.id  = 0u;
                other.gen = 0u;
            }

            return *this;
        };

        inline auto operator==(const GenId<T>& other) const -> bool {
            return this->id == other.id && this->gen == other.gen;
        }

        /**
         * @brief check ObjectId is invalid
         * @return bool - Both id and version are not zero.
         */
        inline auto available() const -> bool { return this->id != 0 && this->gen != 0; }
        inline auto properties() const -> std::pair<uint32_t, uint32_t> { return {this->id, this->gen}; }
    }; // class GenId

    template<class T>
    class GenId<T>::IdAllocator : public Singleton<GenId<T>::IdAllocator> {
        friend Singleton<GenId<T>::IdAllocator>;
        std::mutex alloc_mutex{};
        std::stack<uint32_t> id_pool{{1}};
        std::unordered_map<uint32_t, uint32_t> id_gen_map{};

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
        auto old_id      = dropped_id.id;
        auto old_version = dropped_id.gen;

        {
            std::unique_lock alloc_locker(this->alloc_mutex);
            this->id_pool.push(old_id);
            auto version_it = this->id_gen_map.find(old_id);

            // no record version ever
            if (version_it == this->id_gen_map.end()) {
                this->id_gen_map[old_id] = 1;
            }
        }
    }
} // namespace gkit::core::templates

template<class T>
struct std::hash<gkit::core::templates::GenId<T>> {
    auto operator()(const gkit::core::templates::GenId<T>& obj_id) const -> std::size_t {
        auto uint_hash     = std::hash<uint32_t>();
        auto [id, version] = obj_id.properties();
        return uint_hash(id) ^ uint_hash(version);
    }
};