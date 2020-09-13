#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <iphlpapi.h>
#include <cstdlib>
#include <string>
#include <vector>
#pragma comment(lib,"iphlpapi.lib")
struct NetMsg {
	std::string device_name;
	std::string physics_addr;
	std::string ip;
	std::string ip_mask;
};
class RAMsize
{
public:
	
	RAMsize();
	double GB() { return MB() / 1024.0; };
	double MB() { return KB() / 1024.0; };
	double KB() { return byte / 1024.0; };
	double Byte() { return byte; };
	~RAMsize();

private:
	unsigned long long byte;
};


class ComputerMessage
{
public:
	ComputerMessage();
	~ComputerMessage();
	RAMsize RAM;
//private:
	bool visual_network_card = false;//¼ÇÂ¼ÐéÄâÍø¿¨ÐÅÏ¢
	std::string ComputerName;
	std::string ComputerDomainName;
	std::vector<NetMsg> netmsgs;
};

