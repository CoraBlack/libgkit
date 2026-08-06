#pragma once

#include "gkit/core/object_id.hpp"
#include "gkit/core/templates/gen_id.hpp"

namespace gkit::core {
    class ObjectIdTag;
    using ObjectId = templates::GenId<ObjectIdTag>;
} // namespace gkit::core