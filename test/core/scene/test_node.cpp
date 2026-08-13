#include "gkit/scene/node.hpp"
#include "test_utils.hpp"

using gkit::core::UniqueObject;
using gkit::scene::Node;

class TestNode : public Node {
public:
    using Node::Node;

    auto ready() -> void override {}
    auto process(double) -> void override {}
    auto physical_process(double) -> void override {}
    auto exit() -> void override {}
};

auto test_self_id() -> bool {
    gkit::test::logln("=== self id ===");

    auto node      = UniqueObject::create<TestNode>("root");
    auto* node_ptr = node.get_id().deref_as<TestNode>();

    gkit::test::assert_if(node.get() != nullptr, "UniqueObject::get() returned null");
    gkit::test::assert_if(node_ptr != nullptr, "deref_as<TestNode> returned null");
    // Object::get_id() (self-id) must match the UniqueObject handle's id.
    gkit::test::assert_if(node_ptr->get_id() == node.get_id(), "Object::get_id() == UniqueObject::get_id()");

    return true;
}

auto test_parent_wiring() -> bool {
    gkit::test::logln("=== parent wiring ===");

    auto parent = UniqueObject::create<TestNode>("parent");
    auto child  = UniqueObject::create<TestNode>("child");

    auto* parent_node = parent.get_id().deref_as<TestNode>();
    auto child_id     = child.get_id();
    auto* child_node  = child_id.deref_as<TestNode>();

    gkit::test::assert_if(parent_node != nullptr, "parent deref failed");
    gkit::test::assert_if(child_node != nullptr, "child deref failed");

    // Before adding, the child has no parent.
    gkit::test::assert_if(!child_node->get_parent().available(), "child should have no parent initially");

    parent_node->add_child(std::move(child));

    gkit::test::assert_if(child_node->get_parent() == parent.get_id(), "child.get_parent() == parent.get_id()");
    gkit::test::assert_if(parent_node->get_child(0) == child_id, "get_child(0) returns child id");
    gkit::test::assert_if(parent_node->get_child("child") == child_id, "get_child(\"child\") returns child id");

    return true;
}

auto main() -> int {
    auto test_runner = gkit::test::TestRunner()
                           .add_test_func(test_self_id)
                           .add_test_func(test_parent_wiring);

    test_runner.run();
    return 0;
}
