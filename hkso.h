#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/types_c.h>
/* #include <QDir> */
#include <fstream>
#include <unistd.h>
#include <pthread.h>
// #include <queue>
#include "HCNetSDK.h"
#include "PlayM4.h"
#include "LinuxPlayM4.h"

#ifndef COORDIN_H_
#define COORDIN_H_
// cv::Mat dst;
// HWND h = NULL;
// LONG nPort=-1;
#endif

#define HPR_ERROR   -1
#define HPR_OK      0
#define USECOLOR    0



class HKIPCamera
{
public:
	bool start();
	bool stop();
	cv::Mat current(int rows = 640, int cols = 360);
    //结构函数
    // FILE *g_pFile = NULL;
    

	HKIPCamera(char *IP, int Port, char *UserName, char *Password);

    friend void CALLBACK DecCBFun(LONG nPort, char *pBuf, LONG nSize, FRAME_INFO *pFrameInfo, void* nReserved1, LONG nReserved2);
    friend void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize,void* dwUser);
    
	// friend std::ostream &operator<<(std::ostream &output, HKIPCamera &hkcp);
	// friend void g_fPlayESCallBack(LONG lPreviewHandle, NET_DVR_PACKET_INFO_EX *pstruPackInfo, void *pUser);

protected:
	bool login();
	bool logout();
	bool open();
	bool close();

private:
	char *m_ip;	  // ip address of the camera
	int m_port;	  // port
	char *m_username; // username
	char *m_password; // password
	long m_lRealPlayHandle;
	long m_lUserID;
};

extern "C"
{
	HKIPCamera *HKIPCamera_init(char *ip, int port, char *name, char *pass);
	int HKIPCamera_start(HKIPCamera *hkcp);
	int HKIPCamera_stop(HKIPCamera *hkcp);
	void HKIPCamera_frame(HKIPCamera *hkcp, int rows, int cols, unsigned char *frompy);
}