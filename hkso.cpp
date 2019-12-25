#include <iostream>
#include "hkso.h"

using namespace std;

HWND h = NULL;
LONG nPort=-1;
cv::Mat dst;
// dst = cv::Mat(Size(1920, 1080), CV_8UC3);

// pthread_mutex_t mutex1;
// std::list<cv::Mat> g_frameList;

void CALLBACK DecCBFun(LONG nPort, char *pBuf, LONG nSize, FRAME_INFO *pFrameInfo, void* nReserved1, LONG nReserved2)
{
    long lFrameType = pFrameInfo->nType;
    // cout << "1111111111" <<endl;
    if (lFrameType == T_YV12)
    {
        dst.create(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3);
        cv::Mat src(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, (uchar *)pBuf);
        cv::cvtColor(src, dst, CV_YUV2BGR_YV12);

        // pthread_mutex_lock(&mutex1);
        // g_frameList.push_back(dst);
        // pthread_mutex_unlock(&mutex1);
        // cv::imwrite("temp.jpg", dst);
        // cout << "FrameType ???: 保存." <<endl;
        // pthread_mutex_lock(&mutex1);
        // g_frameList.push_back(dst);
        // pthread_mutex_unlock(&mutex1);
    }
    usleep(100);
}

void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize,void* dwUser)
{
   DWORD dRet;
//    cout << "1111111111" <<endl;
   switch (dwDataType)
   {
    case NET_DVR_SYSHEAD:           //系统头
    if (!PlayM4_GetPort(&nPort))  //获取播放库未使用的通道号
    {
        break;
    }
    if (dwBufSize > 0) {
        if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME)) 
        {
            dRet = PlayM4_GetLastError(nPort);
            break;
        }
        if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, 1024 * 1024)) {
            dRet = PlayM4_GetLastError(nPort);
            break;
        }
        //设置解码回调函数 解码且显示
        if (!PlayM4_SetDecCallBackEx(nPort, DecCBFun, NULL, NULL))
        {
            dRet = PlayM4_GetLastError(nPort);
            break;
        }

        //打开视频解码
        if (!PlayM4_Play(nPort, h))
        {
        dRet = PlayM4_GetLastError(nPort);
        break;
        }

        //打开音频解码, 需要码流是复合流
        if (!PlayM4_PlaySound(nPort)) {
        dRet = PlayM4_GetLastError(nPort);
        break;
        }
    }
    break;
    //usleep(500);
    case NET_DVR_STREAMDATA:  //码流数据
    if (dwBufSize > 0 && nPort != -1) {
        BOOL inData = PlayM4_InputData(nPort, pBuffer, dwBufSize);
        while (!inData) {
        sleep(100);
        inData = PlayM4_InputData(nPort, pBuffer, dwBufSize);
        std::cout << "PlayM4_InputData failed \n" << std::endl;
        }
    }
    break;
   }
}

HKIPCamera::HKIPCamera(char *IP, int Port, char *UserName, char *Password)
{
    m_ip = IP;
    m_port = Port;
    m_username = UserName;
    m_password = Password;
    cout << "登陆信息: "<<endl;
    cout << "\tIP: \t" << m_ip <<endl;
    cout << "\tPort: \t" << m_port <<endl;
    cout << "\tUserName: \t" << m_username <<endl;
    cout << "\tPassword: \t" << m_password <<endl;
}

bool HKIPCamera::login()
{

    int x = NET_DVR_Init();
    cout << x <<endl;
    // NET_DVR_SetConnectTime(2000, 1);
    // NET_DVR_SetReconnect(1000, true);
    NET_DVR_SetLogToFile(0, "./sdkLog");
    NET_DVR_DEVICEINFO_V30 struDeviceInfo = {0};
    // NET_DVR_SetRecvTimeOut(5000);
    m_lUserID = NET_DVR_Login_V30(m_ip, m_port, m_username, m_password, &struDeviceInfo);
    if (m_lUserID >= 0)
    {
        cout << "Login sucessful , Login ID = " << m_lUserID << endl;
        // //设置异常消息回调函数
        // NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
        return 1;
    }  
    else
    {
        cout << "login wrong" <<endl;
        cout << "Login ID = " << m_lUserID << "; Error Code = " << NET_DVR_GetLastError() << endl;
        return 0;
    }
}

bool HKIPCamera::open()
{
    // long lRealPlayHandle;
    NET_DVR_CLIENTINFO ClientInfo = {0};

    ClientInfo.lChannel     = 1;
    ClientInfo.lLinkMode    = 0;
    ClientInfo.hPlayWnd     = 0;
    ClientInfo.sMultiCastIP = NULL;

    //lRealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, PsDataCallBack, NULL, 0);
    // pthread_mutex_init(&mutex1, NULL);
    m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID, &ClientInfo, g_RealDataCallBack_V30, NULL, 0);
    if (m_lRealPlayHandle < 0)
    {
        cout << "pyd1---NET_DVR_RealPlay_V30 error\n" <<endl;
        NET_DVR_Logout(m_lUserID);
        NET_DVR_Cleanup();
        return 0;
    }
    cout << "realplay start." <<endl;
    // cv::namedWindow("HKcamera", CV_WINDOW_NORMAL);
    // cv::resizeWindow("HKcamera", 640, 360);

    return 1;
}

bool HKIPCamera::close()
{
    NET_DVR_StopRealPlay(m_lRealPlayHandle);
    // pthread_mutex_destroy(&mutex1);

    return 1;
}

bool HKIPCamera::logout()
{
    NET_DVR_Logout(m_lUserID);
    NET_DVR_Cleanup();
    cout << "finished!" <<endl;
    return 1;
}

bool HKIPCamera::start()
{
    return login() && open();
}

bool HKIPCamera::stop()
{
    return close() && logout();
}

cv::Mat HKIPCamera::current(int rows, int cols)
{
    // cvImg = imread("/home/scarlet/Pictures/wallpaper.png");
    // cvImg = wall;
    cv::Mat frame;
    resize(dst, frame, cv::Size(cols, rows));
    return frame;
}

// ======================C/Python Interface defined========================
HKIPCamera *HKIPCamera_init(char *ip, int port, char *name, char *pass) 
{
    return new HKIPCamera(ip, port, name, pass); 
}
int HKIPCamera_start(HKIPCamera *hkcp) { return hkcp->start(); }
int HKIPCamera_stop(HKIPCamera *hkcp) { return hkcp->stop(); }
void HKIPCamera_frame(HKIPCamera *hkcp, int rows, int cols, unsigned char *frompy)
{
    // START_TIMER
    memcpy(frompy, (hkcp->current(rows, cols)).data, rows * cols * 3);
    // STOP_TIMER("HKIPCamera_frame")
}
