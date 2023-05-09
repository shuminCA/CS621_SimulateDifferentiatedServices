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

private:
  uint32_t m_packets;
  uint32_t m_maxPackets;
  double_t m_weight;
  uint32_t m_deficit;
  std::queue<Ptr<Packet>> m_queue;
  std::vector<Filter*> m_filters;

public:
  TrafficClass(uint32_t packets = 0, uint32_t maxPackets = 10000);
  virtual ~TrafficClass();
  bool Enqueue(Ptr<Packet> packet);
  Ptr<Packet> Dequeue();
  Ptr<Packet> Peek() const;
  void AddFilter(Filter* filter);
  bool Match(Ptr<Packet> p);
  bool IsEmpty() const;
  uint32_t GetFilterSize();
  uint32_t GetQueueSize();
  void SetDeficit(uint32_t deficit);
  void SetWeight(double weight);
  uint32_t GetDeficit() const;
  double GetWeight() const;
};
}

#endif // TRAFFIC_CLASS_H
