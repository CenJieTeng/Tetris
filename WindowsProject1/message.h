#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <cassert>
#define MAX_BODY_LENGTH 20 //控制消息内容大小

//消息类型
enum MessageType{
	SERVER_MSG = 1,   //服务器相关信息
	GAME_MSG = 2,     //游戏相关信息
	REGISTER_MSG = 3, //注册相关消息
	DEFAULT_MSG = 99  //未定义消息类型
};

//消息头部
struct Header{
	MessageType type_ = DEFAULT_MSG;
	size_t bodyLength;
};

//消息
class message
{
public:
	message() {};
	~message() {
	};

public:
	const char* data() const { return data_; } //返回const data_
	char* data() { return data_; } //返回data_
	static constexpr size_t headerLength() { return (sizeof(Header)); } //返回消息包头部大小
	const MessageType& type() const { return header_.type_; } //返回消息类型type_
	const char* body() const { return (data_ + headerLength()); } //返回const body(消息内容首地址)
	char* body() { return (data_ + headerLength()); } //返回body
	const size_t bodyLength() const { return header_.bodyLength; } //返回消息包实际内容大小
	const size_t length() const { return (headerLength() + bodyLength()); } //返回消息包总大小，header + body

	//设置消息内容
	//消息类型，消息序列化所在的string
	void setMessage(const MessageType mt, std::string serializedStr) {
		header_.type_ = mt; //设置消息类型
		header_.bodyLength = serializedStr.size(); //设置消息大小

		//data_ = new char[headerLength() + serializedStr.size() + 1]; //创建存储空间
		
		memcpy(body(), serializedStr.data(), serializedStr.size()); //设置消息内容
		memcpy(data(), &header_, headerLength());
		//data_[length()] = '\0';
	}

	//对消息包头的一系列检查...
	bool chack_header() {
		(header_.type_ != DEFAULT_MSG); //消息类型不能是未定义(default)的
		//限制消息内容大小
		if (header_.bodyLength > MAX_BODY_LENGTH) {
			header_.bodyLength = MAX_BODY_LENGTH;
		}
	 
		return true;
	}

	//解码
	Header decode() {
		memcpy(&header_, data(), headerLength());
		return header_;
	}

private:
	char data_[100];
	Header header_;
};

#endif // !MESSAGE_H
