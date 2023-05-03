// #include "ns3/log.h"
// #include "strict-priority-queue.h"

// namespace ns3 {

// NS_LOG_COMPONENT_DEFINE("StrictPriorityQueue");

// NS_OBJECT_ENSURE_REGISTERED(StrictPriorityQueue);

// TypeId
// StrictPriorityQueue::GetTypeId(void) {
//     static TypeId tid = TypeId("ns3::StrictPriorityQueue")
//                             .SetParent<DiffServ>()
//                             .SetGroupName("Network")
//                             .AddConstructor<StrictPriorityQueue>();
//     return tid;
// }

// StrictPriorityQueue::StrictPriorityQueue() {
// }

// StrictPriorityQueue::~StrictPriorityQueue() {
// }

// bool StrictPriorityQueue::DoEnqueue(Ptr<Packet> p) {
//     uint32_t cls = Classify(p);

//     if (cls < q_class.size()) {
//         q_class[cls].Enqueue(p);
//         return true;
//     } else {
//         return false;
//     }
// }

// Ptr<Packet> StrictPriorityQueue::DoDequeue(void) {
//     for (uint32_t i = 0; i < q_class.size(); i++) {
//         if (!q_class[i].IsEmpty()) {
//             return q_class[i].Dequeue();
//         }
//     }
//     return 0;
// }

// Ptr<const Packet> StrictPriorityQueue::DoPeek(void) const {
//     for (uint32_t i = 0; i < q_class.size(); i++) {
//         if (!q_class[i].IsEmpty()) {
//             return q_class[i].Peek();
//         }
//     }
//     return 0;
// }

// } // namespace ns3
