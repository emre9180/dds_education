#ifndef DDS_ANYTOPIC_HPP
#define DDS_ANYTOPIC_HPP

#include <string>
#include <typeinfo>
#include <memory>
#include "dds/Topic.hpp"

namespace dds {

/**
 * @brief Type-erased topic holder for runtime polymorphism.
 */
class AnyTopic {
public:
    AnyTopic() = default;
    AnyTopic(const AnyTopic&) = default;
    AnyTopic(AnyTopic&&) = default;
    AnyTopic& operator=(const AnyTopic&) = default;
    AnyTopic& operator=(AnyTopic&&) = default;
    ~AnyTopic() = default;

    template<typename T>
    AnyTopic(const Topic<T>& topic)
        : name_(topic.name()), type_(&typeid(T)) {}

    const std::string& name() const { return name_; }
    const std::type_info& type() const { return *type_; }

    bool valid() const { return !name_.empty() && type_ != nullptr; }

private:
    std::string name_;
    const std::type_info* type_ = nullptr;
};

} // namespace dds

#endif // DDS_ANYTOPIC_HPP 