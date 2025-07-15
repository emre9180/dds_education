#ifndef DDS_TOPIC_HPP
#define DDS_TOPIC_HPP

#include <string>
#include <memory>
#include <typeinfo>

namespace dds {

/**
 * @brief Represents a named topic for message exchange, templated by message type.
 */
template<typename T>
class Topic {
public:
    Topic(const std::string& name) : name_(name) {}
    virtual ~Topic() = default;

    const std::string& name() const { return name_; }
    const std::type_info& type() const { return typeid(T); }

private:
    std::string name_;
};

} // namespace dds

#endif // DDS_TOPIC_HPP 