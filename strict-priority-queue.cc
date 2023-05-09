#include "ns3/log.h"
#include "strict-priority-queue.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("StrictPriorityQueue");

NS_OBJECT_ENSURE_REGISTERED(StrictPriorityQueue);

TypeId
StrictPriorityQueue::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::StrictPriorityQueue")
                            .SetParent<DiffServ>()
                            .SetGroupName("Network")
                            .AddConstructor<StrictPriorityQueue>();
    return tid;
}

StrictPriorityQueue::StrictPriorityQueue(int num): DiffServ(num) {
}

StrictPriorityQueue::~StrictPriorityQueue() {
}

Ptr<Packet> StrictPriorityQueue::Dequeue(void) {
  for (uint32_t i = 0; i < GetSize(); i++) {
    TrafficClass* tc = GetTrafficClass(i);
    if (!tc->IsEmpty()) {
      return tc->Dequeue();
    }
  }
  return nullptr;
}

Ptr<Packet> StrictPriorityQueue::Remove(void) {
  return Dequeue();
}

Ptr<const Packet> StrictPriorityQueue::Peek(void) const {
  for (uint32_t i = 0; i < GetSize(); i++) {
    TrafficClass* tc = GetTrafficClass(i);
    if (!tc->IsEmpty()) {
      return tc->Peek();
    }
  }
  return nullptr;
}

Ptr<Packet> StrictPriorityQueue::Schedule() {
  return Dequeue();
}
}
