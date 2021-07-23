#ifndef ALARM_H
#define ALARM_H

#include <QProcess>
#include <QDir>
#include <iostream>
#include <thread>
#include <string.h>

#define DEBUG true

#ifdef DEBUG
#include <QDebug>
#endif

class Alarm
{
private:
    const char* time2buzz;
    QProcess* buzzProcess;
    QProcess* lcdProcess;
    std::thread* buzzThread;
public:
    Alarm();
    Alarm(const char* time2buzz);
    void setTime2Buzz(const char * time2buzz);
    void buzz();
    void wakeUpScreen();
    void stopBuzzing();
    bool waitForBuzz();
    bool isBuzzing();
};

#endif // ALARM_H
