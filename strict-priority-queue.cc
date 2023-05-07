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

bool StrictPriorityQueue::Enqueue(Ptr<Packet> p) {
  // classify packet
  // std::cout << "DiffServ: Enqueue" << std::endl;
  // std::cout << "DiffServ: Enqueue q_class.size(): " << q_class.size() << std::endl;
  uint32_t cls = Classify(p);
  std::cout << "DiffServ: Enqueue Classify(): " << cls << std::endl;
  // enqueue packet to the appropriate class
  if (cls < q_class.size()) {
    std::cout << "StrictPriorityQueue: success: " << cls << std::endl;
    q_class[cls]->Enqueue(p);
    return true;
  } else {
    std::cout << "DiffServ: false: " << cls << std::endl;
    return false;
  }
}

Ptr<Packet> StrictPriorityQueue::Dequeue(void) {
  // loop through all classes
  std::cout << "StrictPriorityQueue: Dequeue" << std::endl;
  for (uint32_t i = 0; i < q_class.size(); i++) {
    // check if class has packets
    if (!q_class[i]->IsEmpty()) {
      return q_class[i]->Dequeue();
    }
  }
  return nullptr;
}

Ptr<Packet> StrictPriorityQueue::Remove(void) {
  // loop through all classes
  for (uint32_t i = 0; i < q_class.size(); i++) {
    // check if class has packets
    if (!q_class[i]->IsEmpty()) {
      return q_class[i]->Dequeue();
    }
  }
  return nullptr;
}

Ptr<const Packet> StrictPriorityQueue::Peek(void) const {
  // loop through all classes
  for (uint32_t i = 0; i < q_class.size(); i++) {
    // check if class has packets
    if (!q_class[i]->IsEmpty()) {
      // peek packet from class
      return q_class[i]->Peek();
    }
  }
  return nullptr;
}

uint32_t StrictPriorityQueue::Classify(Ptr<Packet> p) {
  for (uint32_t i = 0; i < q_class.size(); ++i) {
    // std::cout << "StrictPriorityQueue: Classify TrafficClass" << i << std::endl;
    // std::cout << "StrictPriorityQueue: Classify Filter size" <<q_class[i]->GetFilterSize()<< std::endl;
    if (q_class[i]->match(p)) {
        return i;
      }
    }
    // std::cout << "StrictPriorityQueue: Classify return -1 " << std::endl;
  return -1;
}

Ptr<Packet> StrictPriorityQueue::Schedule() {
  // Use the dequeue method to get the next packet to transmit
  return Dequeue();
}

} // namespace ns3
