#include "coirelib.h"
ComputerMessage::ComputerMessage()
/*
初始化数据
计算机名称-

*/
{
	//获取计算机名称
	DWORD computernamelength = 0;
	GetComputerNameA(NULL, &computernamelength);
	ComputerName.resize(computernamelength + 1);
	GetComputerNameA(const_cast<char*>(ComputerName.c_str()), &computernamelength);
	//获取计算机域名
	DWORD computerDomainNameLength=0;
	GetComputerNameExA(ComputerNamePhysicalDnsDomain, NULL, &computerDomainNameLength);
	ComputerDomainName.resize(computerDomainNameLength + 1);
	GetComputerNameExA(ComputerNamePhysicalDnsDomain, const_cast<char*>(ComputerDomainName.c_str()), &computerDomainNameLength);
	//---------------------------------------------------------
	//获取网卡配置
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	int netCardNum = 0;
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	PIP_ADAPTER_INFO freepIpAdapterInfo = pIpAdapterInfo;
	std::string strMAC;
	std::string strLocalIp;
	std::string strWLANIp;
	strMAC.resize(20);
	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo)
		{
			//可能网卡有多IP,因此通过循环去判断
			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			if (pIpAddrString)
			{
				sprintf_s(const_cast<char*>(strMAC.c_str()), 20, "%02X-%02X-%02X-%02X-%02X-%02X",
					pIpAdapterInfo->Address[0], pIpAdapterInfo->Address[1], pIpAdapterInfo->Address[2], pIpAdapterInfo->Address[3], pIpAdapterInfo->Address[4], pIpAdapterInfo->Address[5]);

				//包含以下MAC地址的前8个字节（前3段）是虚拟网卡
				//"00:05:69"; //vmware1
				//"00:0C:29"; //vmware2
				//"00:50:56"; //vmware3
				//"00:1c:14"; //vmware4
				//"00:1C:42"; //parallels1
				//"00:03:FF"; //microsoft virtual pc
				//"00:0F:4B"; //virtual iron 4
				//"00:16:3E"; //red hat xen , oracle vm , xen source, novell xen
				//"08:00:27"; //virtualbox

				// 说明是本地网络
				if (pIpAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
				{
					if (!strstr(pIpAdapterInfo->Description, "Virtual")||visual_network_card)
					{
						std::string strIp = pIpAddrString->IpAddress.String;
						if (strIp.compare("0.0.0.0") != 0)
						{
							netmsgs.push_back({std::string(pIpAdapterInfo->Description ),strMAC,strIp,std::string(pIpAdapterInfo->IpAddressList.IpMask.String)});
						}
					}
				}

				// 说明是无线网络
				if (pIpAdapterInfo->Type == 71)
				{
					std::string strIp = pIpAddrString->IpAddress.String;
					if (strIp.compare("0.0.0.0") != 0)
					{
						netmsgs.push_back({ std::string(pIpAdapterInfo->Description),strMAC,strIp,std::string(pIpAdapterInfo->IpAddressList.IpMask.String) });
					}
				}
			}
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	delete freepIpAdapterInfo;

}

ComputerMessage::~ComputerMessage()
{

}
RAMsize::RAMsize()
{
	MEMORYSTATUSEX mem;
	mem.dwLength = sizeof(mem);
	GlobalMemoryStatusEx(&mem);
	byte = mem.ullTotalPhys;
}

RAMsize::~RAMsize()
{
}