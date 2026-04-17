#pragma once

#include "gkit/core/scene/singleton.hpp"
#include "gkit/core/variant.hpp"

#include <concepts>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace gkit::core::reflection {

    /**
     * @brief Stores information about a registered class field.
     *
     * This class encapsulates the name, type name, and a generic getter 
     * for a specific field within a registered class.
     */
    class FieldInfo {
    public:
        /**
         * @brief Default constructor for FieldInfo.
         */
        FieldInfo() = default;

        /**
         * @brief Constructs a new FieldInfo with the given name, type, and getter.
         * 
         * @param name The name of the field.
         * @param type_name The type name of the field.
         * @param getter A functional object that retrieves the field's value from an instance.
         */
        FieldInfo(std::string name, std::string type_name,
                  std::function<auto(const void*) -> std::optional<Variant>> getter);

        /**
         * @brief Gets the name of the field.
         * 
         * @return The field name as a string reference.
         */
        [[nodiscard]] auto name() const -> const std::string&;

        /**
         * @brief Gets the type name of the field.
         * 
         * @return The type name as a string reference.
         */
        [[nodiscard]] auto type_name() const -> const std::string&;

        /**
         * @brief Retrieves the value of this field from the given instance.
         * 
         * @param instance A pointer to the object instance.
         * @return An optional Variant containing the field value, or nullopt if invalid.
         */
        [[nodiscard]] auto get(const void* instance) const -> std::optional<Variant>;

    private:
        std::string name_;
        std::string type_name_;
        std::function<auto(const void*) -> std::optional<Variant>> getter_;
    };

    /**
     * @brief Stores reflection information about a registered class.
     *
     * This includes the class name and a collection of its registered fields.
     */
    class ClassInfo {
    public:
        /**
         * @brief Default constructor for ClassInfo.
         */
        ClassInfo() = default;

        /**
         * @brief Constructs a new ClassInfo with the specified class name.
         * 
         * @param class_name The name of the class.
         */
        explicit ClassInfo(std::string class_name);

        /**
         * @brief Gets the class name.
         * 
         * @return The class name as a string reference.
         */
        [[nodiscard]] auto class_name() const -> const std::string&;

        /**
         * @brief Gets the registered fields of this class.
         * 
         * @return A vector of FieldInfo references.
         */
        [[nodiscard]] auto fields() const -> const std::vector<FieldInfo>&;

        /**
         * @brief Adds a new field to this class's reflection data.
         * 
         * @param field The FieldInfo to add.
         */
        auto add_field(FieldInfo field) -> void;

    private:
        std::string class_name_;
        std::vector<FieldInfo> fields_;
    };

    template <class T>
    concept Bindable = requires (T v) {
        { T::_register() } -> std::same_as<void>;
    };

    /**
     * @brief A centralized database for class reflection information.
     *
     * ClassDB allows registering classes and their fields, enabling runtime 
     * inspection and dynamic value retrieval.
     */
    class ClassDB : public gkit::core::scene::Singleton<ClassDB> {
        friend class gkit::core::scene::Singleton<ClassDB>;

    public:
        /**
         * @brief Registers a class into the reflection database.
         * 
         * @tparam T The class type to register.
         */
        template<typename T>
        auto register_class() -> void {
            std::lock_guard<std::mutex> lock(class_map_mutex);
            std::string class_name = typeid(T).name();
            if (class_map.find(class_name) == class_map.end()) {
                class_map[class_name] = ClassInfo(class_name);
            }

            if constexpr (Bindable<T>) {
                T::_register();
            }
        }

        /**
         * @brief Registers a field for a specific class.
         * 
         * Automatically registers the class if it hasn't been registered yet.
         * 
         * @tparam T The class type.
         * @tparam FieldT The type of the field.
         * @param name The name to associate with the field.
         * @param field A pointer to the class member.
         */
        template<typename T, typename FieldT>
        auto register_field(std::string name, FieldT T::* field) -> void {
            std::lock_guard<std::mutex> lock(class_map_mutex);
            std::string class_name = typeid(T).name();
            auto it = class_map.find(class_name);
            if (it == class_map.end()) {
                class_map[class_name] = ClassInfo(class_name);
                it = class_map.find(class_name);
            }

            auto getter = [field](const void* obj) -> std::optional<Variant> {
                const T* instance = static_cast<const T*>(obj);
                return Variant(instance->*field);
            };

            it->second.add_field(FieldInfo(std::move(name), typeid(FieldT).name(), std::move(getter)));
        }

        /**
         * @brief Retrieves reflection information for a class by name.
         * 
         * @param class_name The string name of the class (as returned by typeid).
         * @return A pointer to the ClassInfo, or nullptr if not found.
         */
        [[nodiscard]] auto get_class(std::string_view class_name) const -> const ClassInfo*;

        /**
         * @brief Retrieves reflection information for a class type.
         * 
         * @tparam T The class type.
         * @return A pointer to the ClassInfo, or nullptr if not found.
         */
        template<typename T>
        [[nodiscard]] auto get_class() const -> const ClassInfo* {
            return get_class(typeid(T).name());
        }

    private:
        /**
         * @brief Private constructor to enforce singleton pattern.
         */
        ClassDB() = default;

        mutable std::mutex class_map_mutex;
        std::unordered_map<std::string, ClassInfo> class_map;
    };

} // namespace gkit::core::reflection