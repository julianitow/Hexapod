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
    this->buzzProcess->start("python", args, QIODevice::ReadWrite);

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
    std::cout << "WAAAAAKE UP" << std::endl;
    this->buzzProcess = new QProcess(nullptr);
    QStringList args = QStringList();
    args << QDir::currentPath() + QDir::separator() + "LCD.py";
    args << "WAAAAKE UP";
    this->buzzProcess->start("python", args, QIODevice::ReadWrite);

    if(!this->buzzProcess->waitForStarted(-1)){
        std::cerr << "An error occured while starting buzzProcess" << std::endl;
    }

    if(this->buzzProcess->waitForFinished(-1)){
        this->buzzProcess->close();
    } else {
        std::cerr << "An error occured while running buzzProcess" << std::endl;
    }
}

bool Alarm::waitForBuzz(){
    try {
        std::thread loopThread([this]{
            char buff[10];
            while(strcmp(buff, this->time2buzz) != 0){
                auto now = std::chrono::system_clock::now();
                std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
                struct tm * timeInfo = std::localtime(&nowTime);
                std::strftime(buff, 10, "%H:%M:00", timeInfo);
                if(DEBUG){
                    std::cout << "Waiting for right time to buzz:" << buff << std::endl;
                    std::cout << "Time to buzz: " << this->time2buzz << std::endl;
                }
            }
            this->buzz();
            this->wakeUpScreen();
        });
        loopThread.detach();
    }  catch (std::exception const& err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    return true;
}
