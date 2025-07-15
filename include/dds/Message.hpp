#ifndef DDS_MESSAGE_HPP
#define DDS_MESSAGE_HPP

#include <string>
#include <chrono>
#include "dds/Types.hpp"

namespace dds {

/**
 * @brief Message struct holding data, topic, QoS, timestamp, and sequence number.
 */
template<typename T>
struct Message {
    T data;
    std::string topic;
    QoS qos;
    std::chrono::system_clock::time_point timestamp;
    uint64_t sequence_number = 0;
};

} // namespace dds

#endif // DDS_MESSAGE_HPP 