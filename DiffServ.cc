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

TrafficClass* DiffServ::GetTrafficClass(int num){
    return q_class[num];
}


}