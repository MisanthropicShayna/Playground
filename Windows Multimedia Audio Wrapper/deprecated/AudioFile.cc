#include "headers/wmaw.hh"

std::string const& operator=(std::string const& str, AudioFormat const& format) {
    const static std::string formatStrings[2] = {
        "mpegvideo",
        "waveform"        
    };
    
    return formatStrings[static_cast<int>(format)];
}

void AudioFile::StartPlayback(double start, double end, bool async) {
    MciCommand command;
    command << "play " << this->fileAlias_ << " from " << start;
    if(end) command << " to " << end;
    if(!async) command << " wait";
    
    uint32_t errorCode = command.execute();
    
    if(errorCode) {
        MciError mciError(errorCode);
        this->lastMciError_ = mciError;
        throw mciError;
    }
}

void AudioFile::PausePlayback() {
    MciCommand command;
    command << "pause " << this->fileAlias_:
    uint32_t errorCode = command.execute();
    
    if(errorCode) {
        MciError mciError(errorCode);
        this->lastMciError_ = mciError;
        throw mciError;
    }
}

void AudioFile::ResumePlayback() {
    MciCommand command;
    command << "resume " << this->fileAlias_;
    uint32_t errorCode = command.execute();
    
    if(errorCode) {
        MciError mciError(errorCode);
        this->lastMciError_ = mciError;
        throw mciError;
    }
}

void AudioFile::StopPlayback() {
    MciCommand command;
    command << "stop " << this->fileAlias_;
    uint32_t errorCode = command.execute();
    
    if(errorCode) {
        MciError mciError(errorCode);
        this->lastMciError_ = mciError;
        throw mciError;
    }
}

void AudioFile::Open(std::string const& path, AudioFormat const& format) {
    this->fileAlias_ = std::to_string(reinterpret_cast<int>(this));
    this->filePath_ = path;
    this->format_ = format;
    this->formatStr_ = format;
    
    MciCommand command;
    command << "open \"" << path << "\" type " << this->formatStr_ << " alias " << this->fileAlias_;
    uint32_t error = command.execute();
    
    if(error) {
        MciError mciError(error);
        this->lastMciError_ = mciError;
        throw mciError;
    }
}

void AudioFile::Close() {
    this->StopPlayback();
    
    MciCommand command;
    command << "close " << this->fileAlias_;
    command.execute();
}

AudioFile::AudioFile(std::string const& path, AudioFormat const& format) {
    this->Open(path, format);
}

AudioFile::AudioFile() {
    this->fileLoaded_ = false;
    this->lastError_ = 0;
}

AudioFile::~AudioFile() {
    this->Close();
}