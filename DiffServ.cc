#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "DiffServ.h"

#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include <math.h>

namespace ns3 {

DiffServ::DiffServ() :
        Queue() {
            m_mode = 0;
}

DiffServ::~DiffServ() {
}

void DiffServ::SetMode(int mode) {
	m_mode = mode;
}

int DiffServ::GetMode(void) {
	return m_mode;
}

}


int main() {
    std::cout<<"Testing"<< std::endl;

    return 0;
}