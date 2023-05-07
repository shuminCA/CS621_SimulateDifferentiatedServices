#ifndef FILTER_H
#define FILTER_H

#include "filter-elements.h"
#include <vector>

namespace ns3 {

class Filter {
public:
  Filter();
  ~Filter();

  void AddFilterElement(FilterElement* element);
  bool match(Ptr<Packet> p);
  uint32_t size();

private:
  std::vector<FilterElement*> m_elements;
};
}

#endif