#ifndef FILTER_ELEMENTS_H
#define FILTER_ELEMENTS_H

#include <iostream>
#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"

using namespace ns3;

class FilterElement {
public:
  virtual bool match(Ptr<Packet> p) = 0;
};

class SrcIPAddr : public FilterElement {
private:
  Ipv4Address m_value;

public:
  SrcIPAddr(Ipv4Address value) : m_value(value) {}

  bool match(Ptr<Packet> p) {
    Ipv4Header ipv4Header;
    p->PeekHeader(ipv4Header);
    return ipv4Header.GetSource() == m_value;
  }
};

// class SrcMask : public FilterElement {
// private:
//   Ipv4Mask m_value;

// public:
//   SrcMask(Ipv4Mask value) : m_value(value) {}

//   bool match(Ptr<Packet> p) {
//     Ipv4Header header;
//     p->PeekHeader(header);
//     Ipv4Address srcIP = header.GetSource();
//     return m_value.IsMatch(srcIP);
//   }
// };

class SrcPortNo : public FilterElement {
private:
  uint32_t m_value;

public:
  SrcPortNo(uint32_t value) : m_value(value) {}

  bool match(Ptr<Packet> p) {
    UdpHeader header;
    p->PeekHeader(header);
    return header.GetSourcePort() == m_value;
  }
};

class DestIPAddr : public FilterElement {
private:
  Ipv4Address m_value;

public:
  DestIPAddr(Ipv4Address value) : m_value(value) {}
  bool match(Ptr<Packet> p) {
    Ipv4Header ipv4Header;
    p->PeekHeader(ipv4Header);
    return ipv4Header.GetDestination() == m_value;
  }
};

// class DestMask : public FilterElement {
// private:
//   Ipv4Mask m_value;

// public:
//   DestMask(Ipv4Mask value) : m_value(value) {}
//   bool match(Ptr<Packet> p) {
//     Ipv4Header header;
//     p->PeekHeader(header);
//     return (header.GetDestination() & m_value) == (m_value & m_value);
//   }
// };

class DestPortNo : public FilterElement {
private:
  uint32_t m_value;

public:
  DestPortNo(uint32_t value) : m_value(value) {}
  bool match(Ptr<Packet> p) {
    UdpHeader header;
    p->PeekHeader(header);
    return header.GetDestinationPort() == m_value;
  }
};

class ProtocolNo : public FilterElement {
private:
  uint32_t m_value;

public:
  ProtocolNo(uint32_t value) : m_value(value) {}
  bool match(Ptr<Packet> p) {
    Ipv4Header header;
    p->PeekHeader(header);
    return header.GetProtocol() == m_value;
  }
};

int main(){
    std::cout << "Testing FilterElement" << std::endl;
}

#endif
