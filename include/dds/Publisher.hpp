#ifndef DDS_PUBLISHER_HPP
#define DDS_PUBLISHER_HPP

#include <memory>
#include <string>
#include "dds/Topic.hpp"
#include "dds/Transport.hpp"
#include "dds/Message.hpp"
#include "dds/AnyTopic.hpp"
#include "dds/AnyMessage.hpp"

namespace dds {

/**
 * @brief Publisher for a specific Topic and message type.
 * Publishes messages by delegating to the Transport layer (type-erased).
 */
template<typename T>
class Publisher {
public:
    using TopicPtr = std::shared_ptr<Topic<T>>;
    using TransportPtr = std::shared_ptr<Transport>;

    virtual ~Publisher() = default;

    /**
     * @brief Publish a message to the topic via the Transport (type-erased).
     * @param msg The message to publish
     */
    void publish(const Message<T>& msg) {
        if (transport_ && topic_) {
            transport_->send(AnyTopic(*topic_), AnyMessage(msg));
        }
    }

protected:
    /**
     * @brief Protected constructor, only accessible by DomainParticipant.
     * @param topic The topic to publish to
     * @param transport The transport to use
     */
    Publisher(const TopicPtr& topic, const TransportPtr& transport)
        : topic_(topic), transport_(transport) {}

    TopicPtr topic_;
    TransportPtr transport_;

    friend class DomainParticipant;
};

} // namespace dds

#endif // DDS_PUBLISHER_HPP 