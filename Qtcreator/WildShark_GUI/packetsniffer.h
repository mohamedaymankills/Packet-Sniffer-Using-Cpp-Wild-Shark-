#ifndef PACKETSNIFFER_H
#define PACKETSNIFFER_H

#include <QObject>
#include <QThread>
#include <pcap.h>

class PacketSniffer : public QObject {
    Q_OBJECT

public:
    explicit PacketSniffer(QObject *parent = nullptr);
    ~PacketSniffer();

    void startCapture(const QString &interface, const QString &ipFilter, const QString &portFilter);
    void stopCapture();

signals:
    void packetCaptured(const QString &packetDetails);

private:
    pcap_t *handle;
    bool isCapturing;
    QThread *captureThread;

    void capturePackets();
};

#endif // PACKETSNIFFER_H
