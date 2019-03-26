# NetMonitor
This Tool used for Capture TCP or UDP Package on Windows

本工具用来在Windows系统上抓取TCP或UDP报文。

使用步骤
1. 程序依赖Winpcap的驱动，所以在使用前先在系统上安装Wireshark(图简便，Wireshark中只带Winpcap驱动)
2. 设置配置文件中的显卡ID和抓包用的过滤表达式，配置文件全文如下：

[BASE]
#运行模式，0手动模式，1自动模式
mode = 1

#以太网卡序号
ethernet_id = 3

#设定网络监控程序的日志路径，路径可以设定一层或多层路径。比如：路径为“D:A”或"D:\A\B"或 "D:A\B/C"或“D:/A\B/C\D”
logpath = D:NetMonitorLog/B1\A2

#日志记录的模式，ASCII字符串，值为0， HEX字符串，值为1
charsetmode = 1

#过滤表达式
filter_express = udp and port 5555
#filter_express = tcp and port 9010

3. 运行程序，抓包到的数据会保存到日志文件里
