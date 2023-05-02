#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include "ns3/queue.h"
#include "ns3/packet.h"
#include <vector>
#include "filter-elements.h"
#include "filter.h"

using namespace ns3;

class TrafficClass {
public:
  TrafficClass(uint32_t bytes, uint32_t packets, uint32_t maxPackets,
               uint32_t maxBytes, double_t weight, uint32_t priority_level,
               bool isDefault);
  virtual ~TrafficClass();
  void AddFilter(Filter* filter);
  bool match(Ptr<Packet> p);
  void Enqueue(Ptr<Packet> packet);
  Ptr<Packet> Dequeue();

private:
  uint32_t m_packets;
  uint32_t m_maxPackets;
  double_t m_weight;
  uint32_t m_priority_level;
  bool m_isDefault;
  std::queue<Ptr<Packet>> m_queue;
  std::vector<Filter*> m_filters;
};

#endif // TRAFFIC_CLASS_H
