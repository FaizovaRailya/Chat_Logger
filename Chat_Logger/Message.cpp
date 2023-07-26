#include "Message.h"
#include <string>
#include <ctime>
#include <chrono>

Message::Message(const std::string& fromMessage, const std::string& toMessage, const std::string& text, std::time_t& timestamp) :
    fromMessage_(fromMessage), toMessage_(toMessage), text_(text), timestamp_(timestamp) {}

const std::string& Message::getFromMessage() const { return fromMessage_; }
const std::string& Message::getToMessage() const { return toMessage_; }
const std::string& Message::getText() const { return text_; }
const std::time_t Message::getTime() const { return timestamp_; }

void Message::setFromMessage(const std::string& msg_from) { fromMessage_ = msg_from; }
void Message::setToMessage(const std::string& msg_to) { toMessage_ = msg_to; }
void Message::setText(const std::string& msg_text) { text_ = msg_text; }
void Message::setTime(const std::time_t& msg_timestamp) { timestamp_ = msg_timestamp; }
