#ifndef DDS_TRANSPORT_HPP
#define DDS_TRANSPORT_HPP

#include <functional>
#include "dds/AnyTopic.hpp"
#include "dds/AnyMessage.hpp"

namespace dds {

/**
 * @brief Abstract Transport interface for type-erased message delivery.
 * Uses AnyTopic and AnyMessage for runtime polymorphism.
 */
class Transport {
public:
    virtual ~Transport() = default;

    /**
     * @brief Send a type-erased message to a type-erased topic.
     * @param topic The topic (type-erased)
     * @param msg The message (type-erased)
     */
    virtual void send(const AnyTopic& topic, const AnyMessage& msg) = 0;

    /**
     * @brief Register a type-erased receive callback for a topic.
     * @param topic The topic (type-erased)
     * @param cb The callback to invoke on message arrival (type-erased)
     */
    virtual void set_receive_callback(const AnyTopic& topic, std::function<void(const AnyMessage&)>&& cb) = 0;
};

} // namespace dds

#endif // DDS_TRANSPORT_HPP 