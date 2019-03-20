#pragma once
#ifndef CLIENT_H
#define CLIENT_H
//模拟客户端
#include <iostream>
#include <deque>
#include <vector>
#include <thread>
#include <atomic>
#include <boost/asio.hpp>
#include "stdafx.h"
#pragma comment(lib, "WINMM.LIB")
#define TIMER_ID1 1

using boost::asio::ip::tcp;

using msg_deque = std::deque<message>; //消息队列

//客户端类
class client {
public:
	client(boost::asio::io_service &io_service,
		tcp::resolver::iterator endpoint_iter)
		: io_service_(io_service),
		socket_(io_service),
		roomId_(0){
		//连接到服务器
		boost::asio::async_connect(
			socket_,
			endpoint_iter,
			[this](auto &error, auto &iter) {
				if (!error) {
					do_read_header();
				}
				else {
					std::cout << error.message() << std::endl;
				}
			});
	}

	//向服务器发送消息
	template <typename T>
	void write(MessageType messageType, T &msg, std::function<void()> fun = std::function<void()>()) {
		
		fun_ = fun;

		message sendMsg;
		std::string serializeMsg;

		if (!msg.SerializeToString(&serializeMsg)) {
			std::cout << "序列化失败" << std::endl;
			system("pause");
			exit(-1);
		}

		sendMsg.setMessage(messageType, serializeMsg); //设置要发送的消息

		//io_service_.post([this, &msg] { start_write(msg); });
		start_write(sendMsg);
	}

	//设置游戏开始函数
	void SetStartGameFun(std::function<void()> fun) {
		startGameFun_ = fun;
	}

	//设置受到伤害函数
	void SetHurtFun(std::function<void()> fun) {
		hurtFun_ = fun;
	}

	//设置刷新函数
	void SetRenovateFun(std::function<void()> fun) {
		renovateFun_ = fun;
	}

	//初始化房间信息
	void InitRoomInfo() {
		room_set_.clear();
		room_people_num_vec_.clear();
		room_status_vec_.clear();
	}

	//关闭客户端socket
	void close() {
		io_service_.post([this] { do_close(); });
	}

	//返回房间列表
	const std::set<_int32>& room_set() const {
		return room_set_;
	}

	//返回房间人数列表
	const std::vector<_int32>& room_people_num_vec() const {
		return room_people_num_vec_;
	}

	//返回房间状态列表
	const std::vector<bool>& room_status_vec() const {
		return room_status_vec_;
	}

	//返回当前所在房间号
	const _int32 roomId() const {
		return roomId_;
	}

	//修改房间号
	void roomId(_int32 id) {
		roomId_ = id;
	}

private:
	//读取消息头部
	void do_read_header() {
		boost::asio::async_read(
			socket_,
			boost::asio::buffer(
				read_msg_.data(), message::headerLength()),
			[this](auto &error, size_t /*length*/) {
				if (!error) {
					read_msg_.decode();
					do_read_body();
				}
				else {
					std::cout << error.message() << std::endl;
					do_close();
				}
			});
	}

	//读取消息内容
	void do_read_body() {
		boost::asio::async_read(
			socket_,
			boost::asio::buffer(
				read_msg_.body(), read_msg_.bodyLength()),
			[this](auto &error, size_t /*length*/) {
				if (!error) {
					handle_message(); //处理消息

					do_read_header();
				}
				else {
					std::cout << error.message() << std::endl;
					do_close();
				}
			});
	}

	//处理收到的消息
	void handle_message() {
		switch (read_msg_.type())
		{
		case SERVER_MSG: //服务器相关信息
			dispose_server();
			break;
		case GAME_MSG: //游戏相关信息
			dispose_game();
			break;
		}
	}

	//反序列化消息
	template<typename T>
	bool ParseMessage(T &msg) {
		bool ok = msg.ParseFromString(
			std::string(
				read_msg_.body(),
				read_msg_.bodyLength()));

		return ok;
	}

	//处理服务器相关处理
	void dispose_server() {

		ServerMsg serverMsg;
		if (!ParseMessage(serverMsg)) {
			std::cout << "severMsg:消息解析失败" << std::endl;
			return;
		}

		switch (serverMsg.msgtype()) {
		case CREATE_SERVER_MSG:  //创建room
		{
			roomId_ = serverMsg.serverid(); //修改当前房间号
			if (fun_) {
				fun_();
				fun_ = std::function<void()>();
			}
		}
		break;
		case JOIN_SERVER_MSG:	 //加入room
			if (fun_) {
				fun_();
				fun_ = std::function<void()>();
			}
			break;
		case LEAVE_SERVER_MSG:	 //离开room
			break;
		case GET_ROOM_LIST_MSG:  //获取房间列表
		{
			for (int i = 1; i < serverMsg.roomlist_size(); ++i) {
				room_set_.insert(serverMsg.roomlist(i));
				room_people_num_vec_.push_back(serverMsg.roompeoplenum(i));
				room_status_vec_.push_back(serverMsg.roomstatus(i));
			}

			if (fun_) {
				fun_(); //回调刷新
				fun_ = std::function<void()>(); //重置
			}
		}
		break;
		case RENOVATE_ROOM_INFO_MSG:
			assert(renovateFun_); 
			renovateFun_();
			break;
		default:
			assert(true);
		}
	}

	//处理游戏相关处理
	void dispose_game() {

		GameMsg gameMsg;
		if (!ParseMessage(gameMsg)) {
			std::cout << "gameMsg:消息解析失败" << std::endl;
			return;
		}

		switch (gameMsg.msgtype())
		{
		case GAME_START_MSG: //游戏开始
			assert(startGameFun_); //startGameFun_必须包含有效目标
			startGameFun_();
			break;
		case ATK_FOE_MSG:	//被攻击
			assert(hurtFun_); //hurtFun_必须包含有效目标
			hurtFun_();
			break;
		case GAME_OVER_MSG: //游戏结束
		{
			mciSendString(L"open ./src/胜利.mp3", NULL, 0, 0);
			mciSendString(L"play ./src/胜利.mp3", NULL, 0, 0);
			MessageBox(NULL, L"You Win!", L"Message", MB_OK);
		}
		break;
		default:
			assert(true);
		}
	}

	//把消息存放在消息队列
	void start_write(const message &msg) {
		bool isWriteing = !write_msg_deque_.empty(); //正在发送消息...
		write_msg_deque_.push_back(msg);

		//如果不是在发送消息,就do_write
		if (!isWriteing)
			do_write();
	}

	//真正写消息的地方
	void do_write() {
		boost::asio::async_write(
			socket_,
			boost::asio::buffer(
				write_msg_deque_.front().data(), write_msg_deque_.front().length()),
			[this](auto &error, size_t /*length*/) {
				if (!error
					&& write_msg_deque_.front().chack_header()) { //消息发送前应该检查...
					write_msg_deque_.pop_front(); //删除发送出去的消息
					if (!write_msg_deque_.empty()) {
						do_write(); //继续发送
					}
				}
				else {
					std::cout << error.message() << std::endl;
				}
			});
	}

	//关闭socket
	void do_close() {
		socket_.close();
	}

	boost::asio::io_service &io_service_;
	tcp::socket socket_;
	message read_msg_; //读取到的消息
	msg_deque write_msg_deque_; //发送消息队列

	std::set<_int32> room_set_; //房间列表
	std::vector<_int32> room_people_num_vec_; //房间人数
	std::vector<bool> room_status_vec_; //房间状态
	_int32 roomId_; //当前所在的房间号
	std::function<void()> fun_; //回调函数
	std::function<void()> startGameFun_; //游戏开始
	std::function<void()> hurtFun_; //受到伤害
	std::function<void()> renovateFun_; //刷新房间列表
};

////返回一个客户端类
//client CreateClient(boost::asio::io_service &io_service) {
//	boost::asio::ip::tcp::resolver resolver_(io_service);
//	boost::asio::ip::tcp::resolver::query query_("127.0.0.1", "9999");
//	client client_(io_service, resolver_.resolve(query_)); //客户端类
//
//	return client_;
//}

#endif // !CLIENT_H