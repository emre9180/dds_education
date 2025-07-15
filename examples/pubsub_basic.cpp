#include <dds/DomainParticipant.hpp>
#include <dds/MockTransport.hpp>
#include <dds/Publisher.hpp>
#include <dds/Subscriber.hpp>
#include <dds/Topic.hpp>
#include <dds/Message.hpp>
#include <iostream>
#include <chrono>
#include <thread>

struct MyData {
    int value;
};

int main() {
    using namespace dds;

    // Create a shared MockTransport
    auto transport = std::make_shared<MockTransport>();

    // Create a DomainParticipant with the transport
    DomainParticipant participant(transport);

    // Create a Topic for MyData
    auto topic = std::make_shared<Topic<MyData>>("my_topic");

    // Create a Publisher and Subscriber for the topic
    auto publisher = participant.create_publisher<MyData>(topic);
    auto subscriber = participant.create_subscriber<MyData>(topic);

    // Register a callback for the subscriber
    subscriber->set_callback([](const Message<MyData>& msg) {
        std::cout << "[Subscriber] Received value: " << msg.data.value << std::endl;
        std::cout << "[Subscriber] Topic: " << msg.topic << std::endl;
        std::cout << "[Subscriber] Sequence: " << msg.sequence_number << std::endl;
    });

    // Prepare a message
    Message<MyData> msg;
    msg.data.value = 42;
    msg.topic = "my_topic";
    msg.timestamp = std::chrono::system_clock::now();
    msg.sequence_number = 1;

    // Publish the message
    std::cout << "[Publisher] Publishing value: " << msg.data.value << std::endl;
    publisher->publish(msg);

    // Give time for the callback to execute (not needed for MockTransport, but good practice)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
} 