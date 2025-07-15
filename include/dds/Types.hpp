#ifndef DDS_TYPES_HPP
#define DDS_TYPES_HPP

#include <string>

namespace dds {

using TopicName = std::string;

/**
 * @brief Quality of Service (QoS) policy placeholder.
 */
struct QoS {
    // Add QoS policy fields here (e.g., reliability, durability)
};

} // namespace dds

#endif // DDS_TYPES_HPP 