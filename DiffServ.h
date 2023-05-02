#ifndef DIFFSERV_H
#define DIFFSERV_H

#include <vector>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "traffic-class.h"

namespace ns3{

class DiffServ: public Queue<Packet> {

public:
    static TypeId GetTypeId (void);
    DiffServ(int num);
    DiffServ();
    ~DiffServ();

    Ptr<Packet> Schedule(void);
    uint32_t Classify(Ptr<Packet> p);

private:
    std::vector<TrafficClass> q_class;
    bool DoEnqueue(Ptr<Packet> p);
    Ptr<Packet> DoDequeue(void);
    Ptr<const Packet> DoPeek(void) const;
};
}

#endif
