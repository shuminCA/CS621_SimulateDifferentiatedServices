#ifndef STRICT_PRIORITY_QUEUE_H
#define STRICT_PRIORITY_QUEUE_H

#include "DiffServ.h"

namespace ns3 {

class StrictPriorityQueue : public DiffServ {
public:
    static TypeId GetTypeId(void);
    StrictPriorityQueue(int num = 1);
    ~StrictPriorityQueue();

    bool Enqueue(Ptr<Packet> p) override;
    Ptr<Packet> Dequeue(void) override;
    Ptr<const Packet> Peek(void) const override;
    Ptr<Packet> Remove(void) override;
    uint32_t Classify(Ptr<Packet> p) override;
    Ptr<Packet> Schedule() override;

};

} // namespace ns3

#endif // STRICT_PRIORITY_QUEUE_H
