#include "filter-elements.h"

namespace ns3 {

SrcIPAddr::SrcIPAddr(Ipv4Address value) : m_value(value) {}

bool SrcIPAddr::match(Ptr<Packet> p) {
  Ipv4Header ipv4Header;
  p->PeekHeader(ipv4Header);
  return ipv4Header.GetSource() == m_value;
}

SrcPortNo::SrcPortNo(uint32_t value) : m_value(value) {}

bool SrcPortNo::match(Ptr<Packet> p) {
  UdpHeader header;
  p->PeekHeader(header);
  return header.GetSourcePort() == m_value;
}

DestIPAddr::DestIPAddr(Ipv4Address value) : m_value(value) {}

bool DestIPAddr::match(Ptr<Packet> p) {
  Ipv4Header ipv4Header;
  p->PeekHeader(ipv4Header);
  return ipv4Header.GetDestination() == m_value;
}

DestPortNo::DestPortNo(uint32_t value) : m_value(value) {}

bool DestPortNo::match(Ptr<Packet> p) {
  UdpHeader header;
  p->PeekHeader(header);
  return header.GetDestinationPort() == m_value;
}

ProtocolNo::ProtocolNo(uint32_t value) : m_value(value) {}

bool ProtocolNo::match(Ptr<Packet> p) {
  Ipv4Header header;
  p->PeekHeader(header);
  return header.GetProtocol() == m_value;
}

SourceMask::SourceMask(Ipv4Mask value, Ipv4Address referenceAddress) 
  : m_value(value), m_referenceAddress(referenceAddress) {}

bool SourceMask::match(Ptr<Packet> p) {
  Ipv4Header ipv4Header;
  p->PeekHeader(ipv4Header);
  Ipv4Address srcAddress = ipv4Header.GetSource();
  Ipv4Address maskedSrcAddress = srcAddress.CombineMask(m_value);
  Ipv4Address maskedRefAddress = m_referenceAddress.CombineMask(m_value);
  return maskedSrcAddress == maskedRefAddress;
}
}
