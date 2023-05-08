#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "DiffServ.h"

#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include <math.h>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(DiffServ);

TypeId
DiffServ::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DiffServ")
    .SetParent<Queue<Packet>> ()
    .SetGroupName("Network");
  return tid;
}

DiffServ::DiffServ(int num)
{
  for (int i = 0; i < num; i++) {
    TrafficClass* tc = new TrafficClass(i);
    q_class.push_back(tc);
  }
  std::cout << "DiffServ: Create DiffServ" << std::endl;
}

DiffServ::~DiffServ() {
  for (TrafficClass* tc : q_class) {
    delete tc;
  }
}

TrafficClass* DiffServ::GetTrafficClass(uint32_t num) const{
    return q_class[num];
}

uint32_t DiffServ::GetSize() const {
  return q_class.size();
}

bool DiffServ::Enqueue(Ptr<Packet> p) {
  // classify packet
  // std::cout << "DiffServ: Enqueue" << std::endl;
  // std::cout << "DiffServ: Enqueue GetSize(): " << GetSize() << std::endl;
  uint32_t cls = Classify(p);
  std::cout << "DiffServ: Enqueue Classify(): " << cls << std::endl;
  // enqueue packet to the appropriate class
  if (cls < GetSize()) {
    std::cout << "StrictPriorityQueue: success: " << cls << std::endl;
    TrafficClass* tc = GetTrafficClass(cls);
    return tc->Enqueue(p);
  } else {
    std::cout << "DiffServ: false: " << cls << std::endl;
    return false;
  }
}

uint32_t DiffServ::Classify(Ptr<Packet> p) {
  for (uint32_t i = 0; i < GetSize(); ++i) {
    // std::cout << "StrictPriorityQueue: Classify TrafficClass" << i << std::endl;
    // std::cout << "StrictPriorityQueue: Classify Filter size" <<q_class[i]->GetFilterSize()<< std::endl;
    TrafficClass* tc = GetTrafficClass(i);
    if (tc->match(p)) {
        return i;
      }
    }
    // std::cout << "StrictPriorityQueue: Classify return -1 " << std::endl;
  return -1;
}

}