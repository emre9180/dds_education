#ifndef DDS_SUBSCRIBER_HPP
#define DDS_SUBSCRIBER_HPP

#include <memory>
#include <functional>
#include "dds/Topic.hpp"
#include "dds/Transport.hpp"
#include "dds/Message.hpp"

namespace dds {

/**
 * @brief Subscriber for a specific Topic and message type.
 *
 * @tparam T The message type.
 */
template<typename T>
class Subscriber {
public:
    using TopicPtr = std::shared_ptr<Topic<T>>;
    using TransportPtr = std::shared_ptr<Transport>;
    using Callback = std::function<void(const Message<T>&)>;

    virtual ~Subscriber() = default;

    void set_callback(Callback cb) {
        callback_ = cb;
        if (transport_ && topic_) {
            transport_->set_receive_callback(*topic_, cb);
        }
    }

protected:
    Subscriber(const TopicPtr& topic, const TransportPtr& transport)
        : topic_(topic), transport_(transport) {}

    TopicPtr topic_;
    TransportPtr transport_;
    Callback callback_;

    friend class DomainParticipant;
};

} // namespace dds

#endif // DDS_SUBSCRIBER_HPP 