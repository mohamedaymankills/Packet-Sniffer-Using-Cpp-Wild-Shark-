#include <iostream> // For standard input/output operations
#include <pcap.h> // For packet capture (libpcap)
#include <arpa/inet.h> // For IP address manipulation (inet_ntoa)
#include <netinet/if_ether.h> // For Ethernet header definitions
#include <netinet/ip.h> // For IP header definitions
#include <netinet/tcp.h> // For TCP header definitions
#include <netinet/udp.h> // For UDP header definitions
#include <netinet/ip_icmp.h> // For ICMP header definitions
#include <cstring> // For string manipulation (strcpy, strcat)
#include <getopt.h> // For parsing command-line options

// Base class for application layer protocols
class ApplicationLayer {
public:
    // Pure virtual function to process payload data
    virtual void process(const u_char *payload, int length) = 0;

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~ApplicationLayer() = default;
};

// HTTP protocol handler (derived from ApplicationLayer)
class HTTP : public ApplicationLayer {
public:
    // Process HTTP payload data
    void process(const u_char *payload, int length) override {
        std::cout << "HTTP Data: ";
        // Print each byte of the payload as a character
        for (int i = 0; i < length; i++) {
            std::cout << payload[i];
        }
        std::cout << std::endl;
    }
};

// DNS protocol handler (derived from ApplicationLayer)
class DNS : public ApplicationLayer {
public:
    // Process DNS payload data
    void process(const u_char *payload, int length) override {
        std::cout << "DNS Data: ";
        // Print each byte of the payload in hexadecimal format
        for (int i = 0; i < length; i++) {
            printf("%02X ", payload[i]);
        }
        std::cout << std::endl;
    }
};

// FTP protocol handler (derived from ApplicationLayer)
class FTP : public ApplicationLayer {
public:
    // Process FTP payload data
    void process(const u_char *payload, int length) override {
        std::cout << "FTP Data: ";
        // Print each byte of the payload as a character
        for (int i = 0; i < length; i++) {
            std::cout << payload[i];
        }
        std::cout << std::endl;
    }
};

// Function to handle captured packets
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // Extract the Ethernet header from the packet
    struct ethhdr *eth_header = (struct ethhdr *)packet;

    // Check if the packet is an IP packet (EtherType is ETH_P_IP)
    if (ntohs(eth_header->h_proto) == ETH_P_IP) {
        // Extract the IP header (located after the Ethernet header)
        struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ethhdr));

        // Print IP header fields
        std::cout << "Source IP: " << inet_ntoa(*(struct in_addr *)&ip_header->saddr) << std::endl;
        std::cout << "Destination IP: " << inet_ntoa(*(struct in_addr *)&ip_header->daddr) << std::endl;
        std::cout << "Protocol: " << (int)ip_header->protocol << std::endl;

        // Calculate the length of the IP header (IHL field is in 32-bit words)
        int ip_header_len = ip_header->ihl * 4;

        // Extract the transport layer header (TCP/UDP/ICMP) located after the IP header
        const u_char *transport_layer = packet + sizeof(struct ethhdr) + ip_header_len;

        // Process the transport layer based on the protocol type
        switch (ip_header->protocol) {
            case IPPROTO_TCP: { // TCP protocol
                // Extract the TCP header
                struct tcphdr *tcp_header = (struct tcphdr *)transport_layer;

                // Print TCP source and destination ports
                std::cout << "TCP Source Port: " << ntohs(tcp_header->source) << std::endl;
                std::cout << "TCP Destination Port: " << ntohs(tcp_header->dest) << std::endl;

                // Calculate the length of the TCP header (Data Offset field is in 32-bit words)
                int tcp_header_len = tcp_header->doff * 4;

                // Extract the payload (located after the TCP header)
                const u_char *payload = transport_layer + tcp_header_len;

                // Calculate the payload length
                int payload_length = pkthdr->len - (sizeof(struct ethhdr) + ip_header_len + tcp_header_len);

                // Check if the packet is HTTP (port 80) or FTP (port 21) and process the payload
                if (ntohs(tcp_header->dest) == 80 || ntohs(tcp_header->source) == 80) {
                    HTTP http;
                    http.process(payload, payload_length);
                } else if (ntohs(tcp_header->dest) == 21 || ntohs(tcp_header->source) == 21) {
                    FTP ftp;
                    ftp.process(payload, payload_length);
                }
                break;
            }
            case IPPROTO_UDP: { // UDP protocol
                // Extract the UDP header
                struct udphdr *udp_header = (struct udphdr *)transport_layer;

                // Print UDP source and destination ports
                std::cout << "UDP Source Port: " << ntohs(udp_header->source) << std::endl;
                std::cout << "UDP Destination Port: " << ntohs(udp_header->dest) << std::endl;

                // Extract the payload (located after the UDP header)
                const u_char *payload = transport_layer + sizeof(struct udphdr);

                // Calculate the payload length
                int payload_length = ntohs(udp_header->len) - sizeof(struct udphdr);

                // Check if the packet is DNS (port 53) and process the payload
                if (ntohs(udp_header->dest) == 53 || ntohs(udp_header->source) == 53) {
                    DNS dns;
                    dns.process(payload, payload_length);
                }
                break;
            }
            case IPPROTO_ICMP: { // ICMP protocol
                // Extract the ICMP header
                struct icmphdr *icmp_header = (struct icmphdr *)transport_layer;

                // Print ICMP type and code
                std::cout << "ICMP Type: " << (int)icmp_header->type << std::endl;
                std::cout << "ICMP Code: " << (int)icmp_header->code << std::endl;
                break;
            }
            default:
                // Handle unsupported protocols
                std::cout << "Unsupported protocol" << std::endl;
        }
    }
    // Print a separator after processing each packet
    std::cout << "----------------------------------------" << std::endl;
}

int main(int argc, char *argv[]) {
    char errbuf[PCAP_ERRBUF_SIZE]; // Buffer for error messages
    pcap_t *handle; // Handle for the packet capture session
    char filter_exp[100] = ""; // Buffer for the filter expression
    struct bpf_program fp; // Compiled filter program

    // Parse command-line options
    int opt;
    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
        switch (opt) {
            case 'i': // Filter by IP address
                strcpy(filter_exp, "host ");
                strcat(filter_exp, optarg);
                break;
            case 'p': // Filter by port
                strcat(filter_exp, " and port ");
                strcat(filter_exp, optarg);
                break;
            default: // Invalid option
                std::cerr << "Usage: " << argv[0] << " [-i IP] [-p port]" << std::endl;
                return 1;
        }
    }

    // Open a live capture session on the network interface "wlp2s0"
    handle = pcap_open_live("wlp2s0", BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Could not open device: " << errbuf << std::endl;
        return 1;
    }

    // Compile the filter expression
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Could not parse filter: " << pcap_geterr(handle) << std::endl;
        return 1;
    }

    // Apply the compiled filter to the capture session
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "Could not install filter: " << pcap_geterr(handle) << std::endl;
        return 1;
    }

    // Start capturing packets in a loop
    pcap_loop(handle, 0, packet_handler, nullptr);

    // Close the capture session
    pcap_close(handle);

    return 0;
}
