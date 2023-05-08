#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"
#include "filter-elements.h"

namespace ns3 {

SourceIPAddress::SourceIPAddress(Ipv4Address value) : m_value(value) {}

bool SourceIPAddress::match(Ptr<Packet> p) {
  Ipv4Header ipv4Header;
  p->PeekHeader(ipv4Header);
  return ipv4Header.GetSource() == m_value;
}

SourcePortNumber::SourcePortNumber(uint32_t value) : m_value(value) {}

bool SourcePortNumber::match(Ptr<Packet> p) {
  std::cout << "FilterElements: SourcePortNumber: " << m_value << std::endl;
  Ptr<Packet> copy = p->Copy();
  PppHeader pppHeader;
  copy->RemoveHeader(pppHeader);

  Ipv4Header ipv4Header;
  copy->RemoveHeader(ipv4Header);

  uint16_t protocol = ipv4Header.GetProtocol();

  if (protocol == UdpL4Protocol::PROT_NUMBER) {
    UdpHeader udpHeader;
    copy->PeekHeader(udpHeader);
    std::cout << "FilterElements: UDPSrcPort: " << udpHeader.GetSourcePort() << std::endl;
    return udpHeader.GetSourcePort() == m_value;
  } else if (protocol == TcpL4Protocol::PROT_NUMBER) {
    TcpHeader tcpHeader;
    copy->PeekHeader(tcpHeader);
    std::cout << "FilterElements: TCPSrcPort: " << tcpHeader.GetSourcePort() << std::endl;
    return tcpHeader.GetSourcePort() == m_value;
  }

  return false;
}

DestinationIPAddress::DestinationIPAddress(Ipv4Address value) : m_value(value) {}

bool DestinationIPAddress::match(Ptr<Packet> p) {
  Ipv4Header ipv4Header;
  p->PeekHeader(ipv4Header);
  return ipv4Header.GetDestination() == m_value;
}

DestPortNo::DestPortNo(uint32_t value) : m_value(value) {}

bool DestPortNo::match(Ptr<Packet> p) {
  std::cout << "FilterElements: DestPortNo: " << m_value << std::endl;

  Ptr<Packet> copy = p->Copy();
  PppHeader pppHeader;
  copy->RemoveHeader(pppHeader);

  Ipv4Header ipv4Header;
  copy->RemoveHeader(ipv4Header);

  uint16_t protocol = ipv4Header.GetProtocol();

  if (protocol == UdpL4Protocol::PROT_NUMBER) {
    UdpHeader udpHeader;
    copy->PeekHeader(udpHeader);
    std::cout << "FilterElements: UDPDestPort: " << udpHeader.GetDestinationPort() << std::endl;
    return udpHeader.GetDestinationPort() == m_value;
  } else if (protocol == TcpL4Protocol::PROT_NUMBER) {
    TcpHeader tcpHeader;
    copy->PeekHeader(tcpHeader);
    // std::cout << "FilterElements: TCPSrcPort: " << tcpHeader.GetSourcePort() << std::endl;
    std::cout << "FilterElements: TCPDestPort: " << tcpHeader.GetDestinationPort() << std::endl;
    return tcpHeader.GetDestinationPort() == m_value;
  }
  return false;
}


ProtocolNumber::ProtocolNumber(uint32_t value) : m_value(value) {}

bool ProtocolNumber::match(Ptr<Packet> p) {
  Ipv4Header header;
  p->PeekHeader(header);
  return header.GetProtocol() == m_value;
}

SourceMask::SourceMask(Ipv4Mask mask, Ipv4Address address) 
  : m_mask(mask), m_address(address) {}

bool SourceMask::match(Ptr<Packet> p) {
  Ipv4Header ipv4Header;
  p->PeekHeader(ipv4Header);
  Ipv4Address PacketAddress = ipv4Header.GetSource();
  Ipv4Address maskedPacketAddress = PacketAddress.CombineMask(m_value);
  Ipv4Address maskedRefAddress = m_referenceAddress.CombineMask(m_value);
  return maskedPacketAddress == maskedRefAddress;
}

DestinationMask::DestinationMask(Ipv4Mask mask, Ipv4Address address) 
  : m_mask(mask), m_address(address) {}

bool DestinationMask::match(Ptr<Packet> p) {
  Ipv4Header ipv4Header;
  p->PeekHeader(ipv4Header);
  Ipv4Address PacketAddress = ipv4Header.GetDestination();
  Ipv4Address maskedPacketAddress = PacketAddress.CombineMask(m_mask);
  Ipv4Address maskedRefAddress = m_address.CombineMask(m_mask);
  return maskedPacketAddress == maskedRefAddress;
}
}
