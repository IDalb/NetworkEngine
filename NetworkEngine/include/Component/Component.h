#pragma once
#include <functional>
#include <map>

#include "TypeDef.h"
#include "yaml-cpp/yaml.h"

namespace GDE {
    class Entity;

    struct ComponentDescription {
        std::map<std::string, Description> parameters;

        explicit ComponentDescription(const Description& description) {
            for (auto& param : description)
                parameters.insert({param.first.as<std::string>(), param.second});
        }
    };

    class Component {
        using CreateFunctionType = std::function<std::unique_ptr<Component>(Entity&)>;

    protected:
        inline static std::map<uint32_t, std::string> componentIdToName;

        bool _enabled{true};
        Entity& _parent;

        static void Register(std::string_view type, const CreateFunctionType& create_function);
        static inline std::unordered_map<std::string, CreateFunctionType> _registry;

        virtual void OnEnable() {}
        virtual void OnDisable() {}


    private:
        void enable() {
            if (!_enabled) {
                _enabled = true;
                OnEnable();
            }
        }

        void disable() {
            if (_enabled) {
                _enabled = false;
                OnDisable();
            }
        }

    public:
        Component(Entity& parent) : _parent(parent) {}
        virtual ~Component() = default;

        Component(const Component&) = default;
        Component& operator=(const Component&) = default;
        Component(Component&&) = default;
        Component& operator=(Component&&) = default;

        virtual void setup(const ComponentDescription& init_value) = 0;
        virtual void resolve() {}

        virtual std::string serialize() { return ""; }
        virtual void deserialize(char*& data, uint32_t frameIndex) {}

        virtual void setValue(std::string_view variable, float value) {};

        // Register the type (std::string T::Type), with the corresponding creation function
        // This let the Create function below be able to create a component with only its name
        template<typename T>
        static void Register() {
            Register(T::type, [](Entity& owner) { return std::make_unique<T>(owner); });
        }
        static std::unique_ptr<Component> create(std::string_view type, Entity& parent);

        Entity& owner() const { return _parent; }

        bool enabled() const { return _enabled; }

        void setEnabled(bool enabled) {
            if (enabled) enable();
            else disable();
        }
        static const std::string getComponentName(uint32_t netId) { return componentIdToName.contains(netId) ? componentIdToName.at(netId) : ""; }


        static constexpr unsigned int SIZEOF_ID = sizeof(uint16_t);
    };
}
