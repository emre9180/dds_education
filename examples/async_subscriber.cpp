#include <dds/DomainParticipant.hpp>
#include <dds/MockTransport.hpp>
#include <dds/Publisher.hpp>
#include <dds/Subscriber.hpp>
#include <dds/Topic.hpp>
#include <dds/Message.hpp>
#include <dds/ThreadSafeQueue.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

struct MyData {
    int value;
};

// Async subscriber wrapper using a thread-safe queue and worker thread
class AsyncSubscriber {
public:
    AsyncSubscriber(const std::shared_ptr<dds::Subscriber<MyData>>& sub)
        : subscriber_(sub), stop_flag_(false) {
        // Register a callback that pushes messages into the queue
        subscriber_->set_callback([this](const dds::Message<MyData>& msg) {
            queue_.push(msg);
        });
        // Start the worker thread
        worker_ = std::thread([this] { this->process(); });
    }
    ~AsyncSubscriber() {
        stop_flag_ = true;
        // Push a dummy message to unblock wait_and_pop
        queue_.push(dds::Message<MyData>{});
        if (worker_.joinable()) worker_.join();
    }
    // Worker thread: process messages from the queue
    void process() {
        while (!stop_flag_) {
            dds::Message<MyData> msg = queue_.wait_and_pop();
            if (stop_flag_) break;
            std::cout << "[AsyncSubscriber] Received value: " << msg.data.value << std::endl;
            std::cout << "[AsyncSubscriber] Topic: " << msg.topic << std::endl;
            std::cout << "[AsyncSubscriber] Sequence: " << msg.sequence_number << std::endl;
        }
    }
private:
    std::shared_ptr<dds::Subscriber<MyData>> subscriber_;
    dds::ThreadSafeQueue<dds::Message<MyData>> queue_;
    std::thread worker_;
    std::atomic<bool> stop_flag_;
};

int main() {
    using namespace dds;

    auto transport = std::make_shared<MockTransport>();
    DomainParticipant participant(transport);
    auto topic = std::make_shared<Topic<MyData>>("async_topic");
    auto publisher = participant.create_publisher<MyData>(topic);
    auto subscriber = participant.create_subscriber<MyData>(topic);

    AsyncSubscriber async_sub(subscriber);

    // Publisher sends multiple messages quickly
    for (int i = 1; i <= 5; ++i) {
        Message<MyData> msg;
        msg.data.value = i * 10;
        msg.topic = "async_topic";
        msg.timestamp = std::chrono::system_clock::now();
        msg.sequence_number = i;
        std::cout << "[Publisher] Publishing value: " << msg.data.value << std::endl;
        publisher->publish(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Wait for all messages to be processed
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // AsyncSubscriber destructor will clean up the thread
    return 0;
} 