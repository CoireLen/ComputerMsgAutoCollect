
#include <cstdio>
#include <iostream>
#include <cpuinfo.h>
#include <xlnt/xlnt.hpp>
#include <fstream>
#include <boost/format.hpp>
#include "coirelib.h"
using namespace std;
int main() {

	ComputerMessage cm;
	cout << cm.ComputerName << "   "<< cm.ComputerDomainName <<"   ";
	for (auto i : cm.netmsgs)
	{
		cout << i.device_name << "  " << i.physics_addr << "  " << i.ip << "  " << i.ip_mask;
	}
	cpuinfo_initialize();
	cout << "   " << cpuinfo_get_package(0)->name << "   " << cpuinfo_get_package(0)->core_count;
	cout <<"   "<< cm.RAM.GB()<<"GB";
	
	xlnt::workbook wb;
	ifstream excelfile("电脑信息表-自动获取.xlsx",ios::in|ios::out);
	if (excelfile.good())
	{
		cout << "\n文件存在!" << endl;
	}
	else {
		xlnt::worksheet ws= wb.active_sheet();
		ws.cell("A1").value("ComputerName");
		ws.cell("B1").value("Domain");
		ws.cell("C1").value("NetWorkAdapter");
		ws.cell("D1").value("MAC");
		ws.cell("E1").value("IPv4");
		ws.cell("F1").value("Mask");
		ws.cell("G1").value("CPU Name");
		ws.cell("H1").value("CPU Core");
		ws.cell("I1").value("RAM(GB)");
		wb.save(L"电脑信息表-自动获取.xlsx");
		excelfile.open(L"电脑信息表-自动获取.xlsx", ios::in | ios::out);
	}
	if (excelfile.good()) {
		//数据写入
		excelfile.close();
		wb.load(L"电脑信息表-自动获取.xlsx");
		auto ws = wb.active_sheet();
		unsigned long long sheetlength = ws.rows().length()+1;
		ws.cell((boost::format("A%d") % sheetlength).str()).value(cm.ComputerName.c_str());
		ws.cell((boost::format("B%d") % sheetlength).str()).value(cm.ComputerDomainName.c_str());
		ws.cell((boost::format("G%d") % sheetlength).str()).value(cpuinfo_get_package(0)->name);
		ws.cell((boost::format("H%d") % sheetlength).str()).value(cpuinfo_get_package(0)->core_count);
		ws.cell((boost::format("I%d") % sheetlength).str()).value(cm.RAM.GB());
		for (auto i : cm.netmsgs) {
			ws.cell((boost::format("C%d") % sheetlength).str()).value(i.device_name.c_str());
			ws.cell((boost::format("D%d") % sheetlength).str()).value(i.physics_addr.c_str());
			ws.cell((boost::format("E%d") % sheetlength).str()).value(i.ip.c_str());
			ws.cell((boost::format("F%d") % sheetlength).str()).value(i.ip_mask.c_str());
			sheetlength++;
		}
		wb.save(L"电脑信息表-自动获取.xlsx");
	}
	else {
		cout << "发生了一些错误（excel文件没有读写权限)" << endl;
	}
}