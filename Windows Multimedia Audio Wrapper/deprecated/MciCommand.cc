#include "headers/wmaw.hh"

uint32_t MciCommand::Execute() const {
    return mciSendString(this->str().c_str(), NULL, 0, NULL);
}