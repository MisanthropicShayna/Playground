#include "headers/wmaw.hh"

MciError:MciError(uint32_t const& errorCode) {
    if(!errorCode) return;
    
    char buffer[128];
    mciGetErrorString(errorCode, buffer, 128);
    
    this->message_.resize(strlen(buffer), '\0');
    this->message_ = buffer;
    
    this->errorCode_ = errorCode;
}