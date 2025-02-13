#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <pcap.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <arpa/inet.h> // For inet_ntoa()
#include <netinet/ip.h> // For IP header
#include <netinet/tcp.h> // For TCP header
#include <netinet/udp.h> // For UDP header
#include <netinet/ip_icmp.h> // For ICMP header

// Global variables
std::vector<std::string> packetLogs;
std::mutex packetLogMutex;
bool isSniffing = false;

// Function to capture packets
void capturePackets() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live("wlp2s0", BUFSIZ, 1, 1000, errbuf);

    if (!handle) {
        std::cerr << "Could not open device: " << errbuf << std::endl;
        return;
    }

    while (isSniffing) {
        struct pcap_pkthdr *header;
        const u_char *packet;
        int result = pcap_next_ex(handle, &header, &packet);

        if (result == 1) {
            // Parse the IP header
            struct ip *ipHeader = (struct ip *)(packet + 14); // Skip Ethernet header (14 bytes)
            char srcIp[INET_ADDRSTRLEN];
            char dstIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipHeader->ip_src), srcIp, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(ipHeader->ip_dst), dstIp, INET_ADDRSTRLEN);

            // Determine the protocol type
            std::string protocol;
            switch (ipHeader->ip_p) {
                case IPPROTO_TCP:
                    protocol = "TCP";
                    break;
                case IPPROTO_UDP:
                    protocol = "UDP";
                    break;
                case IPPROTO_ICMP:
                    protocol = "ICMP";
                    break;
                default:
                    protocol = "Unknown";
                    break;
            }

            // Log the packet details
            std::lock_guard<std::mutex> lock(packetLogMutex);
            packetLogs.push_back(
                "Source: " + std::string(srcIp) +
                " -> Destination: " + std::string(dstIp) +
                " | Protocol: " + protocol
            );
        }
    }

    pcap_close(handle);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a window
    GLFWwindow *window = glfwCreateWindow(800, 600, "Packet Sniffer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a simple window
        ImGui::Begin("Packet Sniffer");

        if (ImGui::Button(isSniffing ? "Stop Sniffing" : "Start Sniffing")) {
            isSniffing = !isSniffing;
            if (isSniffing) {
                std::thread(capturePackets).detach();
            }
        }

        ImGui::Text("Packet Log:");
        ImGui::BeginChild("Scrolling");
        std::lock_guard<std::mutex> lock(packetLogMutex);
        for (const auto &log : packetLogs) {
            ImGui::Text("%s", log.c_str());
        }
        ImGui::EndChild();

        ImGui::End();

        // Render ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
