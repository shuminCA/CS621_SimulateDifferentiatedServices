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

    virtual Ptr<Packet> Schedule(void) = 0;
    virtual uint32_t Classify(Ptr<Packet> p) = 0;
    TrafficClass* GetTrafficClass(int num);
    virtual bool Enqueue(Ptr<Packet> p) = 0;
    virtual Ptr<Packet> Dequeue(void) = 0;
    virtual Ptr<Packet> Remove(void) = 0;
    virtual Ptr<const Packet> Peek(void) const = 0;

protected:
    std::vector<TrafficClass*> q_class;
};

} // namespace ns3

#endif // DIFFSERV_H
