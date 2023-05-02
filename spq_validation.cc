#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"
#include "strict-priority-queue.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("StrictPriorityQueueExample");

int main (int argc, char *argv[])
{
    CommandLine cmd (__FILE__);
    cmd.Parse (argc, argv);

    Time::SetResolution (Time::NS);

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Create three nodes
    NodeContainer nodes;
    nodes.Create (3);

    // Create two PointToPoint channels with different data rates;
    PointToPointHelper potinToPoint, pointToPoint;
    pointToPointA.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
    pointToPointA.SetChannelAttribute ("Delay", StringValue ("2ms"));
    pointToPointB.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
    pointToPointB.SetChannelAttribute ("Delay", StringValue ("2ms"));
    
    // Create devices and install the channel on the nodes
    NetDeviceContainer devicesA, devicesB;
    devicesA = pointToPointA.Install (nodes.Get(0), nodes.Get(1));
    devicesB = pointToPointB.Install (nodes.Get(1), nodes.Get(2));

    // Install the Internet stack and assign IPv4 addresses
    InternetStackHelper stack;
    stack.Install (nodes);

    Ipv4AddressHelper addressA, addressB;
    addressA.SetBase("10.1.1.0", "255.255.255.0");
    addressB.SetBase("10.1.2.0", "255.255.255.0");

    Ipv4InterfaceContainer interfacesA, interfacesB;
    interfacesA = addressA.Assign (devicesA);
    interfacesB = addressB.Assign (devicesB);
    
    // Enable QoS on the middle node
    /*-----------------------------------------------------*/
    TrafficControlHelper tch;
    tch.SetRootQueueDisc("ns3::StrictPriorityQueue");
    tch.Install(devicesA.Get(1));
    tch.Install(devicesB.Get(0));

    // Application A
    uint16_t portA = 50000;
    uint32_t maxBytes = 0;
    BulkSendHelper sourceA ("ns3:TcpSocketFactory", InetSocketAddress (interfacesA.GetAddress(1), portA));
    sourceA.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
    ApplicationContainer sourceAppsA = sourceA.Install (nodes.Get(0));
    sourceAppsA.Start (Seconds (3.0));
    sourceAppsA.Stop (Seconds (12.0));

    PacketSinkHelper sinkA ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny(), portA));
    ApplicationContainer sinkAppsA = sinkA.Install (node.Get(1));
    sinkA.Start (Seconds (1.0));
    sinkA.Stop (Seconds (15.0));

    // Application B
    uint16_t portB = 50001;
    BulkSendHelper sourceB ("ns3::TcpSocketFactory", InetSocketAddress (interfacesA.GetAddress (1), portB));
    sourceB.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
        ApplicationContainer sourceAppsB = sourceB.Install (nodes.Get (0));
    sourceAppsB.Start (Seconds (1.0));
    sourceAppsB.Stop (Seconds (10.0));

    PacketSinkHelper sinkB ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), portB));
    ApplicationContainer sinkAppsB = sinkB.Install (nodes.Get (1));
    sinkAppsB.Start (Seconds (1.0));
    sinkAppsB.Stop (Seconds (15.0));

    // Enable generating pcap files
    pointToPointA.EnablePcapAll("three-node-topology-A");
    pointToPointB.EnablePcapAll("three-node-topology-B");

    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}