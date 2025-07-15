#ifndef DDS_MOCKTRANSPORT_HPP
#define DDS_MOCKTRANSPORT_HPP

#include <dds/Transport.hpp>
#include <dds/AnyTopic.hpp>
#include <dds/AnyMessage.hpp>
#include <dds/Topic.hpp>
#include <dds/Message.hpp>
#include <unordered_map>
#include <any>
#include <mutex>
#include <string>
#include <functional>
#include <typeinfo>

namespace dds {

/**
 * @brief MockTransport provides in-process message delivery for testing and development.
 * Uses type-erased callback registry and is thread-safe.
 */
class MockTransport : public Transport {
public:
    MockTransport() = default;
    ~MockTransport() override = default;

    /**
     * @brief Type-erased send implementation (overrides Transport).
     */
    void send(const AnyTopic& topic, const AnyMessage& msg) override;

    /**
     * @brief Type-erased callback registration (overrides Transport).
     */
    void set_receive_callback(const AnyTopic& topic, std::function<void(const AnyMessage&)>&& cb) override;

    /**
     * @brief Template convenience: send a strongly-typed message.
     */
    template<typename T>
    void send(const Topic<T>& topic, const Message<T>& msg) {
        send(AnyTopic(topic), AnyMessage(msg));
    }

    /**
     * @brief Template convenience: register a strongly-typed callback.
     */
    template<typename T>
    void set_receive_callback(const Topic<T>& topic, std::function<void(const Message<T>&)> cb) {
        // Wrap the typed callback in a type-erased lambda
        set_receive_callback(
            AnyTopic(topic),
            [cb, type = std::ref(typeid(Message<T>))](const AnyMessage& any_msg) {
                if (any_msg.type() == type.get()) {
                    cb(any_msg.get<T>());
                }
            }
        );
    }

private:
    std::unordered_map<std::string, std::function<void(const AnyMessage&)>> callbacks_; ///< topic name to type-erased callback
    std::mutex mutex_;
};

} // namespace dds

#endif // DDS_MOCKTRANSPORT_HPP 