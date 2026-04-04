#pragma once

#include "gkit/core/scene/unit.hpp"
#include "gkit/core/utils/Singleton.hpp"
#include <atomic>
#include <memory>

namespace gkit {

    class Application : public utils::Singleton<Application> {
    public:
        explicit Application() noexcept;
        virtual ~Application() noexcept;

    public:
        auto run() -> void;
        auto stop() -> void;

        auto set_root(std::unique_ptr<scene::Unit>&& root_ptr) noexcept -> void;

    private:
        std::unique_ptr<scene::Unit> root;
        std::unique_ptr<scene::Unit> singleton_units;
        std::atomic<bool> running = false;
    }; // class Application

} // namespace gkit