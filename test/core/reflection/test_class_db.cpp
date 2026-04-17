#include <gkit/core/reflection/class_db.hpp>
#include <iostream>
#include <cassert>
#include <string>

using gkit::core::reflection::ClassDB;

struct Player {
    std::string name;
    int level;
    float health;
};

struct Game {
    int id;
    std::string title;
};

auto test_class_registration() -> void {
    std::cout << "=== Test: Class Registration ===" << std::endl;

    auto& db = ClassDB::instance();

    db.register_class<Player>();
    db.register_class<Game>();

    auto* player_info = db.get_class<Player>();
    assert(player_info != nullptr);
    std::cout << "  Player class registered: OK" << std::endl;

    auto* game_info = db.get_class<Game>();
    assert(game_info != nullptr);
    std::cout << "  Game class registered: OK" << std::endl;

    auto* unknown_info = db.get_class<std::string>();
    assert(unknown_info == nullptr);
    std::cout << "  Unknown class returns nullptr: OK" << std::endl;

    std::cout << "All registration tests passed!" << std::endl << std::endl;
}

auto test_field_registration() -> void {
    std::cout << "=== Test: Field Registration ===" << std::endl;

    auto& db = ClassDB::instance();

    db.register_class<Player>();
    db.register_field<Player>("name", &Player::name);
    db.register_field<Player>("level", &Player::level);
    db.register_field<Player>("health", &Player::health);

    auto* player_info = db.get_class<Player>();
    assert(player_info != nullptr);

    const auto& fields = player_info->fields();
    assert(fields.size() == 3);
    std::cout << "  Player has 3 fields: OK" << std::endl;

    assert(fields[0].name() == "name");
    assert(fields[1].name() == "level");
    assert(fields[2].name() == "health");
    std::cout << "  Field names correct: OK" << std::endl;

    std::cout << "All field registration tests passed!" << std::endl << std::endl;
}

auto test_field_get() -> void {
    std::cout << "=== Test: Field Get ===" << std::endl;

    auto& db = ClassDB::instance();

    Player player{"Hero", 10, 75.5f};

    auto* player_info = db.get_class<Player>();
    assert(player_info != nullptr);

    const auto& fields = player_info->fields();

    auto name_val = fields[0].get(&player);
    assert(name_val.has_value());
    assert(name_val->is<std::string>());
    assert(name_val->get<std::string>().value() == "Hero");
    std::cout << "  name field: OK" << std::endl;

    auto level_val = fields[1].get(&player);
    assert(level_val.has_value());
    assert(level_val->is<std::int64_t>());
    assert(level_val->get<std::int64_t>().value() == 10);
    std::cout << "  level field: OK" << std::endl;

    auto health_val = fields[2].get(&player);
    assert(health_val.has_value());
    assert(health_val->is<float>());
    std::cout << "  health field: OK" << std::endl;

    std::cout << "All field get tests passed!" << std::endl << std::endl;
}

auto test_multiple_classes() -> void {
    std::cout << "=== Test: Multiple Classes ===" << std::endl;

    auto& db = ClassDB::instance();

    db.register_class<Game>();
    db.register_field<Game>("id", &Game::id);
    db.register_field<Game>("title", &Game::title);

    Game game{1, "Adventure"};

    auto* game_info = db.get_class<Game>();
    assert(game_info != nullptr);

    const auto& fields = game_info->fields();

    auto id_val = fields[0].get(&game);
    assert(id_val->get<std::int64_t>().value() == 1);
    std::cout << "  Game id: OK" << std::endl;

    auto title_val = fields[1].get(&game);
    assert(title_val->get<std::string>().value() == "Adventure");
    std::cout << "  Game title: OK" << std::endl;

    std::cout << "All multiple classes tests passed!" << std::endl << std::endl;
}

auto main() -> int {
    std::cout << "========================================" << std::endl;
    std::cout << "    gkit::core::reflection Tests      " << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    test_class_registration();
    test_field_registration();
    test_field_get();
    test_multiple_classes();

    std::cout << "========================================" << std::endl;
    std::cout << "         ALL TESTS PASSED!             " << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}