#include "ns3/core-module.h"
#include "spq_validation.h"
#include "drr_validation.h"

using namespace ns3;

void DRRValidation();
void SPQValidation();

int main(int argc, char* argv[])
{
    std::string mode;

    CommandLine cmd;
    cmd.AddValue("mode", "Choose between 'spq' and 'drr' validation", mode);
    cmd.Parse(argc, argv);

    if (mode == "spq") {
        SPQValidation();
    } else if (mode == "drr") {
        DRRValidation();
    } else {
        std::cerr << "Error: Invalid mode. Please provide either 'spq' or 'drr'." << std::endl;
        return 1;
    }
    return 0;
}