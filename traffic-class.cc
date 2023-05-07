#include "traffic-class.h"

namespace ns3 {

TrafficClass::TrafficClass(uint32_t priority_level, bool isDefault, uint32_t packets, uint32_t maxPackets, double_t weight)
    : m_packets(packets), m_maxPackets(maxPackets), m_weight(weight), m_priority_level(priority_level),
      m_isDefault(isDefault) {}

TrafficClass::~TrafficClass() {}

void TrafficClass::AddFilter(Filter* filter) {
  m_filters.push_back(filter);
  std::cout << "TrafficClass: AddFilters() -> size:" << m_filters.size() << std::endl;
}

// const std::vector<Filter*>& TrafficClass::GetFilters() const {
//   std::cout << "TrafficClass: GetFilters() -> size:" << m_filters.size() << std::endl;
//   return m_filters;
// }

bool TrafficClass::match(Ptr<Packet> p) {
    for (auto& filter : m_filters) {
        // std::cout << "TrafficClass: match filter size()" << filter->size() << std::endl;
      if (filter->match(p)) {
        std::cout << "TrafficClass: match false" << std::endl;
        return true;
      }
    }
    std::cout << "TrafficClass: match true" << std::endl;
    return false;
}

bool TrafficClass::Enqueue(Ptr<Packet> packet) {
  if (m_packets < m_maxPackets) {
    m_queue.push(packet);
    m_packets++;
    std::cout << "Traffic Class: Enqueue" << std::endl;
    return true;
  } else {
    return false;
  }
}

bool TrafficClass::IsEmpty() const{
  return m_queue.empty();
}

Ptr<Packet> TrafficClass::Dequeue() {
  if (!m_queue.empty()) {
    Ptr<Packet> packet = m_queue.front();
    m_queue.pop();
    m_packets--;
    return packet;
  }
  return nullptr;
}

Ptr<Packet> TrafficClass::Peek() const{
  if (!m_queue.empty()) {
    Ptr<Packet> packet = m_queue.front();
    return packet;
  }
  return nullptr;
}

uint32_t TrafficClass::GetQueueSize() {
  return m_queue.size();
}

uint32_t TrafficClass::GetFilterSize() {
  return m_filters.size();
}
}
