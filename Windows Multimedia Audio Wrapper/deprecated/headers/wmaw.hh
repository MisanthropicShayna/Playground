#ifndef WMAW
#define WMAW

#include <windows.h>
#include <sstream>
#include <iostream>
#include <exception>
#include <array>

#pragma comment(lib, "Winmm.lib")

class MciError {
protected:
    uint32_t errorCode_;
    std::string message_;

public:
    uint32_t const& ErrorCode = errorCode_;
    std::string const& Message = message_;
    
    MciError(uint32_t const& errorCode);
};

class MciCommand : public std::stringstream {
public:
    uint32_t Execute() const;
};

enum struct AudioFormat {
    MP3 = 0,
    WAV = 1
};

class AudioFile {
protected:
    bool fileLoaded_;               // A bool to specify whether or not a file is currently loaded by MCI.
    
    MciError lastMciError_;         // The last MciError encountered that was subsequently thrown.
      
    std::string fileAlias_;         // The MCI alias for the loaded file. This is usually the instance address.
    std::string filePath_;          // The path of the loaded file.
     
    AudioFormat format_;            // An AudioFormat enum, representing the audio format of the loaded file.
    std::string formatStr_;         // The MCI string representation of the audio format.
    
public:
    /* Read-only accessors for protected members */
    bool const& FileLoaded = fileLoaded_;
    
    uint32_t const& LastError = lastError_;
    std::string const& LastErrorStr = lastErrorStr_;
    
    std::string const& FileAlias = fileAlias_;
    std::string const& FilePath = filePath_;

    AudioFormat const& Format = format_;
    std::string const& FormatStr = formatStr_;
    /* ----------------------------------------- */
    
    void StartPlayback(double start = 0, double end = 0, bool async=false);
    void PausePlayback();
    void ResumePlayback();
    void StopPlayback();

    void Open(std::string const& path, AudioFormat const& format);
    void Close();
    
    AudioFile(std::string const& path, AudioFormat const& format);
    
    AudioFile();
    ~AudioFile();
};

#endif