#include "DiffServ.h"
#include "strict-priority-queue.h"

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);

    // LogComponentEnable ("BulkSendApplication", LOG_LEVEL_INFO);
    // LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
    // LogComponentEnable ("PointToPointNetDevice", LOG_LEVEL_INFO);

    uint16_t portA = 8080;
    uint16_t portB = 8081;
    // uint16_t portServer = 8090;
    uint32_t maxPacketCount = 20000;

    // Create three nodes
    NodeContainer nodes;
    nodes.Create(3);

    // Create two PointToPoint channels with different data rates;
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("20Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    // Create devices and install the channel on the nodes
    NetDeviceContainer devicesA;
    devicesA = pointToPoint.Install(nodes.Get(0), nodes.Get(1));

    NetDeviceContainer devicesB;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    devicesB = pointToPoint.Install(nodes.Get(1), nodes.Get(2));

    // Install the Internet stack and assign IPv4 addresses
    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper addressA;
    addressA.SetBase("10.1.1.0", "255.255.255.0");

    Ipv4InterfaceContainer interfacesA;
    interfacesA = addressA.Assign(devicesA);

    Ipv4InterfaceContainer interfacesB;
    addressA.SetBase("10.1.2.0", "255.255.255.0");
    interfacesB = addressA.Assign(devicesB);

    /*----------------customized spq starts----------------*/

    Ptr<StrictPriorityQueue> spq = CreateObject<StrictPriorityQueue>(2);
    Filter* p0_filterA = new Filter();
    // p0_filterA->AddFilterElement(new DestPortNo(portServer));
    p0_filterA->AddFilterElement(new SrcPortNo(portA));
    Filter* p0_filterB = new Filter();
    p0_filterB->AddFilterElement(new DestPortNo(portA));
    // p0_filterB->AddFilterElement(new SrcPortNo(portServer));
    spq->GetTrafficClass(0)->AddFilter(p0_filterA);
    spq->GetTrafficClass(0)->AddFilter(p0_filterB);
    std::cout << "Main: 0 filter size: " << spq->GetTrafficClass(0)->GetFilterSize() << std::endl;

    Filter* p1_filterA = new Filter();
    // p1_filterA->AddFilterElement(new DestPortNo(portServer));
    p1_filterA->AddFilterElement(new SrcPortNo(portB));
    Filter* p1_filterB = new Filter();
    p1_filterB->AddFilterElement(new DestPortNo(portB));
    // p1_filterB->AddFilterElement(new SrcPortNo(portServer));
    spq->GetTrafficClass(1)->AddFilter(p1_filterA);
    spq->GetTrafficClass(1)->AddFilter(p1_filterB);
    std::cout << "Main: 1 filter size: " << spq->GetTrafficClass(1)->GetFilterSize() << std::endl;

    nodes.Get(1)->GetDevice(0)->GetObject<PointToPointNetDevice>()->SetQueue(spq);
    nodes.Get(1)->GetDevice(1)->GetObject<PointToPointNetDevice>()->SetQueue(spq);

    /*----------------customized spq ends----------------*/

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    /*---------------------UDP starts------------------*/

    // Set up the server application on node 2
    UdpServerHelper serverA(portA);
    ApplicationContainer serverAppA = serverA.Install(nodes.Get(2));
    serverAppA.Start(Seconds(1.0));
    serverAppA.Stop(Seconds(15.0));

    UdpServerHelper serverB(portB);
    ApplicationContainer serverAppB = serverB.Install(nodes.Get(2));
    serverAppB.Start(Seconds(1.0));
    serverAppB.Stop(Seconds(30.0));

    // Set up the client application on node 0
    Time interPacketIntervalA = Seconds(0.0003);
    UdpClientHelper clientA(interfacesB.GetAddress(1), portA);
    clientA.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    clientA.SetAttribute("Interval", TimeValue(interPacketIntervalA));
    clientA.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientAppA = clientA.Install(nodes.Get(0));
    clientAppA.Start(Seconds(4.0));
    clientAppA.Stop(Seconds(30.0));

    Time interPacketIntervalB = Seconds(0.0003);
    UdpClientHelper clientB(interfacesB.GetAddress(1), portB);
    clientB.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    clientB.SetAttribute("Interval", TimeValue(interPacketIntervalB));
    clientB.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientAppB = clientB.Install(nodes.Get(0));
    clientAppB.Start(Seconds(2.0));
    clientAppB.Stop(Seconds(30.0));

    /*---------------------UDP ends------------------*/

    // /*-----------------Tcp Starts/ Do not touch ----------------*/
    // // Set up the server application on node 2
    // PacketSinkHelper serverA ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (),
    // portA)); ApplicationContainer serverAppA = serverA.Install (nodes.Get (2)); serverAppA.Start
    // (Seconds (1.0)); serverAppA.Stop (Seconds (15.0));

    // // Set up the server application on node 2
    // PacketSinkHelper serverB ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (),
    // portB)); ApplicationContainer serverAppB = serverB.Install (nodes.Get (2)); serverAppB.Start
    // (Seconds (1.0)); serverAppB.Stop (Seconds (15.0));

    // // Set up the client application on node 0
    // BulkSendHelper clientA ("ns3::TcpSocketFactory", InetSocketAddress
    // (interfacesB.GetAddress(1), portA)); clientA.SetAttribute ("MaxBytes", UintegerValue (0));
    // clientA.SetAttribute("Local", AddressValue(InetSocketAddress(Ipv4Address::GetAny(), portA)));
    // ApplicationContainer clientAppA = clientA.Install (nodes.Get(0));
    // clientAppA.Start (Seconds (2.5));
    // clientAppA.Stop (Seconds (15.0));

    // BulkSendHelper clientB ("ns3::TcpSocketFactory", InetSocketAddress (interfacesB.GetAddress
    // (1), portB)); clientB.SetAttribute ("MaxBytes", UintegerValue (0));
    // clientB.SetAttribute("Local", AddressValue(InetSocketAddress(Ipv4Address::GetAny(), portB)));
    // ApplicationContainer clientAppB = clientB.Install (nodes.Get (0));
    // clientAppB.Start (Seconds (2.0));
    // clientAppB.Stop (Seconds (15.0));

    /*-----------------Tcp ends/ Do not touch ----------------*/

    // Enable generating pcap files
    pointToPoint.EnablePcapAll("scratch/cs621_QoS/pcap/three-node-topology-A");
    // pointToPointB.EnablePcapAll("scratch/cs621_QoS/pcap/three-node-topology-B");

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}