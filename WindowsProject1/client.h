#pragma once
#ifndef CLIENT_H
#define CLIENT_H
//ģ��ͻ���
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

using msg_deque = std::deque<message>; //��Ϣ����

//�ͻ�����
class client {
public:
	client(boost::asio::io_service &io_service,
		tcp::resolver::iterator endpoint_iter)
		: io_service_(io_service),
		socket_(io_service),
		roomId_(0){
		//���ӵ�������
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

	//�������������Ϣ
	template <typename T>
	void write(MessageType messageType, T &msg, std::function<void()> fun = std::function<void()>()) {
		
		fun_ = fun;

		message sendMsg;
		std::string serializeMsg;

		if (!msg.SerializeToString(&serializeMsg)) {
			std::cout << "���л�ʧ��" << std::endl;
			system("pause");
			exit(-1);
		}

		sendMsg.setMessage(messageType, serializeMsg); //����Ҫ���͵���Ϣ

		//io_service_.post([this, &msg] { start_write(msg); });
		start_write(sendMsg);
	}

	//������Ϸ��ʼ����
	void SetStartGameFun(std::function<void()> fun) {
		startGameFun_ = fun;
	}

	//�����ܵ��˺�����
	void SetHurtFun(std::function<void()> fun) {
		hurtFun_ = fun;
	}

	//����ˢ�º���
	void SetRenovateFun(std::function<void()> fun) {
		renovateFun_ = fun;
	}

	//��ʼ��������Ϣ
	void InitRoomInfo() {
		room_set_.clear();
		room_people_num_vec_.clear();
		room_status_vec_.clear();
	}

	//�رտͻ���socket
	void close() {
		io_service_.post([this] { do_close(); });
	}

	//���ط����б�
	const std::set<_int32>& room_set() const {
		return room_set_;
	}

	//���ط��������б�
	const std::vector<_int32>& room_people_num_vec() const {
		return room_people_num_vec_;
	}

	//���ط���״̬�б�
	const std::vector<bool>& room_status_vec() const {
		return room_status_vec_;
	}

	//���ص�ǰ���ڷ����
	const _int32 roomId() const {
		return roomId_;
	}

	//�޸ķ����
	void roomId(_int32 id) {
		roomId_ = id;
	}

private:
	//��ȡ��Ϣͷ��
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

	//��ȡ��Ϣ����
	void do_read_body() {
		boost::asio::async_read(
			socket_,
			boost::asio::buffer(
				read_msg_.body(), read_msg_.bodyLength()),
			[this](auto &error, size_t /*length*/) {
				if (!error) {
					handle_message(); //������Ϣ

					do_read_header();
				}
				else {
					std::cout << error.message() << std::endl;
					do_close();
				}
			});
	}

	//�����յ�����Ϣ
	void handle_message() {
		switch (read_msg_.type())
		{
		case SERVER_MSG: //�����������Ϣ
			dispose_server();
			break;
		case GAME_MSG: //��Ϸ�����Ϣ
			dispose_game();
			break;
		}
	}

	//�����л���Ϣ
	template<typename T>
	bool ParseMessage(T &msg) {
		bool ok = msg.ParseFromString(
			std::string(
				read_msg_.body(),
				read_msg_.bodyLength()));

		return ok;
	}

	//�����������ش���
	void dispose_server() {

		ServerMsg serverMsg;
		if (!ParseMessage(serverMsg)) {
			std::cout << "severMsg:��Ϣ����ʧ��" << std::endl;
			return;
		}

		switch (serverMsg.msgtype()) {
		case CREATE_SERVER_MSG:  //����room
		{
			roomId_ = serverMsg.serverid(); //�޸ĵ�ǰ�����
			if (fun_) {
				fun_();
				fun_ = std::function<void()>();
			}
		}
		break;
		case JOIN_SERVER_MSG:	 //����room
			if (fun_) {
				fun_();
				fun_ = std::function<void()>();
			}
			break;
		case LEAVE_SERVER_MSG:	 //�뿪room
			break;
		case GET_ROOM_LIST_MSG:  //��ȡ�����б�
		{
			for (int i = 1; i < serverMsg.roomlist_size(); ++i) {
				room_set_.insert(serverMsg.roomlist(i));
				room_people_num_vec_.push_back(serverMsg.roompeoplenum(i));
				room_status_vec_.push_back(serverMsg.roomstatus(i));
			}

			if (fun_) {
				fun_(); //�ص�ˢ��
				fun_ = std::function<void()>(); //����
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

	//������Ϸ��ش���
	void dispose_game() {

		GameMsg gameMsg;
		if (!ParseMessage(gameMsg)) {
			std::cout << "gameMsg:��Ϣ����ʧ��" << std::endl;
			return;
		}

		switch (gameMsg.msgtype())
		{
		case GAME_START_MSG: //��Ϸ��ʼ
			assert(startGameFun_); //startGameFun_���������ЧĿ��
			startGameFun_();
			break;
		case ATK_FOE_MSG:	//������
			assert(hurtFun_); //hurtFun_���������ЧĿ��
			hurtFun_();
			break;
		case GAME_OVER_MSG: //��Ϸ����
		{
			mciSendString(L"open ./src/ʤ��.mp3", NULL, 0, 0);
			mciSendString(L"play ./src/ʤ��.mp3", NULL, 0, 0);
			MessageBox(NULL, L"You Win!", L"Message", MB_OK);
		}
		break;
		default:
			assert(true);
		}
	}

	//����Ϣ�������Ϣ����
	void start_write(const message &msg) {
		bool isWriteing = !write_msg_deque_.empty(); //���ڷ�����Ϣ...
		write_msg_deque_.push_back(msg);

		//��������ڷ�����Ϣ,��do_write
		if (!isWriteing)
			do_write();
	}

	//����д��Ϣ�ĵط�
	void do_write() {
		boost::asio::async_write(
			socket_,
			boost::asio::buffer(
				write_msg_deque_.front().data(), write_msg_deque_.front().length()),
			[this](auto &error, size_t /*length*/) {
				if (!error
					&& write_msg_deque_.front().chack_header()) { //��Ϣ����ǰӦ�ü��...
					write_msg_deque_.pop_front(); //ɾ�����ͳ�ȥ����Ϣ
					if (!write_msg_deque_.empty()) {
						do_write(); //��������
					}
				}
				else {
					std::cout << error.message() << std::endl;
				}
			});
	}

	//�ر�socket
	void do_close() {
		socket_.close();
	}

	boost::asio::io_service &io_service_;
	tcp::socket socket_;
	message read_msg_; //��ȡ������Ϣ
	msg_deque write_msg_deque_; //������Ϣ����

	std::set<_int32> room_set_; //�����б�
	std::vector<_int32> room_people_num_vec_; //��������
	std::vector<bool> room_status_vec_; //����״̬
	_int32 roomId_; //��ǰ���ڵķ����
	std::function<void()> fun_; //�ص�����
	std::function<void()> startGameFun_; //��Ϸ��ʼ
	std::function<void()> hurtFun_; //�ܵ��˺�
	std::function<void()> renovateFun_; //ˢ�·����б�
};

////����һ���ͻ�����
//client CreateClient(boost::asio::io_service &io_service) {
//	boost::asio::ip::tcp::resolver resolver_(io_service);
//	boost::asio::ip::tcp::resolver::query query_("127.0.0.1", "9999");
//	client client_(io_service, resolver_.resolve(query_)); //�ͻ�����
//
//	return client_;
//}

#endif // !CLIENT_H