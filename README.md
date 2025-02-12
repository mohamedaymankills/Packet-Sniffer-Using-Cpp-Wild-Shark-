# Packet-Sniffer-Using-Cpp-Wild-Shark-
## What is a Packet Sniffer?
A packet sniffer is a tool that captures and analyzes network traffic. It works at the data link layer (Layer 2) or network layer (Layer 3) of the OSI model.

    The sniffer puts the network interface into promiscuous mode to capture all packets, not just those addressed to the host.

    It uses low-level libraries like libpcap (Linux) or WinPcap/Npcap (Windows) to capture packets.
    
    
    Promiscuous Mode: Allows the network interface to capture all packets on the network segment.

    Packet Headers: Each packet contains headers (Ethernet, IP, TCP/UDP/ICMP) that provide metadata about the packet.

    Payload: The actual data being transmitted (e.g., HTTP request, DNS query).   

## What are Network Protocol?
### Ethernet Frame

    The outermost layer of a packet.

    Contains source and destination MAC addresses.

    The EtherType field indicates the next protocol (e.g., IPv4, IPv6).

### IP Packet

    Contains source and destination IP addresses.

    The Protocol field indicates the next layer (e.g., TCP, UDP, ICMP).

### TCP/UDP/ICMP

    TCP: Connection-oriented protocol with source/destination ports, sequence numbers, and flags.

    UDP: Connectionless protocol with source/destination ports.

    ICMP: Used for diagnostic purposes (e.g., ping).

### Application Layer Protocols

    HTTP: Web traffic (port 80).

    DNS: Domain name resolution (port 53).

    FTP: File transfer (port 21).

## Learn About libpcap
### What is libpcap?

    A library for packet capture.

    Provides APIs to capture, filter, and analyze network traffic.

### Key Functions

    pcap_open_live(): Opens a network interface for packet capture.

    pcap_compile(): Compiles a filter expression (e.g., "host 192.168.1.1 and port 80").

    pcap_setfilter(): Applies the filter to the capture session.

    pcap_loop(): Captures packets in a loop and passes them to a callback function.

    pcap_close(): Closes the capture session.

### Filtering with BPF (Berkeley Packet Filter)

    BPF allows you to filter packets based on specific criteria (e.g., IP address, port).

    Example: host 192.168.1.1 and port 80 captures only HTTP traffic to/from 192.168.1.1.
    
