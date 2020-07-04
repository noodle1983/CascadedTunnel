#include "PcapSniffer.h"

int main()
{
    Pcap::Sniffer sniffer;
    sniffer.start();

    sleep(3);

    sniffer.stop();
    return 0;
}

