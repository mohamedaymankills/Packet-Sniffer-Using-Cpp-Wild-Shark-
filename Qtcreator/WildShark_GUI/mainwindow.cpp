#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), isSniffing(false) {
    ui->setupUi(this);

    // Initialize the packet sniffer
    sniffer = new PacketSniffer(this);

    // Connect signals and slots
    connect(sniffer, &PacketSniffer::packetCaptured, this, &MainWindow::updatePacketInfo);
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::on_startStopButton_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_startStopButton_clicked() {
    if (isSniffing) {
        // Stop sniffing
        sniffer->stopCapture();
        ui->startStopButton->setText("Start Sniffing");
    } else {
        // Start sniffing
        QString ipFilter = ""; // Add IP filter if needed
        QString portFilter = ""; // Add port filter if needed
        QString interface = "wlp2s0"; // Replace with the desired interface

        sniffer->startCapture(interface, ipFilter, portFilter);
        ui->startStopButton->setText("Stop Sniffing");
    }

    // Toggle the state
    isSniffing = !isSniffing;
}

void MainWindow::updatePacketInfo(const QString &packetDetails) {
    ui->packetTextEdit->append(packetDetails);
}
