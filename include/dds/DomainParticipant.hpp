#ifndef DDS_DOMAIN_PARTICIPANT_HPP
#define DDS_DOMAIN_PARTICIPANT_HPP

#include <memory>
#include <string>
#include "dds/Publisher.hpp"
#include "dds/Subscriber.hpp"
#include "dds/Topic.hpp"
#include "dds/Transport.hpp"

namespace dds {

/**
 * @brief DomainParticipant manages the DDS domain and acts as a factory for Publisher and Subscriber.
 * Injects shared Transport dependency and ensures consistent initialization.
 */
class DomainParticipant {
public:
    using TransportPtr = std::shared_ptr<Transport>;

    /**
     * @brief Construct a DomainParticipant with a shared Transport.
     * @param transport The transport to use for all publishers and subscribers
     */
    explicit DomainParticipant(const TransportPtr& transport)
        : transport_(transport) {}
    virtual ~DomainParticipant() = default;

    /**
     * @brief Create a Publisher for a given topic.
     * @tparam T Message type
     * @param topic The topic to publish to
     * @return Shared pointer to Publisher<T>
     */
    template<typename T>
    std::shared_ptr<Publisher<T>> create_publisher(const std::shared_ptr<Topic<T>>& topic) const {
        // Use direct new to allow protected constructor
        return std::shared_ptr<Publisher<T>>(new Publisher<T>(topic, transport_));
    }

    /**
     * @brief Create a Subscriber for a given topic.
     * @tparam T Message type
     * @param topic The topic to subscribe to
     * @return Shared pointer to Subscriber<T>
     */
    template<typename T>
    std::shared_ptr<Subscriber<T>> create_subscriber(const std::shared_ptr<Topic<T>>& topic) const {
        // Use direct new to allow protected constructor
        return std::shared_ptr<Subscriber<T>>(new Subscriber<T>(topic, transport_));
    }

private:
    TransportPtr transport_; ///< Shared transport for all publishers and subscribers
};

} // namespace dds

#endif // DDS_DOMAIN_PARTICIPANT_HPP 