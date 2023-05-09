#include "filter.h"

namespace ns3 {

Filter::Filter() {
  // std::cout << "Filter: Create Filter" << std::endl;
}

Filter::~Filter() {
  for (auto element : m_elements) {
    delete element;
  }
}

void Filter::AddFilterElement(FilterElement* element) {
  // std::cout << "Filter: AddFilterElement" << std::endl;
  m_elements.push_back(element);
}

uint32_t Filter::size(){
  return m_elements.size();
}

bool Filter::match(Ptr<Packet> p) {
  if (m_elements.size() == 0){
    return false;
  }else{
    for (auto element : m_elements) {
      // std::cout << "Filter: element size " << m_elements.size()<< std::endl;
      if (!element->match(p)) {
        return false;
      }
    }
    // std::cout << "Filter: return true" << std::endl;
    return true;
  }
}
}