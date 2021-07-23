#include "alarm.h"

Alarm::Alarm()
{}

Alarm::Alarm(const char* time2Buzz) {
    this->time2buzz = time2Buzz;
}

void Alarm::setTime2Buzz(const char* time2Buzz){
   this->time2buzz = time2Buzz;
}

void Alarm::buzz() {
    this->buzzProcess = new QProcess(nullptr);
    QStringList args = QStringList();
    args << QDir::currentPath() + QDir::separator() + "buzzer.py";
    this->buzzProcess->setProgram("python");
    this->buzzProcess->setArguments(args);
    this->buzzProcess->open(QIODevice::ReadWrite);
    this->buzzProcess->startDetached();
    //this->buzzProcess->start("python", args, QIODevice::ReadWrite);

    if(!this->buzzProcess->waitForStarted(-1)){
        std::cerr << "An error occured while starting buzzProcess" << std::endl;
    }

    if(this->buzzProcess->waitForFinished(-1)){
        this->buzzProcess->close();
    } else {
        std::cerr << "An error occured while running buzzProcess" << std::endl;
    }
}

void Alarm::wakeUpScreen() {
    // std::cout << "WAAAAAKE UP" << std::endl;
    this->lcdProcess = new QProcess(nullptr);
    QStringList args = QStringList();
    args << QDir::currentPath() + QDir::separator() + "LCD.py";
    args << "WAAAAKE UP";
    this->lcdProcess->start("python", args, QIODevice::ReadWrite);

    if(!this->lcdProcess->waitForStarted(-1)){
        std::cerr << "An error occured while starting buzzProcess" << std::endl;
    }

    if(this->lcdProcess->waitForFinished(-1)){
        this->lcdProcess->close();
    } else {
        std::cerr << "An error occured while running buzzProcess" << std::endl;
    }
}

bool Alarm::isBuzzing(){
    qDebug() << "Is already buzzing:" << this->buzzProcess->state();
    if (this->buzzProcess->state() == QProcess::Running){
        return true;
    }
    return false;
}

void Alarm::stopBuzzing() {
    try {
        this->buzzThread->~thread();
        this->buzzProcess->close();
        QStringList args = QStringList();
        args << QDir::currentPath() + QDir::separator() + "stopBuzzer.py";
        this->buzzProcess->setArguments(args);
        this->buzzProcess->startDetached();
    }  catch (std::exception const& err) {
        std::cerr << "Eception occured: " << err.what() << std::endl;
    }
}

void Alarm::threadExec(Alarm* alarm){

    char buff[10];
    while(strcmp(buff, alarm->time2buzz) != 0){
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        struct tm * timeInfo = std::localtime(&nowTime);
        std::strftime(buff, 10, "%H:%M:00", timeInfo);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        if(DEBUG){
            std::cout << "Now is: " << buff << std::endl;
            std::cout << "Time to buzz: " << alarm->time2buzz << std::endl;
        }
    }
    alarm->buzz();
    alarm->wakeUpScreen();
}

bool Alarm::waitForBuzz(){
    try {
        this->buzzThread = new std::thread(Alarm::threadExec, this);
        this->buzzThread->detach();
    }  catch (std::exception const& err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    return true;
}
