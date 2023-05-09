#ifndef FILTER_ELEMENTS_H
#define FILTER_ELEMENTS_H

#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"

namespace ns3 {

class FilterElement {
public:
  virtual ~FilterElement(){}
  virtual bool match(Ptr<Packet> p) = 0;
};

class SourceIPAddress : public FilterElement {
private:
  Ipv4Address m_value;

public:
  SourceIPAddress(Ipv4Address value);

  bool match(Ptr<Packet> p) override;
};

class SourceMask : public FilterElement {
private:
  Ipv4Mask m_mask;
  Ipv4Address m_address;

public:
  SourceMask(Ipv4Mask value, Ipv4Address address);

  bool match(Ptr<Packet> p) override;
};

class SourcePortNumber : public FilterElement {
private:
  uint32_t m_value;

public:
  SourcePortNumber(uint32_t value);

  bool match(Ptr<Packet> p) override;
};

class DestinationIPAddress : public FilterElement {
private:
  Ipv4Address m_value;

public:
  DestinationIPAddress(Ipv4Address value);

  bool match(Ptr<Packet> p) override;
};

class DestinationMask : public FilterElement {
private:
  Ipv4Mask m_mask;
  Ipv4Address m_address;

public:
  DestinationMask(Ipv4Mask mask, Ipv4Address address);

  bool match(Ptr<Packet> p) override;
};

class DestinationPortNumber : public FilterElement {
private:
  uint32_t m_value;

public:
  DestinationPortNumber(uint32_t value);

  bool match(Ptr<Packet> p) override;
};

class ProtocolNumber : public FilterElement {
private:
  uint32_t m_value;

public:
  ProtocolNumber(uint32_t value);

  bool match(Ptr<Packet> p) override;
};

}

#endif // FILTER_ELEMENTS_H
