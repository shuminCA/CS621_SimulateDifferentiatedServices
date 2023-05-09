#ifndef DEFICIT_ROUND_ROBIN_H
#define DEFICIT_ROUND_ROBIN_H

#include <vector>
#include "ns3/packet.h"
#include "DiffServ.h"

namespace ns3 {

class DeficitRoundRobin : public DiffServ {
public:
    static TypeId GetTypeId(void);
    DeficitRoundRobin(int num = 1, std::string quantaRatio = "1", uint32_t deficitSize = 100);
    ~DeficitRoundRobin();

    Ptr<Packet> Dequeue(void) override;
    Ptr<const Packet> Peek(void) const override;
    Ptr<Packet> Remove(void) override;
    Ptr<Packet> Schedule() override;

private:
    std::vector<double> ParseQuantaRatio(std::string quantaRatio);
    uint32_t m_currentClass;
    uint32_t m_deficitSize;
};
}

#endif
