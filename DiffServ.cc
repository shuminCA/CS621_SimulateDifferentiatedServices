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

NS_LOG_COMPONENT_DEFINE("DiffServ");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(DiffServ);

TypeId DiffServ::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DiffServ")
    .SetParent<Queue<Packet>> ()
    .SetGroupName("Network")
    .AddConstructor<DiffServ> ()
  ;
  return tid;
}


DiffServ::DiffServ(int num)
{
  for (int i = 0; i < num; i++) {
    TrafficClass tc;
    q_class.push_back(tc);
  }
}

DiffServ::~DiffServ() {}

bool DiffServ::DoEnqueue(Ptr<Packet> p) {
  // classify packet
  uint32_t cls = Classify(p);

  // enqueue packet to the appropriate class
  if (cls < q_class.size()) {
    q_class[cls].Enqueue(p);
    return true;
  } else {
    return false;
  }
}

Ptr<Packet> DiffServ::DoDequeue(void) {
  // loop through all classes
  for (uint32_t i = 0; i < q_class.size(); i++) {
    // check if class has packets
    if (!q_class[i].IsEmpty()) {
      return q_class[i].Dequeue();
    }
  }
  return nullptr;
}

Ptr<const Packet> DiffServ::DoPeek(void) const {
  // loop through all classes
  for (uint32_t i = 0; i < q_class.size(); i++) {
    // check if class has packets
    if (!q_class[i].IsEmpty()) {
      // peek packet from class
      return q_class[i].Peek();
    }
  }
  return nullptr;
}

uint32_t DiffServ::Classify(Ptr<Packet> p) {
  NS_LOG_FUNCTION(this << p);
  Ipv4Header ip;

  for (uint32_t i = 0; i < m_classes.size(); ++i) {
    for (auto& filter : m_classes[i].filters) {
      if (filter->match(p)) {
        return i;
      }
    }
  }
  return -1;
}

Ptr<Packet> DiffServ::Schedule() {
  // Use the dequeue method to get the next packet to transmit
  return DoDequeue();
}
}