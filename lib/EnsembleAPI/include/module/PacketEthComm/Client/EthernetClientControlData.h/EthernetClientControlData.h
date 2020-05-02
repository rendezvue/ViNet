#ifndef ETHERNETCLIENT_CONTROL_DATA_H
#define ETHERNETCLIENT_CONTROL_DATA_H

#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdint.h>

#include <chrono>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

//boost 
#include <boost/asio.hpp>
#include <boost/thread.hpp>


#include "RendezvueCheckData.h"

#include "opencv2/opencv.hpp"

#include "EnsembleCommon.h"

//#include <qdebug.h>


#define DEFAULT_BUFLEN 4096
#define DEFAULT_BUFLEN2 1024

//#define COMMAND_BUF_SIZE	1024
#define DATA_BUF_SIZE	1000000*100		//100Mega //////20Mega * 3(RGB) = 60Mega
						
using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace std;

class  CEthernetClientControlData
{
public:
	// 싱글턴 인스턴스를 반환할 멤버함수
	static CEthernetClientControlData* getInstance();

	// 소멸자, instanceFlag를 false로 변경
 	virtual ~CEthernetClientControlData() ;
	
	int Send(tcp::socket *soc, const unsigned int command, const std::string id, std::vector<float> *p_vec_send_data);
	int SendNoData(tcp::socket *soc, const unsigned int command, const std::string id);
	int SendString(tcp::socket *soc, const unsigned int command, const std::string id, const std::string str);
	int Receive(tcp::socket *soc, const unsigned int command, std::vector<float>* out_receive_data) ;
	int Receive(tcp::socket *soc, std::string *out_str_id, std::vector<float>* out_receive_data) ;

	int SendImage(tcp::socket *soc, const unsigned int command, cv::Mat image) ;
	int SendImage(tcp::socket *soc, const unsigned int command, const int width, const int height, const int image_type, unsigned char* image_buf, const int buf_len);
    int ReceiveImage(tcp::socket *soc, const unsigned int command, int& width, int& height, ImageBuf* out_buf) ;
	
	int Send(tcp::socket *soc, unsigned int command, unsigned char* send_data, const unsigned int send_data_size, const unsigned int send_scalefactor, unsigned char** out_data, int* out_data_size, unsigned int* out_scalefactor);
private:

	/**
	 * 생성자
	 * 외부에서 인스턴스를 생성할 수 없도록 private 영역에 정의하고 구현함
	 * 원리: private 영역에 생성자가 존재하므로 외부에서 SingletonClass()를 호출할 수 없음
	 */
	CEthernetClientControlData(void) ;
	
	// 싱글턴 인스턴스가 생성되었는지 여부
	static bool instanceFlag;

	// 싱글턴 인스턴스
	static CEthernetClientControlData* instance;

private:
	//char *m_buf;
	unsigned char m_buf[DEFAULT_BUFLEN2];

	//CCheckData m_cls_check_data;
	CRendezvueCheckData m_cls_check_data;

	unsigned char *m_p_command ;
	unsigned int m_ui_command_size ;

	//use jpg
	std::vector<uchar> m_jpg_buf ;
		
	boost::mutex m_mutex_recv;
	boost::mutex m_mutex_send;

    //
    void OnAsyncReceive(const boost::system::error_code& ErrorCode, std::size_t bytes_transferred ) ;
};

#endif
