#ifndef DDS_PUBLISHER_HPP
#define DDS_PUBLISHER_HPP

#include <memory>
#include <string>
#include "dds/Topic.hpp"
#include "dds/Transport.hpp"
#include "dds/Message.hpp"

namespace dds {

template<typename T>
class Publisher {
public:
    using TopicPtr = std::shared_ptr<Topic<T>>;
    using TransportPtr = std::shared_ptr<Transport>;

    virtual ~Publisher() = default;

    void publish(const Message<T>& msg) {
        if (transport_ && topic_) {
            transport_->send(*topic_, msg);
        }
    }

protected:
    Publisher(const TopicPtr& topic, const TransportPtr& transport)
        : topic_(topic), transport_(transport) {}

    TopicPtr topic_;
    TransportPtr transport_;

    friend class DomainParticipant;
};

} // namespace dds

#endif // DDS_PUBLISHER_HPP 