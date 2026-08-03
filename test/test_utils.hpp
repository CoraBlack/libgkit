#pragma once

#include <cstdlib>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>


namespace gkit::test {
    class TestRunner {
        std::vector<std::function<bool()>> test_funcs;

    public:
        TestRunner()  = default;
        ~TestRunner() = default;

        auto add_test_func(std::function<bool()>&& func) -> TestRunner& {
            this->test_funcs.emplace_back(std::move(func));
            return *this;
        }

        auto run() -> void {
            auto res        = true;
            auto fail_count = 0;
            auto test_count = this->test_funcs.size();

            for (auto&& func : test_funcs) {
                if (!func()) {
                    fail_count += 1;
                    res = false;
                }
            }

            if (res) {
                std::cout << std::format("All tests(total {}) passed!", test_count) << '\n';
            } else {
                std::cout << std::format("Some tests(total {}) failed!", fail_count) << '\n';
            }

            return;
        }
    };

    auto assert_if(bool cond, const std::string& failed_msg) -> void {
        if (!cond) {
            std::cout << failed_msg << '\n';
            std::abort();
        }
    }

    auto assert_ifnot(bool cond, const std::string& failed_msg) -> void {
        assert_if(!cond, failed_msg);
    }

    template<class... Args>
    auto logln(std::format_string<Args...> fmt, Args&&... args) -> void {
        std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
    }
} // namespace gkit::test
