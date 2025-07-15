#ifndef DDS_ANYMESSAGE_HPP
#define DDS_ANYMESSAGE_HPP

#include <any>
#include <typeinfo>
#include <memory>
#include "dds/Message.hpp"

namespace dds {

/**
 * @brief Type-erased message holder for runtime polymorphism.
 */
class AnyMessage {
public:
    AnyMessage() = default;
    AnyMessage(const AnyMessage&) = default;
    AnyMessage(AnyMessage&&) = default;
    AnyMessage& operator=(const AnyMessage&) = default;
    AnyMessage& operator=(AnyMessage&&) = default;
    ~AnyMessage() = default;

    template<typename T>
    AnyMessage(const Message<T>& msg)
        : msg_(msg), type_(&typeid(Message<T>)) {}

    const std::type_info& type() const { return *type_; }

    template<typename T>
    const Message<T>& get() const {
        if (typeid(Message<T>) != *type_) throw std::bad_any_cast();
        return std::any_cast<const Message<T>&>(msg_);
    }

    bool has_value() const { return msg_.has_value(); }

private:
    std::any msg_;
    const std::type_info* type_ = nullptr;
};

} // namespace dds

#endif // DDS_ANYMESSAGE_HPP 