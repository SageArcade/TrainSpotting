#include <iostream>
#include <sstream>

enum class LogLevel {Log, Warning, Error, Info};
class ThreadSafeLogger{
private:
    std::string getLogLevelStr(const LogLevel& logLevel){
        switch (logLevel) {
            case LogLevel::Log :        return std::string{"[Log]"}; break;
            case LogLevel::Warning :    return std::string{"[Warning]"}; break;
            case LogLevel::Error :      return std::string{"[Error]"}; break;
            case LogLevel::Info :       return std::string{"[Info]"}; break;
        }
        return std::string{"[Log]"};
    }
    std::mutex logger_mutex;
    
    template<typename... Args>
    void logger(const LogLevel& logLevel, Args&&... args) {
        std::ostringstream stream;
        stream << getLogLevelStr(logLevel) << " ";
        (stream << ... << std::forward<Args>(args));  // Fix 1: no trailing ...

        std::lock_guard<std::mutex> lock(logger_mutex); // Fix 3: actually lock
        std::cout << stream.str() << "\n";             // Fix 2: actually output
    }
    
    ThreadSafeLogger() = default;
    ~ThreadSafeLogger() = default;
    
public:
    
    ThreadSafeLogger(const ThreadSafeLogger& other) = delete;
    ThreadSafeLogger& operator=(const ThreadSafeLogger& other) = delete;
    ThreadSafeLogger(ThreadSafeLogger&& other) noexcept = delete;
    ThreadSafeLogger& operator=(ThreadSafeLogger&& other) noexcept = delete;
    
    static ThreadSafeLogger& getInstance(){
        static ThreadSafeLogger instance;
        return instance;
    }
    
    template<typename ...Args>
    void log(const LogLevel& logLevel, Args&&... args){
        logger(logLevel, std::forward<Args>(args)...);
    }
};

int main(int argc, const char * argv[]) {
    
    ThreadSafeLogger::getInstance().log(LogLevel::Info, "Nikhilesh Gautam : ", "nikgauta");
    
    return EXIT_SUCCESS;
}
