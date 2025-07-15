#ifndef DDS_SUBSCRIBER_HPP
#define DDS_SUBSCRIBER_HPP

#include <memory>
#include <functional>
#include "dds/Topic.hpp"
#include "dds/Transport.hpp"
#include "dds/Message.hpp"
#include "dds/AnyTopic.hpp"
#include "dds/AnyMessage.hpp"
#include <typeinfo>
#include <functional> // for std::ref

namespace dds {

/**
 * @brief Subscriber for a specific Topic and message type.
 * Receives messages by registering a type-erased callback with the Transport layer.
 */
template<typename T>
class Subscriber {
public:
    using TopicPtr = std::shared_ptr<Topic<T>>;
    using TransportPtr = std::shared_ptr<Transport>;
    using Callback = std::function<void(const Message<T>&)>;

    virtual ~Subscriber() = default;

    /**
     * @brief Register a callback to be invoked when a message is received.
     *        The callback is wrapped and registered as type-erased with the Transport.
     * @param cb The callback function
     */
    void set_callback(Callback cb) {
        callback_ = cb;
        if (transport_ && topic_) {
            transport_->set_receive_callback(
                AnyTopic(*topic_),
                [cb, type = std::ref(typeid(Message<T>))](const AnyMessage& any_msg) {
                    if (any_msg.type() == type.get()) {
                        cb(any_msg.get<T>());
                    }
                }
            );
        }
    }

protected:
    /**
     * @brief Protected constructor, only accessible by DomainParticipant.
     * @param topic The topic to subscribe to
     * @param transport The transport to use
     */
    Subscriber(const TopicPtr& topic, const TransportPtr& transport)
        : topic_(topic), transport_(transport) {}

    TopicPtr topic_;
    TransportPtr transport_;
    Callback callback_;

    friend class DomainParticipant;
};

} // namespace dds

#endif // DDS_SUBSCRIBER_HPP 