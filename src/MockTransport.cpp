#include "dds/MockTransport.hpp"
#include <stdexcept>

namespace dds {

void MockTransport::send(const AnyTopic& topic, const AnyMessage& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = callbacks_.find(topic.name());
    if (it != callbacks_.end() && it->second) {
        it->second(msg);
    }
}

void MockTransport::set_receive_callback(const AnyTopic& topic, std::function<void(const AnyMessage&)>&& cb) {
    std::lock_guard<std::mutex> lock(mutex_);
    callbacks_[topic.name()] = std::move(cb);
}

// Explicit template instantiations for common types can be added here if needed

} // namespace dds 