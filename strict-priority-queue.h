#ifndef STRICT_PRIORITY_QUEUE_H
#define STRICT_PRIORITY_QUEUE_H

#include "DiffServ.h"

namespace ns3 {

class StrictPriorityQueue : public DiffServ {
public:
    static TypeId GetTypeId(void);
    StrictPriorityQueue(int num = 1);
    ~StrictPriorityQueue();

    Ptr<Packet> Dequeue(void) override;
    Ptr<const Packet> Peek(void) const override;
    Ptr<Packet> Remove(void) override;
    Ptr<Packet> Schedule() override;
};
}

#endif
