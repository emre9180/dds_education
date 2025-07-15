#ifndef DDS_TRANSPORT_HPP
#define DDS_TRANSPORT_HPP

#include <memory>
#include <string>
#include <functional>
#include "dds/Types.hpp"

namespace dds {

template<typename T>
struct Message;

template<typename T>
class Topic;

/**
 * @brief Abstract Transport interface for sending/receiving messages.
 */
class Transport {
public:
    virtual ~Transport() = default;

    template<typename T>
    using ReceiveCallback = std::function<void(const Message<T>&)>;

    /**
     * @brief Send a message to a topic.
     */
    template<typename T>
    virtual void send(const Topic<T>& topic, const Message<T>& msg) = 0;

    /**
     * @brief Register a receive callback for a topic.
     */
    template<typename T>
    virtual void set_receive_callback(const Topic<T>& topic, ReceiveCallback<T> cb) = 0;
};

/**
 * @brief MockTransport for testing (no real network, just local delivery).
 */
class MockTransport : public Transport {
public:
    MockTransport() = default;
    ~MockTransport() override = default;

    template<typename T>
    void send(const Topic<T>& topic, const Message<T>& msg) override {
        // Immediately deliver to registered callback if exists
        if (auto it = callbacks_.find(topic.name()); it != callbacks_.end()) {
            auto cb = std::any_cast<ReceiveCallback<T>>(it->second);
            if (cb) cb(msg);
        }
    }

    template<typename T>
    void set_receive_callback(const Topic<T>& topic, ReceiveCallback<T> cb) override {
        callbacks_[topic.name()] = cb;
    }

private:
    std::unordered_map<std::string, std::any> callbacks_;
};

} // namespace dds

#endif // DDS_TRANSPORT_HPP 