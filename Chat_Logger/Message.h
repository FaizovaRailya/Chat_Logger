#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Message {
	std::string fromMessage_;
	std::string toMessage_;
	std::string text_;
	std::time_t timestamp_;
public:
	Message() = default;
	Message(const std::string& fromMessage, const std::string& toMessage, const std::string& text, std::time_t& timestamp);
	~Message() = default;

	const std::string& getFromMessage() const;
	const std::string& getToMessage() const;
	const std::string& getText() const;
	const std::time_t getTime() const;

	void setFromMessage(const std::string& msg_from);
	void setToMessage(const std::string& msg_to);
	void setText(const std::string& msg_text);
	void setTime(const std::time_t& msg_time);


};