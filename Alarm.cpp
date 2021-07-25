#include "Alarm.h"
Alarm* Alarm::alarm_ = nullptr;
std::thread* Alarm::buzzThread = nullptr;
pthread_t Alarm::threadHandle = NULL;

Alarm::Alarm()
{}

Alarm::Alarm(const char* time2Buzz) {
    Alarm::getAlarm()->time2buzz = time2Buzz;
}

Alarm* Alarm::getAlarm(){
    if (Alarm::alarm_ == nullptr){
        Alarm::alarm_ = new Alarm();
    }
    return Alarm::alarm_;
}

bool Alarm::isAlarmExists() {
    return Alarm::alarm_ == nullptr ? false : true;
}

void Alarm::setTime2Buzz(const char* time2Buzz){
   Alarm::time2buzz = time2Buzz;
}

void Alarm::buzz() {
    Alarm::getAlarm()->buzzProcess = new QProcess(nullptr);
    QStringList args = QStringList();
    args << QDir::currentPath() + QDir::separator() + "buzzer.py";
    Alarm::getAlarm()->buzzProcess->setProgram("python");
    Alarm::getAlarm()->buzzProcess->setArguments(args);
    Alarm::getAlarm()->buzzProcess->open(QIODevice::ReadWrite);

    if(!Alarm::getAlarm()->buzzProcess->waitForStarted(-1)){
        std::cerr << "An error occured while starting buzzProcess" << std::endl;
    } else {
        Alarm::getAlarm()->wakeUpScreen();
    }

    if(Alarm::getAlarm()->buzzProcess->waitForFinished(-1)){
        Alarm::getAlarm()->buzzProcess->close();
    } else {
        std::cerr << "An error occured while running buzzProcess" << std::endl;
    }
}

void Alarm::wakeUpScreen() {
    // std::cout << "WAAAAAKE UP" << std::endl;
    Alarm::getAlarm()->lcdProcess = new QProcess(nullptr);
    QStringList args = QStringList();
    args << QDir::currentPath() + QDir::separator() + "LCD.py";
    args << "WAAAAKE UP";
    Alarm::getAlarm()->lcdProcess->setProgram("python");
    Alarm::getAlarm()->lcdProcess->setArguments(args);
    Alarm::getAlarm()->lcdProcess->open(QIODevice::ReadWrite);

    if(!Alarm::getAlarm()->lcdProcess->waitForStarted(-1)){
        std::cerr << "An error occured while starting lcdProcess" << std::endl;
    }

    if(Alarm::getAlarm()->lcdProcess->waitForFinished(-1)){
        Alarm::getAlarm()->lcdProcess->close();
    } else {
        std::cerr << "An error occured while running lcdProcess" << std::endl;
    }
}

void Alarm::stopBuzzer() {
    Alarm::getAlarm()->stopBuzzerProcess = new QProcess(nullptr);
    QStringList args = QStringList();
    args << QDir::currentPath() + QDir::separator() + "stopBuzzer.py";
    Alarm::getAlarm()->stopBuzzerProcess->setProgram("python");
    Alarm::getAlarm()->stopBuzzerProcess->setArguments(args);
    Alarm::getAlarm()->stopBuzzerProcess->open(QIODevice::ReadWrite);
}

bool Alarm::isBuzzing(){
    qDebug() << "Is already buzzing:" << this->buzzProcess->state();
    if (Alarm::getAlarm()->buzzProcess->state() == QProcess::Running){
        return true;
    }
    return false;
}

void Alarm::stopBuzzing() {
    try {
        QStringList args = QStringList();
        pthread_cancel(Alarm::threadHandle);

        if (Alarm::getAlarm()->buzzProcess != nullptr) {
            if(Alarm::getAlarm()->buzzProcess->isOpen()){
                Alarm::getAlarm()->buzzProcess->close();
                Alarm::getAlarm()->stopBuzzer();
            }
        }
        if (Alarm::getAlarm()->lcdProcess != nullptr) {
            if (Alarm::getAlarm()->lcdProcess->isOpen()) {
                Alarm::getAlarm()->lcdProcess->close();
                Alarm::getAlarm()->lcdProcess = new QProcess(nullptr);
                args = QStringList();
                args << QDir::currentPath() + QDir::separator() + "LCD.py";
                args << "Status: Online";
                Alarm::getAlarm()->lcdProcess->setProgram("python");
                Alarm::getAlarm()->lcdProcess->setArguments(args);
                Alarm::getAlarm()->lcdProcess->open(QIODevice::ReadWrite);
            }
        }
    }  catch (std::exception const& err) {
        std::cerr << "Eception occured: " << err.what() << std::endl;
    }
}

void Alarm::threadExec(const char* time){
    char buff[10];
    while(strcmp(buff, time) != 0){
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        struct tm * timeInfo = std::localtime(&nowTime);
        std::strftime(buff, 10, "%H:%M:00", timeInfo);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        if(DEBUG){
            std::cout << "Now is: " << buff << std::endl;
            std::cout << "Time to buzz: " << time << std::endl;
        }
    }
    Alarm::getAlarm()->buzz();
}

const char* Alarm::getTime2Buzz() {
    return Alarm::getAlarm()->time2buzz;
}

bool Alarm::waitForBuzz(const char* time){
    try {
        Alarm::buzzThread = new std::thread(Alarm::threadExec, time);
        Alarm::threadHandle = Alarm::buzzThread->native_handle();
        Alarm::buzzThread->detach();
    }  catch (std::system_error const& err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    return true;
}
