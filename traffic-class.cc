#include "traffic-class.h"

using namespace ns3;

TrafficClass::TrafficClass(uint32_t packets, uint32_t maxPackets, double_t weight, uint32_t priority_level,
                           bool isDefault)
    : m_packets(packets), m_maxPackets(maxPackets), m_weight(weight), m_priority_level(priority_level),
      m_isDefault(isDefault) {}

TrafficClass::~TrafficClass() {}

void TrafficClass::AddFilter(Filter* filter) {
  m_filters.push_back(filter);
}

bool TrafficClass::match(Ptr<Packet> p) {
  for (auto& filter : m_filters) {
    if (!filter->match(p)) {
      return false;
    }
  }
  return true;
}

void TrafficClass::Enqueue(Ptr<Packet> packet) {
  if (m_packets < m_maxPackets) {
    m_queue.push(packet);
    m_packets++;
  } else {
    NS_LOG_WARN("TrafficClass Queue is Full");
  }
}

bool TrafficClass::IsEmpty(Ptr<Packet> packet) {
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

Ptr<Packet> TrafficClass::Peek() {
  if (!m_queue.empty()) {
    Ptr<Packet> packet = m_queue.front();
    m_packets--;
    return packet;
  }
  return nullptr;
}
