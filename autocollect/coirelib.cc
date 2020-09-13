#include "coirelib.h"
ComputerMessage::ComputerMessage()
/*
��ʼ������
���������-

*/
{
	//��ȡ���������
	DWORD computernamelength = 0;
	GetComputerNameA(NULL, &computernamelength);
	ComputerName.resize(computernamelength + 1);
	GetComputerNameA(const_cast<char*>(ComputerName.c_str()), &computernamelength);
	//��ȡ���������
	DWORD computerDomainNameLength=0;
	GetComputerNameExA(ComputerNamePhysicalDnsDomain, NULL, &computerDomainNameLength);
	ComputerDomainName.resize(computerDomainNameLength + 1);
	GetComputerNameExA(ComputerNamePhysicalDnsDomain, const_cast<char*>(ComputerDomainName.c_str()), &computerDomainNameLength);
	//---------------------------------------------------------
	//��ȡ��������
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	int netCardNum = 0;
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	PIP_ADAPTER_INFO freepIpAdapterInfo = pIpAdapterInfo;
	std::string strMAC;
	std::string strLocalIp;
	std::string strWLANIp;
	strMAC.resize(20);
	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		while (pIpAdapterInfo)
		{
			//���������ж�IP,���ͨ��ѭ��ȥ�ж�
			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			if (pIpAddrString)
			{
				sprintf_s(const_cast<char*>(strMAC.c_str()), 20, "%02X-%02X-%02X-%02X-%02X-%02X",
					pIpAdapterInfo->Address[0], pIpAdapterInfo->Address[1], pIpAdapterInfo->Address[2], pIpAdapterInfo->Address[3], pIpAdapterInfo->Address[4], pIpAdapterInfo->Address[5]);

				//��������MAC��ַ��ǰ8���ֽڣ�ǰ3�Σ�����������
				//"00:05:69"; //vmware1
				//"00:0C:29"; //vmware2
				//"00:50:56"; //vmware3
				//"00:1c:14"; //vmware4
				//"00:1C:42"; //parallels1
				//"00:03:FF"; //microsoft virtual pc
				//"00:0F:4B"; //virtual iron 4
				//"00:16:3E"; //red hat xen , oracle vm , xen source, novell xen
				//"08:00:27"; //virtualbox

				// ˵���Ǳ�������
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

				// ˵������������
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