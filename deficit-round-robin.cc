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

  // Calculate weights and deficits for each traffic class
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

  for (uint32_t i = 0; i < num_classes; ++i) {
    uint32_t class_index = (m_currentClass + i) % num_classes;
    TrafficClass* tc = GetTrafficClass(class_index);

    if (!tc->IsEmpty()) {
      uint32_t packet_size = tc->Peek()->GetSize();
      uint32_t deficit = tc->GetDeficit();

      // If the deficit is greater or equal to the packet size, dequeue the packet
      if (deficit >= packet_size) {
        Ptr<Packet> packet = tc->Dequeue();
        tc->SetDeficit(deficit - packet_size);

        // Update the current class for the next dequeue operation
        m_currentClass = (class_index + 1) % num_classes;
        return packet;
      } else {
        // Increment the deficit by the weight of the traffic class
        tc->SetDeficit(deficit + static_cast<uint32_t>(tc->GetWeight() * m_deficitSize));
      }
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

} // namespace ns3
