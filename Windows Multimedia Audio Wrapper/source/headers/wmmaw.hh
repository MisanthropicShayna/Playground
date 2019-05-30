#ifndef WMMAW_HH
#define WMMAW_HH

#include <windows.h>
#include <sstream>
#include <string>

class AudioFile {
protected:
    uint32_t lastErrorCode_;
    std::string lastErrorString_;
    std::string mediaAlias_;
    std::string filePath_;
    std::string encoding_;
    
public:
    /* Public read-only references to protected members */
    const uint32_t& LastErrorCode = lastErrorCode_;
    const std::string& LastErrorString = lastErrorString_;
    const std::string& MediaAlias = mediaAlias_;
    const std::string& FilePath = filePath_;
    const std::string& Encoding = encoding_;
    /* ------------------------------------------------ */
    
    std::string ErrorAsString(uint32_t = 0) const;
    
    uint32_t Play(double = 0, double = 0, bool = false);
    uint32_t Pause();
    uint32_t Resume();
    uint32_t Stop();

    uint32_t Open(const std::string& filePath, const std::string& encoding);
    uint32_t Reopen();
    uint32_t Close();

    AudioFile(const std::string& filePath, const std::string& encoding);
    AudioFile();
    ~AudioFile();
};

#endif