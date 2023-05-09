#include "deficit-round-robin.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/traffic-control-module.h"
#include "json.hpp"
#include "drr_validation.h"
#include <fstream>

using namespace ns3;
using json = nlohmann::json;

void readFileDRR(std::string filename, uint32_t &queue_num, std::string &quantaRatio,uint16_t &dest_portA, uint16_t &dest_portB, uint16_t &dest_portC) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        exit(1);
    }

    json config;
    try {
        file >> config;
    } catch (const json::exception &e) {
        std::cerr << "Error: JSON parsing error: " << e.what() << std::endl;
        exit(1);
    }

    queue_num = config["queue_num"];
    quantaRatio = config["quanta_ratio"];
    dest_portA = config["dest_port"][0];
    dest_portB = config["dest_port"][1];
    dest_portC = config["dest_port"][2];
}

void DRRValidation(){
    uint32_t queue_num;
    std::string quantaRatio;
    uint16_t portA;
    uint16_t portB;
    uint16_t portC;

    readFileDRR("scratch/cs621_QoS/drr_config.json", queue_num, quantaRatio, portA, portB, portC);

    uint32_t maxPacketCount = 20000;
    
    double SERVER_START_TIME = 1.0;
    double SERVER_END_TIME = 30.0;
    double CLIENT_START_TIME = 2.0;
    double CLIENT_END_TIME = 30.0;
    double PACKET_INTERVAL = 0.001;

    std::string DATARATEA = "40Mbps";
    std::string DATARATEB = "10Mbps";
    std::string DEFAULT_DELAY = "2ms";
    Ipv4Mask IPV4_MASK = "255.255.255.0";
    Ipv4Address IPV4_ADDRA = "10.1.1.0";
    Ipv4Address IPV4_ADDRB = "10.1.2.0";

    Time::SetResolution(Time::NS);

    // Create three nodes
    NodeContainer nodes;
    nodes.Create(3);

    // Create two PointToPoint channels with different data rates;
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue(DATARATEA));
    pointToPoint.SetChannelAttribute("Delay", StringValue(DEFAULT_DELAY));

    // Create devices and install the channel on the nodes
    NetDeviceContainer devicesA;
    devicesA = pointToPoint.Install(nodes.Get(0), nodes.Get(1));

    NetDeviceContainer devicesB;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue(DATARATEB));
    pointToPoint.SetChannelAttribute("Delay", StringValue(DEFAULT_DELAY));
    devicesB = pointToPoint.Install(nodes.Get(1), nodes.Get(2));

    // Install the Internet stack and assign IPv4 addresses
    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper addressA;
    addressA.SetBase(IPV4_ADDRA, IPV4_MASK);

    Ipv4InterfaceContainer interfacesA;
    interfacesA = addressA.Assign(devicesA);

    Ipv4InterfaceContainer interfacesB;
    addressA.SetBase(IPV4_ADDRB, IPV4_MASK);
    interfacesB = addressA.Assign(devicesB);

    /*----------------customized drr starts----------------*/

    Ptr<DeficitRoundRobin> drr = CreateObject<DeficitRoundRobin>(queue_num, quantaRatio);
    Filter* p0_filterB = new Filter();
    p0_filterB->AddFilterElement(new DestinationPortNumber(portA));
    drr->GetTrafficClass(0)->AddFilter(p0_filterB);

    Filter* p1_filterB = new Filter();
    p1_filterB->AddFilterElement(new DestinationPortNumber(portB));
    drr->GetTrafficClass(1)->AddFilter(p1_filterB);

    Filter* p2_filterB = new Filter();
    p2_filterB->AddFilterElement(new DestinationPortNumber(portC));
    drr->GetTrafficClass(2)->AddFilter(p2_filterB);

    nodes.Get(1)->GetDevice(1)->GetObject<PointToPointNetDevice>()->SetQueue(drr);

    /*----------------customized drr ends----------------*/

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    /*---------------------UDP starts------------------*/

    // Set up the server application on node 2
    UdpServerHelper serverA(portA);
    ApplicationContainer serverAppA = serverA.Install(nodes.Get(2));
    serverAppA.Start(Seconds(SERVER_START_TIME));
    serverAppA.Stop(Seconds(SERVER_END_TIME));

    UdpServerHelper serverB(portB);
    ApplicationContainer serverAppB = serverB.Install(nodes.Get(2));
    serverAppB.Start(Seconds(SERVER_START_TIME));
    serverAppB.Stop(Seconds(SERVER_END_TIME));

    UdpServerHelper serverC(portC);
    ApplicationContainer serverAppC = serverC.Install(nodes.Get(2));
    serverAppC.Start(Seconds(SERVER_START_TIME));
    serverAppC.Stop(Seconds(SERVER_END_TIME));

    // Set up the client application on node 0
    // Client A
    Time interPacketInterval = Seconds(PACKET_INTERVAL);
    UdpClientHelper clientA(interfacesB.GetAddress(1), portA);
    clientA.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    clientA.SetAttribute("Interval", TimeValue(interPacketInterval));
    clientA.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientAppA = clientA.Install(nodes.Get(0));
    clientAppA.Start(Seconds(CLIENT_START_TIME));
    clientAppA.Stop(Seconds(CLIENT_END_TIME));

    // Client B
    UdpClientHelper clientB(interfacesB.GetAddress(1), portB);
    clientB.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    clientB.SetAttribute("Interval", TimeValue(interPacketInterval));
    clientB.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientAppB = clientB.Install(nodes.Get(0));
    clientAppB.Start(Seconds(CLIENT_START_TIME));
    clientAppB.Stop(Seconds(CLIENT_END_TIME));

    // Client C
    UdpClientHelper clientC(interfacesB.GetAddress(1), portC);
    clientC.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    clientC.SetAttribute("Interval", TimeValue(interPacketInterval));
    clientC.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientAppC = clientC.Install(nodes.Get(0));
    clientAppC.Start(Seconds(CLIENT_START_TIME));
    clientAppC.Stop(Seconds(CLIENT_END_TIME));

    // Enable generating pcap files
    pointToPoint.EnablePcapAll("scratch/cs621_QoS/pcap_drr/drr-3nodes");

    Simulator::Run();
    Simulator::Destroy();
}