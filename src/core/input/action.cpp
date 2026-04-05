#include "gkit/core/input/action.hpp"
#include "gkit/core/input/input.hpp"


gkit::input::Action::Action(std::string name, bool auto_register) : name(std::move(name)) {
    if (auto_register) {
        gkit::Input::instance().register_action(*this);
    }
}
