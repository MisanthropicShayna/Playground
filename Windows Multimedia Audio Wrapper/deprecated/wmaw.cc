#include "headers/wmaw.hh"

MciError::MciError(uint32_t const& errorCode)

  // MciCommand class definitions.
 //
// ------------------------------------------------------------------------------------------------------ */

void MciCommand::execute() const {
    return mciSendString(this->str().c_str(), NULL, 0, NULL);
}

  // AudioFile class definitions.
 //
// ------------------------------------------------------------------------------------------------------ */

void AudioFile::Open(std::string const& path, AudioFormat const& format) {
    this->lastError_ = NULL;
    this->lastErrorStr_ = std::string();
    this->fileAlias_ = std::to_string(reinterpret_cast<int>(this));
    this->filePath_ = file;
    this->format_ = format;
    
    this->formatStr_ = [&format]() -> std::string {
        char const* format_strings[2] = {"mpegvideo", "waveform"};
        return format_strings[static_cast<int>(format)];        
    }();
    
    MciCommand command;
    command << "open \"" << path << "\" type " << this->formatStr_ << " alias " << this->fileAlias_;
    uint32_t error = command.execute();
    
    if(error) {
        
    }
}

void AudioFile::Close() {
    
}


AudioFile::AudioFile(std::string const& file, AudioFormat const& format) {
    this->Open(file, format);
}

AudioFile::AudioFile() {}

AudioFile::~AudioFile() {
    
}


#ifdef OLD

MediaException::MediaException(char* message) {
    this->message_ = (char*)malloc(strlen(message)+1);
    memcpy(this->message_, message, strlen(message)+1);
}

void AudioFile::StartPlaybackAsync(double from, double to) {
    std::stringstream command;
    command << "play \"" << this->audioAlias_ << "\" from " << from;
    if(to) command << " to " << to;
    
    this->lastError_ = mciSendString(command.str().c_str(), NULL, 0, NULL);
    
    if(this->lastError_) {
        char error_message[128];
        mciGetErrorString(this->lastError_, error_message, 128);
        
        this->lastErrorStr_ = (char*)malloc(strlen(error_message)+1);
        memcpy(this->lastErrorStr_, error_message, strlen(error_message)+1);
        
        throw MediaException(this->lastErrorStr_);
    }
}

AudioFile::AudioFile(const char* path, AudioFormat format) {
    this->audioAlias_ = (char*)malloc(sizeof(char)*(int)log10((int)this)+1);
    sprintf(this->audioAlias_, "%d", (int)this);
    
    this->audioPath_ = (char*)malloc(strlen(path)+1);
    memcpy(this->audioPath_, path, strlen(path)+1);

    std::stringstream command;
    command << "open \"" << path << "\"";
    command << " type " << this->formatStrings_[static_cast<int>(format)];
    command << " alias " << this->audioAlias_;
    
    mciSendString(command.str().c_str(), NULL, 0, NULL);
}

AudioFile::~AudioFile() {
    
    free(this->audioAlias_);
    free(this->audioPath_);    
}

#endif