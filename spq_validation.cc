// #include "ns3/core-module.h"
// #include "ns3/network-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/point-to-point-module.h"
// #include "ns3/applications-module.h"
// #include "ns3/traffic-control-module.h"
// #include "strict-priority-queue.h"

// using namespace ns3;

// NS_LOG_COMPONENT_DEFINE("StrictPriorityQueueExample");

// int main (int argc, char *argv[])
// {
//     CommandLine cmd (__FILE__);
//     cmd.Parse (argc, argv);

//     Time::SetResolution (Time::NS);

//     LogComponentEnable ("BulkSendApplication", LOG_LEVEL_INFO);
//     LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);

//     // Create three nodes
//     NodeContainer nodes;
//     nodes.Create (3);

//     // Create two PointToPoint channels with different data rates;
//     PointToPointHelper potinToPointA, pointToPointB;
//     pointToPointA.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
//     pointToPointA.SetChannelAttribute ("Delay", StringValue ("2ms"));
//     pointToPointB.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
//     pointToPointB.SetChannelAttribute ("Delay", StringValue ("2ms"));
    
//     // Create devices and install the channel on the nodes
//     NetDeviceContainer devicesA, devicesB;
//     devicesA = pointToPointA.Install (nodes.Get(0), nodes.Get(1));
//     devicesB = pointToPointB.Install (nodes.Get(1), nodes.Get(2));

//     // Install the Internet stack and assign IPv4 addresses
//     InternetStackHelper stack;
//     stack.Install (nodes);

//     Ipv4AddressHelper addressA, addressB;
//     addressA.SetBase("10.1.1.0", "255.255.255.0");
//     addressB.SetBase("10.1.2.0", "255.255.255.0");

//     Ipv4InterfaceContainer interfacesA, interfacesB;
//     interfacesA = addressA.Assign (devicesA);
//     interfacesB = addressB.Assign (devicesB);
    
//     // Set up the SPQ on node 1
//     /*-----------------------------------------------------*/
//     Ptr<StrictPriorityQueue> spq = CreateObject<StrictPriorityQueue>(2);
//     nodes.Get(1)->GetDevice(0)->GetObject<PointToPointNetDevice>()->SetQueue(spq);
//     nodes.Get(1)->GetDevice(1)->GetObject<PointToPointNetDevice>()->SetQueue(spq);

//     // Set up the server application on node 2
//     uint16_t portServer = 9;
//     PacketSinkHelper server ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), portServer));
//     ApplicationContainer serverApp = server.Install (nodes.Get (2));
//     serverApp.Start (Seconds (1.0));
//     serverApp.Stop (Seconds (10.0));

//     // Set up the client application on node 0
//     uint16_t portA = 50000;
//     BulkSendHelper clientA ("ns3:TcpSocketFactory", InetSocketAddress (interfacesB.GetAddress(1), portA));
//     clientA.SetAttribute ("MaxBytes", UintegerValue (0));
//     ApplicationContainer clientAppA = clientA.Install (nodes.Get(0));
//     clientAppA.Start (Seconds (2.5));
//     clientAppA.Stop (Seconds (10.0));

//     uint16_t portB = 50001;
//     BulkSendHelper clientB ("ns3::TcpSocketFactory", InetSocketAddress (interfacesB.GetAddress (1), portB));
//     clientB.SetAttribute ("MaxBytes", UintegerValue (0));
//         ApplicationContainer clientAppsB = clientB.Install (nodes.Get (0));
//     clientAppB.Start (Seconds (2.0));
//     clientAppB.Stop (Seconds (10.0));



//     // Enable generating pcap files
//     pointToPointA.EnablePcapAll("three-node-topology-A");
//     pointToPointB.EnablePcapAll("three-node-topology-B");

//     Simulator::Run ();
//     Simulator::Destroy ();

//     return 0;
// }