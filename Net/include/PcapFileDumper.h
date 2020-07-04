#ifndef PCAPFILEDUMPER_H
#define PCAPFILEDUMPER_H

#include "Processor.h"
#include <pcap.h>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace nd
{
    class FileDumper
    {
    public:
        FileDumper();
        ~FileDumper();
		
		void init(pcap_t* thePcapHandler);
		void fini();
		void reset(pcap_t* thePcapHandler);

        void asynHandlePackage(
				boost::shared_ptr<struct pcap_pkthdr> theHeader, 
				boost::shared_array<u_char> thePacket);

		friend class CppProcessor;
		friend class BoostWorker;

	private:
		std::string getFileName();
		void _init(pcap_t* thePcapHandler);
		void _fini();
        void handlePackage(
				boost::shared_ptr<struct pcap_pkthdr> theHeader, 
				boost::shared_array<u_char> thePacket);

    private:    
        CppProcessor processorM;
        pcap_t* pcapHandlerM;
        pcap_dumper_t* pcapDumperM;
    };
}

#endif /* PCAPFILEDUMPER_H */

