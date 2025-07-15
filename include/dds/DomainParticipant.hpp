#ifndef DDS_DOMAIN_PARTICIPANT_HPP
#define DDS_DOMAIN_PARTICIPANT_HPP

#include <memory>
#include <string>
#include "dds/Publisher.hpp"
#include "dds/Subscriber.hpp"
#include "dds/Topic.hpp"
#include "dds/Transport.hpp"

namespace dds {

class DomainParticipant {
public:
    using TransportPtr = std::shared_ptr<Transport>;

    DomainParticipant(const TransportPtr& transport) : transport_(transport) {}
    virtual ~DomainParticipant() = default;

    template<typename T>
    std::shared_ptr<Publisher<T>> create_publisher(const std::shared_ptr<Topic<T>>& topic) {
        return std::make_shared<Publisher<T>>(topic, transport_);
    }

    template<typename T>
    std::shared_ptr<Subscriber<T>> create_subscriber(const std::shared_ptr<Topic<T>>& topic) {
        return std::make_shared<Subscriber<T>>(topic, transport_);
    }

private:
    TransportPtr transport_;
};

} // namespace dds

#endif // DDS_DOMAIN_PARTICIPANT_HPP 