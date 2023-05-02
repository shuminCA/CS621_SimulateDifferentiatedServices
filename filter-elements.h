#ifndef FILTER_ELEMENTS_H
#define FILTER_ELEMENTS_H

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
  SrcIPAddr(Ipv4Address value);

  bool match(Ptr<Packet> p) override;
};

class SrcPortNo : public FilterElement {
private:
  uint32_t m_value;

public:
  SrcPortNo(uint32_t value);

  bool match(Ptr<Packet> p) override;
};

class DestIPAddr : public FilterElement {
private:
  Ipv4Address m_value;

public:
  DestIPAddr(Ipv4Address value);

  bool match(Ptr<Packet> p) override;
};

class DestPortNo : public FilterElement {
private:
  uint32_t m_value;

public:
  DestPortNo(uint32_t value);

  bool match(Ptr<Packet> p) override;
};

class ProtocolNo : public FilterElement {
private:
  uint32_t m_value;

public:
  ProtocolNo(uint32_t value);

  bool match(Ptr<Packet> p) override;
};

#endif // FILTER_ELEMENTS_H
