#include <mutex>

#include <gkit/resource/resource_loader.hpp>

auto gkit::resource::ResourceLoader::push_to_cache(const std::shared_ptr<gkit::resource::Resource>& res) -> void {
    if (res == nullptr || !res->available()) {
        return;
    }

    std::unique_lock lock(this->cache_rw_mutex);
    this->resource_cache.insert(std::make_pair(res->get_path(), res));
}

auto gkit::resource::ResourceLoader::get_cache(const std::filesystem::path& path)
    -> std::optional<std::shared_ptr<gkit::resource::Resource>> {
    std::shared_lock lock(this->cache_rw_mutex);
    if (!this->resource_cache.contains(path)) {
        return std::nullopt;
    }

    return this->resource_cache.at(path);
}