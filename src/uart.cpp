#include "includes.hpp"

void uartInit(void)
{
    HKEY hKey;
    long ret;
    char portName[256], commName[256];
    ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", 0, KEY_QUERY_VALUE, &hKey);
    if (ret == ERROR_SUCCESS)
    {
        int i = 0;
        DWORD dwLong, dwSize;
        while (TRUE)
        {
            dwLong = dwSize = sizeof(portName);
            ret = RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (unsigned char *)commName, &dwSize);
            if (ret == ERROR_NO_MORE_ITEMS)
            {
                printf("no TTL connected!\n");
                return;
            }
            if (strstr(portName, "Serial"))
            {
                break;
            }
            i++;
        }
    }
    RegCloseKey(hKey);

    sprintf(portName, "\\\\.\\%s", commName);
    uart = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (uart == INVALID_HANDLE_VALUE)
    {
        cout << "open uart failed" << endl;
        return;
    }
    SetupComm(uart, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024
    COMMTIMEOUTS TimeOuts;       //设定读超时
    TimeOuts.ReadIntervalTimeout = 1000;
    TimeOuts.ReadTotalTimeoutMultiplier = 500;
    TimeOuts.ReadTotalTimeoutConstant = 5000; //设定写超时
    TimeOuts.WriteTotalTimeoutMultiplier = 500;
    TimeOuts.WriteTotalTimeoutConstant = 2000;
    SetCommTimeouts(uart, &TimeOuts); //设置超时
    DCB dcb;
    GetCommState(uart, &dcb);
    dcb.BaudRate = 115200;     //波特率为115200
    dcb.ByteSize = 8;          //每个字节有8位
    dcb.Parity = NOPARITY;     //无奇偶校验位
    dcb.StopBits = ONESTOPBIT; //两个停止位
    SetCommState(uart, &dcb);
    PurgeComm(uart, PURGE_TXCLEAR | PURGE_RXCLEAR);
}
void *dispScore(void *arg)
{
    char Tx[100];
    DWORD cnt;
    while (!finish)
    {
        sprintf(Tx, "n%8d", score);
        if (uart)
        {
            WriteFile(uart, Tx, 9, &cnt, NULL);
        }
        Sleep(1000);
    }
    return NULL;
}