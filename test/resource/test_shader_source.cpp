#include <cassert>
#include <filesystem>
#include <iostream>
#include <string_view>

#include <gkit/resource/resource_loader.hpp>
#include <gkit/resource/shader_source.hpp>

namespace fs = std::filesystem;

namespace {

    /**
     * @brief Get path to the test resource directory (same folder as this source file)
     */
    auto resource_dir() -> fs::path {
        return fs::path(__FILE__).parent_path();
    }

} // anonymous namespace

auto test_load_vertex_and_fragment() -> void {
    std::cout << "=== Test: Load Vertex and Fragment Shader ===" << '\n';

    const auto path = resource_dir() / "basic.shader";
    auto result     = gkit::resource::ResourceLoader::instance().load<gkit::resource::ShaderSource>(path);

    assert(result.has_value());
    const auto& shader = result.value();
    assert(shader->is_loaded());

    // Print raw file content
    auto full = shader->source();
    std::cout << "\n--- Raw Shader Source ---\n" << full << "--- End Raw Source ---\n" << '\n';

    // Print per-stage sources
    std::cout << "--- Vertex Shader ---\n"
              << shader->get_source(gkit::resource::ShaderStage::Vertex) << "--- End Vertex Shader ---\n"
              << '\n';
    std::cout << "--- Fragment Shader ---\n"
              << shader->get_source(gkit::resource::ShaderStage::Fragment) << "--- End Fragment Shader ---\n"
              << '\n';

    assert(full.find("#shader vertex") != std::string_view::npos);
    assert(full.find("#shader fragment") != std::string_view::npos);

    // Per-stage access
    assert(shader->has_stage(gkit::resource::ShaderStage::Vertex));
    assert(shader->has_stage(gkit::resource::ShaderStage::Fragment));

    auto vs = shader->get_source(gkit::resource::ShaderStage::Vertex);
    assert(vs.find("gl_Position") != std::string_view::npos);
    assert(vs.find("aPos") != std::string_view::npos);

    auto fs = shader->get_source(gkit::resource::ShaderStage::Fragment);
    assert(fs.find("FragColor") != std::string_view::npos);

    std::cout << "  Vertex and fragment stages parsed correctly: OK" << '\n';
    std::cout << "Test passed!" << '\n' << '\n';
}

auto test_cache_returns_same_instance() -> void {
    std::cout << "=== Test: Cache Returns Same Instance ===" << '\n';

    const auto path = resource_dir() / "basic.shader";

    auto first  = gkit::resource::ResourceLoader::instance().load<gkit::resource::ShaderSource>(path);
    auto second = gkit::resource::ResourceLoader::instance().load<gkit::resource::ShaderSource>(path);

    assert(first.has_value());
    assert(second.has_value());

    // Same pointer — cached instance
    assert(first.value().get() == second.value().get());

    std::cout << "  Loading same path twice returns cached instance: OK" << '\n';
    std::cout << "Test passed!" << '\n' << '\n';
}

auto test_load_missing_file() -> void {
    std::cout << "=== Test: Load Missing File ===" << '\n';

    auto result = gkit::resource::ResourceLoader::instance().load<gkit::resource::ShaderSource>(
        resource_dir() / "does_not_exist.shader");

    assert(!result.has_value());

    std::cout << "  Missing file returns nullopt: OK" << '\n';
    std::cout << "Test passed!" << '\n' << '\n';
}

auto main() -> int {
    std::cout << "========================================" << '\n';
    std::cout << "   gkit::resource::ShaderSource Tests   " << '\n';
    std::cout << "========================================" << '\n' << '\n';

    test_load_vertex_and_fragment();
    test_cache_returns_same_instance();
    test_load_missing_file();

    std::cout << "========================================" << '\n';
    std::cout << "         ALL TESTS PASSED!             " << '\n';
    std::cout << "========================================" << '\n';

    return 0;
}
