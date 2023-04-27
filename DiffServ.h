#ifndef DIFFSERV_H
#define DIFFSERV_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"

namespace ns3{

class DiffServ: public Queue<Packet> {

public:
    DiffServ();
    ~DiffServ();

    void SetMode(int mode);
    int GetMode(void);

    Ptr<Packet> Schedule(void);
    uint32_t Classify(Ptr<Packet> p);

private:
    // byte mode: return 0; packet mode: return 1;

    uint8_t m_mode;
    // vector<TrafficClass> q_class;
    bool DoEnqueue(Ptr<Packet> p);
    Ptr<Packet> DoDequeue(void);
    Ptr<const Packet> DoPeek(void) const;
    Ptr<Packet> DoRemove;
};

}

#endif
