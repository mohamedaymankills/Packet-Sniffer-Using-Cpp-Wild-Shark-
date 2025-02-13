#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "packetsniffer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startStopButton_clicked();
    void updatePacketInfo(const QString &packetDetails);

private:
    Ui::MainWindow *ui;
    PacketSniffer *sniffer;
    bool isSniffing; // Tracks the state of the sniffer
};

#endif // MAINWINDOW_H
