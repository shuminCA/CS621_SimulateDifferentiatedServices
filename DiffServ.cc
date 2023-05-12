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
    TrafficClass* tc = new TrafficClass();
    q_class.push_back(tc);
  }
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
  uint32_t cls = Classify(p);
  if (cls < GetSize()) {
    TrafficClass* tc = GetTrafficClass(cls);
    return tc->Enqueue(p);
  } else {
    return false;
  }
}

uint32_t DiffServ::Classify(Ptr<Packet> p) {
  for (uint32_t i = 0; i < GetSize(); ++i) {
    TrafficClass* tc = GetTrafficClass(i);
    if (tc->Match(p)) {
        return i;
      }
    }
  return -1;
}

}