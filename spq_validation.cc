// #include "strict-priority-queue.h"
// #include "ns3/applications-module.h"
// #include "ns3/core-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/network-module.h"
// #include "ns3/point-to-point-module.h"
// #include "ns3/traffic-control-module.h"
// #include "json.hpp"
// #include <fstream>

// using namespace ns3;
// using json = nlohmann::json;

// void readFile(std::string filename, uint32_t &queue_num, uint16_t &dest_portA, uint16_t &dest_portB) {
//     std::ifstream file(filename);

//     if (!file.is_open()) {
//         std::cerr << "Error: Unable to open file: " << filename << std::endl;
//         exit(1);
//     }

//     json config;
//     try {
//         file >> config;
//     } catch (const json::exception &e) {
//         std::cerr << "Error: JSON parsing error: " << e.what() << std::endl;
//         exit(1);
//     }

//     queue_num = config["queue_num"];
//     dest_portA = config["queue_info"][0]["dest_port"];
//     dest_portB = config["queue_info"][1]["dest_port"];
// }

// int
// main(int argc, char* argv[])
// {
//     uint16_t portA;
//     uint16_t portB;
//     uint32_t queue_num;

//     readFile("scratch/cs621_QoS/spq_config.json", queue_num, portA, portB);

//     uint32_t maxPacketCount = 20000;
    
//     double SERVER_START_TIME = 1.0;
//     double SERVER_END_TIME = 30.0;
//     double CLIENTA_START_TIME = 4.0;
//     double CLIENTA_END_TIME = 30.0;
//     double CLIENTB_START_TIME = 2.0;
//     double CLIENTB_END_TIME = 30.0;
//     double PACKET_INTERVAL = 0.0003;

//     std::string DATARATEA = "20Mbps";
//     std::string DATARATEB = "10Mbps";
//     std::string DEFAULT_DELAY = "2ms";
//     Ipv4Mask IPV4_MASK = "255.255.255.0";
//     Ipv4Address IPV4_ADDRA = "10.1.1.0";
//     Ipv4Address IPV4_ADDRB = "10.1.2.0";

//     CommandLine cmd(__FILE__);
//     cmd.Parse(argc, argv);

//     Time::SetResolution(Time::NS);

//     // LogComponentEnable ("BulkSendApplication", LOG_LEVEL_INFO);
//     // LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
//     // LogComponentEnable ("PointToPointNetDevice", LOG_LEVEL_INFO);

//     // Create three nodes
//     NodeContainer nodes;
//     nodes.Create(3);

//     // Create two PointToPoint channels with different data rates;
//     PointToPointHelper pointToPoint;
//     pointToPoint.SetDeviceAttribute("DataRate", StringValue(DATARATEA));
//     pointToPoint.SetChannelAttribute("Delay", StringValue(DEFAULT_DELAY));

//     // Create devices and install the channel on the nodes
//     NetDeviceContainer devicesA;
//     devicesA = pointToPoint.Install(nodes.Get(0), nodes.Get(1));

//     NetDeviceContainer devicesB;
//     pointToPoint.SetDeviceAttribute("DataRate", StringValue(DATARATEB));
//     pointToPoint.SetChannelAttribute("Delay", StringValue(DEFAULT_DELAY));
//     devicesB = pointToPoint.Install(nodes.Get(1), nodes.Get(2));

//     // Install the Internet stack and assign IPv4 addresses
//     InternetStackHelper stack;
//     stack.Install(nodes);

//     Ipv4AddressHelper addressA;
//     addressA.SetBase(IPV4_ADDRA, IPV4_MASK);

//     Ipv4InterfaceContainer interfacesA;
//     interfacesA = addressA.Assign(devicesA);

//     Ipv4InterfaceContainer interfacesB;
//     addressA.SetBase(IPV4_ADDRB, IPV4_MASK);
//     interfacesB = addressA.Assign(devicesB);

//     /*----------------customized spq starts----------------*/

//     Ptr<StrictPriorityQueue> spq = CreateObject<StrictPriorityQueue>(queue_num);
//     Filter* p0_filterA = new Filter();
//     // p0_filterA->AddFilterElement(new DestinationPortNumber(portServer));
//     p0_filterA->AddFilterElement(new SourcePortNumber(portA));
//     Filter* p0_filterB = new Filter();
//     p0_filterB->AddFilterElement(new DestinationPortNumber(portA));
//     // p0_filterB->AddFilterElement(new SourcePortNumber(portServer));
//     spq->GetTrafficClass(0)->AddFilter(p0_filterA);
//     spq->GetTrafficClass(0)->AddFilter(p0_filterB);
//     std::cout << "Main: 0 filter size: " << spq->GetTrafficClass(0)->GetFilterSize() << std::endl;

//     Filter* p1_filterA = new Filter();
//     // p1_filterA->AddFilterElement(new DestinationPortNumber(portServer));
//     p1_filterA->AddFilterElement(new SourcePortNumber(portB));
//     Filter* p1_filterB = new Filter();
//     p1_filterB->AddFilterElement(new DestinationPortNumber(portB));
//     // p1_filterB->AddFilterElement(new SourcePortNumber(portServer));
//     spq->GetTrafficClass(1)->AddFilter(p1_filterA);
//     spq->GetTrafficClass(1)->AddFilter(p1_filterB);
//     std::cout << "Main: 1 filter size: " << spq->GetTrafficClass(1)->GetFilterSize() << std::endl;

//     nodes.Get(1)->GetDevice(0)->GetObject<PointToPointNetDevice>()->SetQueue(spq);
//     nodes.Get(1)->GetDevice(1)->GetObject<PointToPointNetDevice>()->SetQueue(spq);

//     /*----------------customized spq ends----------------*/

//     Ipv4GlobalRoutingHelper::PopulateRoutingTables();

//     /*---------------------UDP starts------------------*/

//     // Set up the server application on node 2
//     UdpServerHelper serverA(portA);
//     ApplicationContainer serverAppA = serverA.Install(nodes.Get(2));
//     serverAppA.Start(Seconds(SERVER_START_TIME));
//     serverAppA.Stop(Seconds(SERVER_END_TIME));

//     UdpServerHelper serverB(portB);
//     ApplicationContainer serverAppB = serverB.Install(nodes.Get(2));
//     serverAppB.Start(Seconds(SERVER_START_TIME));
//     serverAppB.Stop(Seconds(SERVER_END_TIME));

//     // Set up the client application on node 0
//     Time interPacketInterval = Seconds(PACKET_INTERVAL);
//     UdpClientHelper clientA(interfacesB.GetAddress(1), portA);
//     clientA.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
//     clientA.SetAttribute("Interval", TimeValue(interPacketInterval));
//     clientA.SetAttribute("PacketSize", UintegerValue(1024));

//     ApplicationContainer clientAppA = clientA.Install(nodes.Get(0));
//     clientAppA.Start(Seconds(CLIENTA_START_TIME));
//     clientAppA.Stop(Seconds(CLIENTA_END_TIME));

//     UdpClientHelper clientB(interfacesB.GetAddress(1), portB);
//     clientB.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
//     clientB.SetAttribute("Interval", TimeValue(interPacketInterval));
//     clientB.SetAttribute("PacketSize", UintegerValue(1024));

//     ApplicationContainer clientAppB = clientB.Install(nodes.Get(0));
//     clientAppB.Start(Seconds(CLIENTB_START_TIME));
//     clientAppB.Stop(Seconds(CLIENTB_END_TIME));

//     /*---------------------UDP ends------------------*/

//     // /*-----------------Tcp Starts/ Do not touch ----------------*/
//     // // Set up the server application on node 2
//     // PacketSinkHelper serverA ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (),
//     // portA)); ApplicationContainer serverAppA = serverA.Install (nodes.Get (2)); serverAppA.Start
//     // (Seconds (1.0)); serverAppA.Stop (Seconds (15.0));

//     // // Set up the server application on node 2
//     // PacketSinkHelper serverB ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (),
//     // portB)); ApplicationContainer serverAppB = serverB.Install (nodes.Get (2)); serverAppB.Start
//     // (Seconds (1.0)); serverAppB.Stop (Seconds (15.0));

//     // // Set up the client application on node 0
//     // BulkSendHelper clientA ("ns3::TcpSocketFactory", InetSocketAddress
//     // (interfacesB.GetAddress(1), portA)); clientA.SetAttribute ("MaxBytes", UintegerValue (0));
//     // clientA.SetAttribute("Local", AddressValue(InetSocketAddress(Ipv4Address::GetAny(), portA)));
//     // ApplicationContainer clientAppA = clientA.Install (nodes.Get(0));
//     // clientAppA.Start (Seconds (2.5));
//     // clientAppA.Stop (Seconds (15.0));

//     // BulkSendHelper clientB ("ns3::TcpSocketFactory", InetSocketAddress (interfacesB.GetAddress
//     // (1), portB)); clientB.SetAttribute ("MaxBytes", UintegerValue (0));
//     // clientB.SetAttribute("Local", AddressValue(InetSocketAddress(Ipv4Address::GetAny(), portB)));
//     // ApplicationContainer clientAppB = clientB.Install (nodes.Get (0));
//     // clientAppB.Start (Seconds (2.0));
//     // clientAppB.Stop (Seconds (15.0));

//     /*-----------------Tcp ends/ Do not touch ----------------*/

//     // Enable generating pcap files
//     pointToPoint.EnablePcapAll("scratch/cs621_QoS/pcap/three-3nodes-topology-A");

//     Simulator::Run();
//     Simulator::Destroy();

//     return 0;
// }