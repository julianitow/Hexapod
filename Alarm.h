#ifndef ALARM_H
#define ALARM_H

#include <QProcess>
#include <QDir>
#include <iostream>
#include <thread>
#include <string.h>
#include <chrono>
#include <functional>
#include <cstring>
#include <pthread.h>
#include <QWebSocket>

#define DEBUG true

#ifdef DEBUG
#include <QDebug>
#endif

class Alarm
{
private:
    Alarm();
    Alarm(const char*);
    QProcess* buzzProcess;
    QProcess* lcdProcess;
    QProcess* stopBuzzerProcess;
    QProcess* automaticProcess;
    static std::thread* buzzThread;
    static pthread_t threadHandle;
    static Alarm* alarm_;
    const char* time2buzz;

protected:


public:
    static Alarm* getAlarm();
    static bool isAlarmExists();
    void setTime2Buzz(const char*);
    static void threadExec(const char*);
    void buzz();
    void wakeUpScreen();
    void stopBuzzing();
    bool waitForBuzz(const char*);
    bool isBuzzing();
    const char* getTime2Buzz();
    static void initAlarm();
    void stopBuzzer();
    std::string status(QWebSocket*);
    void automaticMode(bool);
};

#endif // ALARM_H
