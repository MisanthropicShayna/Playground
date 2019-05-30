#include "headers/wmmaw.hh"

#pragma comment(lib, "winmm.lib")

uint32_t AudioFile::Play(double from, double to, bool async) {
    std::stringstream command;
    command << "play " << this->mediaAlias_ << " from " << from;
    if(to) command << " to " << to;
    if(!async) command << " wait";
    uint32_t errorCode = mciSendString(command.str().c_str(), NULL, 0, NULL);
    
    if(errorCode) {
        this->lastErrorCode_ = errorCode;
        this->lastErrorString_ = this->ErrorAsString(this->lastErrorCode_);
    }
    
    return errorCode;
}

uint32_t AudioFile::Pause() {
    std::stringstream command;
    command << "pause " << this->mediaAlias_;
    uint32_t errorCode = mciSendString(command.str().c_str(), NULL, 0, NULL);
    
    if(errorCode) {
        this->lastErrorCode_ = errorCode;
        this->lastErrorString_ = this->ErrorAsString(this->lastErrorCode_);
    }
    
    return errorCode;
}

uint32_t AudioFile::Resume() {
    std::stringstream command;
    command << "resume " << this->mediaAlias_;
    uint32_t errorCode = mciSendString(command.str().c_str(), NULL, 0, NULL);
    
    if(errorCode) {
        this->lastErrorCode_ = errorCode;
        this->lastErrorString_ = this->ErrorAsString(this->lastErrorCode_);
    }
    
    return errorCode;
}

uint32_t AudioFile::Stop() {
    std::stringstream command;
    command << "stop " << this->mediaAlias_;
    uint32_t errorCode = mciSendString(command.str().c_str(), NULL, 0, NULL);
    
    if(errorCode) {
        this->lastErrorCode_ = errorCode;
        this->lastErrorString_ = this->ErrorAsString(this->lastErrorCode_);
    }
    
    return errorCode;
}

uint32_t AudioFile::Open(const std::string& filePath, const std::string& encoding) {
    this->filePath_ = filePath;
    this->encoding_ = encoding;
    
    std::stringstream command;
    command << "open \"" << filePath << "\" ";
    command << "type " << encoding << " ";
    command << "alias " << this->mediaAlias_;
    uint32_t errorCode = mciSendString(command.str().c_str(), NULL, 0, NULL);
    
    if(errorCode) {
        this->lastErrorCode_ = errorCode;
        this->lastErrorString_ = this->ErrorAsString(this->lastErrorCode_);
    }
    
    return errorCode;
}

uint32_t AudioFile::Reopen() {
   this->Close();
   uint32_t errorCode = this->Open(this->filePath_, this->encoding_);
   
   if(errorCode) {
        this->lastErrorCode_ = errorCode;
        this->lastErrorString_ = this->ErrorAsString(this->lastErrorCode_);
    }
    
    return errorCode;
   
}

uint32_t AudioFile::Close() {
    this->Stop();
    
    std::stringstream command;
    command << "close " << this->mediaAlias_;
    uint32_t errorCode = mciSendString(command.str().c_str(), NULL, 0, NULL);
    
    if(errorCode) {
        this->lastErrorCode_ = errorCode;
        this->lastErrorString_ = this->ErrorAsString(this->lastErrorCode_);
    }
    
    return errorCode;
}

AudioFile::AudioFile(const std::string& filePath, const std::string& encoding) : AudioFile() {
    this->Open(filePath, encoding);
}

AudioFile::AudioFile() {
    this->mediaAlias_ = std::to_string(reinterpret_cast<int>(this));
}

AudioFile::~AudioFile() {
    this->Close();
}