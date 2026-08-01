#include "core/reflect/seralize.hpp"
#include "core/object_pool.hpp"
#include "gkit/core/reflect/registry.hpp"

namespace gkit::core::reflect {
    SerdeStruct::SerdeStruct(const ObjectId v) noexcept {
        if (!v.available()) return;

        // TODO(impl) cora -replace deref_from by ObjectId::deref or dereference operator 
        auto* v_ptr = ObjectPool::instance().deref_from(v);
        auto& db = ClassDB::instance();
        /* auto* class_info = db.find(v_ptr) */
        this->available_flag = true;
    }
} // namespace gkit::core::reflect