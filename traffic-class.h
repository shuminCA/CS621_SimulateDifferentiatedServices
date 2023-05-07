#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include "ns3/queue.h"
#include "ns3/packet.h"
#include <vector>
#include <queue>
#include "filter-elements.h"
#include "filter.h"

namespace ns3 {

class TrafficClass {
public:
  TrafficClass(uint32_t priority_level, bool isDefault = false, uint32_t packets = 0, uint32_t maxPackets = 10000, double_t weight = 1);
  virtual ~TrafficClass();
  void AddFilter(Filter* filter);
  bool match(Ptr<Packet> p);
  bool Enqueue(Ptr<Packet> packet);
  Ptr<Packet> Dequeue();
  Ptr<Packet> Peek() const;
  bool IsEmpty() const;
  // const std::vector<Filter*>& GetFilters() const;
  uint32_t GetFilterSize();
  uint32_t GetQueueSize();
  
private:
  uint32_t m_packets;
  uint32_t m_maxPackets;
  double_t m_weight;
  uint32_t m_priority_level;
  bool m_isDefault;
  std::queue<Ptr<Packet>> m_queue;
  std::vector<Filter*> m_filters;
};
}

#endif // TRAFFIC_CLASS_H
