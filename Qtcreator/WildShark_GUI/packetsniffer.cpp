#include "packetsniffer.h"
#include <pcap.h>
#include <QDebug>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

PacketSniffer::PacketSniffer(QObject *parent) : QObject(parent), handle(nullptr), isCapturing(false) {}

PacketSniffer::~PacketSniffer() {
    stopCapture();
}

void PacketSniffer::startCapture(const QString &interface, const QString &ipFilter, const QString &portFilter) {
    if (isCapturing) return;

    char errbuf[PCAP_ERRBUF_SIZE];
    QString filter;

    // Construct the filter only if both IP and port are provided
    if (!ipFilter.isEmpty() && !portFilter.isEmpty()) {
        filter = QString("host %1 and port %2").arg(ipFilter).arg(portFilter);
    } else if (!ipFilter.isEmpty()) {
        filter = QString("host %1").arg(ipFilter);
    } else if (!portFilter.isEmpty()) {
        filter = QString("port %1").arg(portFilter);
    } else {
        filter = ""; // No filter
    }

    handle = pcap_open_live(interface.toStdString().c_str(), BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        qDebug() << "Could not open device:" << errbuf;
        return;
    }

    if (!filter.isEmpty()) {
        struct bpf_program fp;
        if (pcap_compile(handle, &fp, filter.toStdString().c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1) {
            qDebug() << "Could not parse filter:" << pcap_geterr(handle);
            pcap_close(handle);
            return;
        }
        if (pcap_setfilter(handle, &fp) == -1) {
            qDebug() << "Could not install filter:" << pcap_geterr(handle);
            pcap_close(handle);
            return;
        }
    }

    isCapturing = true;
    captureThread = QThread::create([this] { capturePackets(); });
    captureThread->start();
}



void PacketSniffer::stopCapture() {
    if (!isCapturing) return;

    isCapturing = false;
    pcap_breakloop(handle);
    captureThread->quit();
    captureThread->wait();
    pcap_close(handle);
}

void PacketSniffer::capturePackets() {
    while (isCapturing) {
        struct pcap_pkthdr *header;
        const u_char *packet;
        int result = pcap_next_ex(handle, &header, &packet);

        if (result == 1) {
            // Process packet and emit signal
            QString packetDetails = "Packet captured";
            emit packetCaptured(packetDetails);
        }
    }
}
