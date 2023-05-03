#include "filter.h"

namespace ns3 {

Filter::Filter() {}

Filter::~Filter() {
  for (auto element : m_elements) {
    delete element;
  }
}

void Filter::AddFilterElement(FilterElement* element) {
  m_elements.push_back(element);
}

bool Filter::match(Ptr<Packet> p) {
  for (auto element : m_elements) {
    if (!element->match(p)) {
      return false;
    }
  }
  return true;
}
}