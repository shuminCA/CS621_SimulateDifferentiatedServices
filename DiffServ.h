#ifndef DIFFSERV_H
#define DIFFSERV_H

#include <vector>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "traffic-class.h"
#include "filter.h"

namespace ns3 {

class DiffServ : public Queue<Packet> {
public:
    static TypeId GetTypeId(void);
    explicit DiffServ(int num = 1);
    ~DiffServ();

    uint32_t Classify(Ptr<Packet> p);
    TrafficClass* GetTrafficClass(uint32_t num) const;
    bool Enqueue(Ptr<Packet> p);
    uint32_t GetSize() const;

    virtual Ptr<Packet> Dequeue(void) = 0;
    virtual Ptr<Packet> Remove(void) = 0;
    virtual Ptr<Packet> Schedule(void) = 0;
    virtual Ptr<const Packet> Peek(void) const = 0;

private:
    std::vector<TrafficClass*> q_class;
};
}

#endif
