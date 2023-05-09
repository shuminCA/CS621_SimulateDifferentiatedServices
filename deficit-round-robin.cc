#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "deficit-round-robin.h"
#include <sstream>

namespace ns3 {

TypeId
DeficitRoundRobin::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::DeficitRoundRobin")
                            .SetParent<DiffServ>()
                            .SetGroupName("Network")
                            .AddConstructor<DeficitRoundRobin> ();
    return tid;
}

DeficitRoundRobin::DeficitRoundRobin(int num, std::string quantaRatio, uint32_t deficitSize)
    : DiffServ(num), m_currentClass(0), m_deficitSize(deficitSize) {
  std::vector<double> weights = ParseQuantaRatio(quantaRatio);

  for (int i = 0; i < num; ++i) {
    double weight = weights[i];
    uint32_t deficit = static_cast<uint32_t>(weight * m_deficitSize);
    TrafficClass* tc = GetTrafficClass(i);
    tc->SetWeight(weight);
    tc->SetDeficit(deficit);
  }

}

DeficitRoundRobin::~DeficitRoundRobin() {}

std::vector<double> DeficitRoundRobin::ParseQuantaRatio(std::string quantaRatio) {
    std::istringstream ss(quantaRatio);
    std::string token;
    std::vector<double> weights;

    while (std::getline(ss, token, ':')) {
        uint32_t quantum = std::stoi(token);
        weights.push_back(quantum);
    }
    return weights;
}

Ptr<Packet> DeficitRoundRobin::Schedule(void) {
    return Dequeue();
}

Ptr<Packet> DeficitRoundRobin::Dequeue(void) {
  uint32_t num_classes = GetSize();
  bool dequeued = false;
  while (!dequeued){
    bool allEmpty = true;
    for (uint32_t i = 0; i < num_classes; ++i) {
        uint32_t class_index = (m_currentClass + i) % num_classes;
        TrafficClass* tc = GetTrafficClass(class_index);
        if (!tc->IsEmpty()) {
        allEmpty = false;
        uint32_t packet_size = tc->Peek()->GetSize();
        uint32_t deficit = tc->GetDeficit();
            if (deficit >= packet_size) {
                Ptr<Packet> packet = tc->Dequeue();
                tc->SetDeficit(deficit - packet_size);
                m_currentClass = (class_index + 1) % num_classes;
                dequeued = true;
                return packet;
            } else {
                tc->SetDeficit(deficit + static_cast<uint32_t>(tc->GetWeight() * m_deficitSize));
            }
        } 
    }
    if (allEmpty){
        break;
    }
  }
  return nullptr;
}


Ptr<Packet> DeficitRoundRobin::Remove(void) {
    return Dequeue();
}

Ptr<const Packet> DeficitRoundRobin::Peek(void) const {
    for (uint32_t i = 0; i < GetSize(); i++) {
    TrafficClass* tc = GetTrafficClass(i);
    if (!tc->IsEmpty()) {
      return tc->Peek();
    }
  }
  return nullptr;
}
}
