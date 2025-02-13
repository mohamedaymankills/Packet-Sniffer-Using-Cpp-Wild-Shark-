# Packet-Sniffer-Using-Cpp-Wild-Shark-
## What is a Packet Sniffer?

![1](https://github.com/mohamedaymankills/Packet-Sniffer-Using-Cpp-Wild-Shark-/blob/main/Images/Screenshot%20from%202025-02-13%2022-27-24.png?raw=true)
![2](https://github.com/mohamedaymankills/Packet-Sniffer-Using-Cpp-Wild-Shark-/blob/main/Images/Screenshot%20from%202025-02-13%2022-26-03.png?raw=true)
A packet sniffer is a tool that captures and analyzes network traffic. It works at the data link layer (Layer 2) or network layer (Layer 3) of the OSI model.

    The sniffer puts the network interface into promiscuous mode to capture all packets, not just those addressed to the host.

    It uses low-level libraries like libpcap (Linux) or WinPcap/Npcap (Windows) to capture packets.
    
    
    Promiscuous Mode: Allows the network interface to capture all packets on the network segment.

    Packet Headers: Each packet contains headers (Ethernet, IP, TCP/UDP/ICMP) that provide metadata about the packet.

    Payload: The actual data being transmitted (e.g., HTTP request, DNS query).   



# Network Packet Sniffer

## Overview
This project is a simple network packet sniffer implemented in C++ using the `libpcap` library. It captures and analyzes network packets, supporting multiple protocols, including TCP, UDP, ICMP, and application-layer protocols such as HTTP, DNS, and FTP.

## Features
- Captures live packets on a specified network interface.
- Extracts and displays Ethernet, IP, TCP, UDP, and ICMP headers.
- Identifies and processes HTTP, DNS, and FTP payloads.
- Filters packets based on IP address and port using command-line options.

## Dependencies
Ensure the following dependencies are installed before compiling and running the project:
- `g++` (GCC compiler for C++)
- `libpcap` (packet capture library)
- `netinet` headers (included in standard Linux distributions)

## Installation
1. Install `libpcap` if not already installed:
   ```sh
   sudo apt update && sudo apt install libpcap-dev
   ```
2. Clone the repository:
   ```sh
   git clone <repository-url>
   cd <repository-directory>
   ```
3. Compile the project:
   ```sh
   g++ -o packet_sniffer packet_sniffer.cpp -lpcap
   ```

## Usage
Run the program with optional filters:
```sh
sudo ./packet_sniffer [-i IP] [-p PORT]
```
- `-i IP` : Filter packets based on the specified IP address.
- `-p PORT` : Filter packets based on the specified port number.

Example:
```sh
sudo ./packet_sniffer -i 192.168.1.1 -p 80
```
This captures packets to/from IP `192.168.1.1` and port `80` (HTTP traffic).

## Code Structure
- `ApplicationLayer` (Base class for protocol handlers)
- `HTTP`, `DNS`, `FTP` (Derived classes for application-layer protocols)
- `packet_handler()` (Processes each captured packet)
- `main()` (Initializes packet capture and sets filters)

## License
This project is licensed under the MIT License.

## Author
Mohamed Ayman Mohamed Abdelaziz

## Download
[Download README](./README.md)





## UML
![3](https://github.com/mohamedaymankills/Packet-Sniffer-Using-Cpp-Wild-Shark-/blob/main/Images/UML%20class.png?raw=true)
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


## Steps to Implement

### Open a Network Interface:

        Use pcap_open_live() to start capturing packets.

### Set Up Filtering:

        Use pcap_compile() and pcap_setfilter() to filter packets by IP/port.

### Capture Packets:

        Use pcap_loop() to capture packets and pass them to a callback function.

### Parse Packet Headers:

        Extract Ethernet, IP, and TCP/UDP/ICMP headers.

        Use structs like ethhdr, iphdr, tcphdr, udphdr, and icmphdr.

### Digest Application Layer Protocols:

        Identify the protocol based on the port number (e.g., port 80 for HTTP).

        Parse the payload data.

    Handle Inheritance for Application Protocols:

        Use a base class (e.g., ApplicationLayer) and derive classes for HTTP, DNS, and FTP.

        Override a process() method to handle protocol-specific parsing.


## Why Use Inheritance?

    Inheritance allows you to create a generic interface for processing application layer protocols.

    Each protocol (HTTP, DNS, FTP) can implement its own parsing logic.

### Design

   ### Base Class:

        Define a virtual method process() to handle payload data.

   ### Derived Classes:

        Implement process() for each protocol (e.g., HTTP, DNS, FTP).        


## Implement Filtering
### Command-Line Arguments

    Use a library like getopt to parse command-line options for IP and port filtering.

### BPF Filter

    Construct a filter expression based on the provided IP and port.

    Example: host 192.168.1.1 and port 80.

### Apply the Filter

    Use pcap_compile() and pcap_setfilter() to apply the filter to the capture session.
    
