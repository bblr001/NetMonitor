#include "stdafx.h"
#include "NetMonitorWrapper.h"
#include "MsgFIFO.h"
#include "MsgList.h"
using namespace std;

NetMonitorWrapper::NetMonitorWrapper()
{
	//myList = new MsgList();
}

NetMonitorWrapper::~NetMonitorWrapper()
{
	//if(myList != NULL)
	//delete myList;
	//myList = NULL;
}

int NetMonitorWrapper::initial()
{
	CString cstr;
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		//exit(1);
		return -1;
	}
	int i = 0;
	/* Print the list */
	devlist.clear();
	for(d=alldevs; d; d=d->next)
	{
		//printf("%d. %s", ++i, d->name);
		cstr.Format("%d. %s", ++i, d->name);
		if (d->description)
		{
			//printf(" (%s)\n", d->description);
			cstr.AppendFormat(" (%s)\n", d->description);
			/*SOCKADDR_IN* s;	
			s = (SOCKADDR_IN*)&d->addresses->addr;
			string ip = inet_ntoa(s->sin_addr);
			int port = ntohs(s->sin_port);
			cstr.AppendFormat(" addr-->ip:%s,port:%d\r\n", ip.c_str(),port);

			s = (SOCKADDR_IN*)&d->addresses->broadaddr;
			ip = inet_ntoa(s->sin_addr);
			port = ntohs(s->sin_port);
			cstr.AppendFormat(" broadaddr-->ip:%s,port:%d\r\n", ip.c_str(),port);

			s = (SOCKADDR_IN*)&d->addresses->dstaddr;
			ip = inet_ntoa(s->sin_addr);
			port = ntohs(s->sin_port);
			cstr.AppendFormat(" dstaddr-->ip:%s,port:%d\r\n", ip.c_str(),port);*/

			/*memcpy(&s, &d->addresses->addr, sizeof(SOCKADDR));
			string ip = inet_ntoa(s.sin_addr);
			int port = ntohs(s.sin_port);
			cstr.AppendFormat("\taddr-->ip:%s,port:%d\r\n", ip.c_str(),port);

			memcpy(&s, &d->addresses->broadaddr, sizeof(SOCKADDR));
			ip = inet_ntoa(s.sin_addr);
			port = ntohs(s.sin_port);
			cstr.AppendFormat("\tbroadaddr-->ip:%s,port:%d\r\n", ip.c_str(),port);

			memcpy(&s, &d->addresses->dstaddr, sizeof(SOCKADDR));
			ip = inet_ntoa(s.sin_addr);
			port = ntohs(s.sin_port);
			cstr.AppendFormat("\tdstaddr-->ip:%s,port:%d\r\n", ip.c_str(),port);*/

		}
		else
		{
			//printf(" (No description available)\n");
			cstr.Append(" (No description available)\n");
		}
		devlist.push_back(cstr.GetString());
	}

	if(i==0)
	{
		//printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		cstr = "\nNo interfaces found! Make sure WinPcap is installed.\n";
		CFIFO::GetInstance()->Write(cstr.GetString());
		return -1;
	}
	
	return 0;
}

int  NetMonitorWrapper::FilterLoop()
{
	int i = 0;
	if(inum < 1 || inum > devlist.size())
	{
		printf("\nAdapter number out of range.\n");
		
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
	
	/* Open the adapter */
	if ((adhandle= pcap_open_live(d->name,	// name of the device
							 65536,			// portion of the packet to capture. 
											// 65536 grants that the whole packet will be captured on all the MACs.
							 1,				// promiscuous mode (nonzero means promiscuous)
							 1000,			// read timeout
							 errbuf			// error buffer
							 )) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* Check the link layer. We support only Ethernet for simplicity. */
	if(pcap_datalink(adhandle) != DLT_EN10MB)
	{
		fprintf(stderr,"\nThis program works only on Ethernet networks.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	if(d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask=0xffffff; 


	//compile the filter
	if (pcap_compile(adhandle, &fcode, filter_exp.c_str(), 1, netmask) <0 )
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	//set the filter
	if (pcap_setfilter(adhandle, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	printf("\nlistening on %s...\n", d->description);
	
	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);
	
	/* start the capture */
	//pcap_loop(adhandle, 0, packet_handler, NULL);
	//pcap_loop(adhandle, 0, got_packet, NULL);
	
	pcap_loop(adhandle, 0, got_packet, (u_char*)this);
	return 0;
}

void got_packet (u_char * args, const struct pcap_pkthdr *header, const u_char * packet)   
{
	static int count = 1;		/* packet counter */
 
	/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;	/* The ethernet header [1] */
	const struct sniff_ip *ip;	/* The IP header */
	const struct sniff_tcp *tcp;	/* The TCP header */
	const struct sniff_udp *udp;	/* The UDP header */
	const u_char *payload;		/* Packet payload */
 
	int size_ip;
	int size_tcp;
	int size_payload;
	time_t local_tv_sec;
	struct tm *ltime;
	char timestr[16] = {0};
	CString cstr = "";
	u_short srcport = 0;
	u_short dstport = 0;
	string sip = "";
	string dip = "";
	/*
	 * unused parameter
	 */
	//(VOID)(args);
	NetMonitorWrapper* nm = (NetMonitorWrapper*)args;
	//printf ("\nPacket number %d:\n", count);
	//cstr.Format("\nPacket number %d:\n", count);
	count++;
	
	local_tv_sec = header->ts.tv_sec;
	ltime=localtime(&local_tv_sec);
	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
	cstr.AppendFormat("%s.%.6d:  \t", timestr, header->ts.tv_usec);
	
	/* define ethernet header */
	ethernet = (struct sniff_ethernet *) (packet);
 
	/* define/compute ip header offset */
	ip = (struct sniff_ip *) (packet + SIZE_ETHERNET);
	size_ip = IP_HL (ip) * 4;
	if (size_ip < 20)
	{
		//printf ("   * Invalid IP header length: %u bytes\n", size_ip);
		cstr.Format("   * Invalid IP header length: %u bytes\n", size_ip);
		MsgList::GetInstance()->Pushback(cstr.GetString());
		return;
	}
 	
	sip.assign(inet_ntoa (ip->ip_src));
	dip.assign(inet_ntoa (ip->ip_dst));

	switch (ip->ip_p)
	{
	case IPPROTO_TCP:
		printf ("   Protocol: TCP\n");
		break;
	case IPPROTO_UDP:
		printf ("  ************* Protocol: UDP *************\n");
		break;
	//return;
	case IPPROTO_ICMP:
		printf ("   Protocol: ICMP\n");
		return;
	case IPPROTO_IP:
		printf ("   Protocol: IP\n");
		return;
	default:
		printf ("   Protocol: unknown\n");
		return;
	}
 
	/*
	*  This packet is TCP.
	*/
	if (ip->ip_p == IPPROTO_TCP)
	{
		/* define/compute tcp header offset */
		tcp = (struct sniff_tcp *) (packet + SIZE_ETHERNET + size_ip);
		size_tcp = TH_OFF (tcp) * 4;
		if (size_tcp < 20)
		{
			printf ("   * Invalid TCP header length: %u bytes\n", size_tcp);
			return;
		}
 
		printf ("   Src port: %d\n", ntohs (tcp->th_sport));
		printf ("   Dst port: %d\n", ntohs (tcp->th_dport));
		srcport = 	ntohs (tcp->th_sport);
		dstport =   ntohs (tcp->th_dport);
			/* define/compute tcp payload (segment) offset */
		payload = (u_char *) (packet + SIZE_ETHERNET + size_ip + size_tcp);
 
			/* compute tcp payload (segment) size */
		size_payload = ntohs (ip->ip_len) - (size_ip + size_tcp);
 
		/*
		* Print payload data; it might be binary, so don't just
		* treat it as a string.
		*/
		cstr.AppendFormat("%s:%d -> %s:%d \t", sip.c_str(), srcport, dip.c_str(), dstport);
		//cstr.AppendFormat("%d -> %d =>\t", srcport, dstport);
		cstr.AppendFormat("size_payload = %d =>\t", size_payload);
		if (size_payload > 0)
		{
			printf ("   Payload (%d bytes):\n", size_payload);
			//print_payload (payload, size_payload, cstr);
			if(nm->GetCharMode()==0)
			{
				print_payload_ASCII(payload, size_payload, cstr);
			}else
			{
				print_payload_HEX(payload, size_payload, cstr);
			}
		}
	}				//end tcp
 
	/*
	*  This packet is UDP.
	*/
	else if (ip->ip_p == IPPROTO_UDP)
	{
		/* define/compute udp header offset */
		udp = (struct sniff_udp *) (packet + SIZE_ETHERNET + size_ip);
		printf ("   Src port: %d\n", ntohs (udp->sport));
		printf ("   Dst port: %d\n", ntohs (udp->dport));
		printf ("udp length:%d\n", ntohs (udp->udp_length));
		printf ("udp sum:%d\n", ntohs (udp->udp_sum));

		srcport = 	ntohs (udp->sport);
		dstport =   ntohs (udp->dport);
			/* define/compute udp payload (segment) offset */
		payload = (u_char *) (packet + SIZE_ETHERNET + size_ip + 8);
		size_payload = ntohs (ip->ip_len) - (size_ip + 8);
		
		cstr.AppendFormat("%s:%d -> %s:%d ", sip.c_str(), srcport, dip.c_str(), dstport);
		//cstr.AppendFormat("%d -> %d  \t", srcport, dstport);
		/*
		* Print payload data; it might be binary, so don't just
		* treat it as a string.
		*/
		cstr.AppendFormat("size_payload = %d =>\t", size_payload);
		if (size_payload > 0)
		{
			printf ("   Payload (%d bytes):\n", size_payload);
			//print_payload (payload, size_payload,cstr);
			if(nm->GetCharMode()==0)
			{
				print_payload_ASCII(payload, size_payload, cstr);
			}else
			{
				print_payload_HEX(payload, size_payload, cstr);
			}
		}
 
	}//end udp

	MsgList::GetInstance()->Pushback(cstr.GetString());
	return;

}

/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
 
void print_ascii (const u_char * payload, int len, int offset, CString &cstr)
{
	char hexarr[3] = {0};
	char asciichar ;
	int i;
	int gap;
	const u_char *ch;
	
	/* ascii (if printable) */
	ch = payload;
	for (i = 0; i < len; i++)
	{
		if (isprint (*ch))
		{
			cstr.AppendChar(*ch);
		}
		else
			//printf (".");
			cstr.AppendChar('.');
		ch++;
	}
 
	return;
}
 
void print_hex(const u_char * payload, int len, int offset, CString &cstr)
{
	char hexarr[3] = {0};
	char asciichar ;
	int i;
	int gap;
	const u_char *ch;

	/* hex */
	ch = payload;
	for (i = 0; i < len; i++)
	{	
		sprintf(hexarr, "%02x", *ch);
		cstr.Append(hexarr);
		cstr.Append(" ");
		ch++;
			/* print extra space after 8th byte for visual aid */
		//if (i%7==0)
		//	//printf (" ");
		//	cstr.Append(" ");
		//if (i%15==0)
		//	cstr.Append("\t");
	}
	/* print space to handle line less than 8 bytes */
	//if (len < 8)
	//	//printf (" ");
	//	cstr.Append(" ");
 //
	///* fill hex gap with spaces if not full line */
	//if (len < 16)
	//{
	//	gap = 16 - len;
	//	for (i = 0; i < gap; i++)
	//	{
	//		//printf ("   ");
	//		cstr.Append("   ");
	//	}
	//}
	//printf ("   ");
	//cstr.Append("   ");
}
/*
 * print packet payload data (avoid printing binary data)
 */ 
void print_payload (const u_char * payload, int len, CString &cstr)
{
 
	int len_rem = len;
	int line_width = 16;		/* number of bytes per line */
	int line_len;
	int offset = 0;		/* zero-based offset counter */
	const u_char *ch = payload;
 
	if (len <= 0)
		return;
#if 0
	/* data fits on one line */
	if (len <= line_width)
	{
		print_hex_ascii_line (ch, len, offset, cstr);
		return;
	}
 
	/* data spans multiple lines */
	for (;;)
	{
		/* compute current line length */
		line_len = line_width % len_rem;
		/* print line */
		print_hex_ascii_line (ch, line_len, offset, cstr);
		/* compute total remaining */
		len_rem = len_rem - line_len;
		/* shift pointer to remaining bytes to print */
		ch = ch + line_len;
		/* add offset */
		offset = offset + line_width;
		/* check if we have line width chars or less */
		if (len_rem <= line_width)
		{
			/* print last line and get out */
			print_hex_ascii_line (ch, len_rem, offset, cstr);
			break;
		}
	}
#else
	/*if(charmode == 0)
		print_ascii (ch, len_rem, offset, cstr);
	else
		print_hex(ch, len_rem, offset, cstr);*/
#endif
	return;
}

void print_payload_ASCII (const u_char * payload, int len, CString &cstr)
{
	int len_rem = len;
	int line_width = 16;		/* number of bytes per line */
	int line_len;
	int offset = 0;		/* zero-based offset counter */
	const u_char *ch = payload;
 
	if (len <= 0)
		return;

	print_ascii (ch, len_rem, offset, cstr);
	return;
}

void print_payload_HEX (const u_char * payload, int len, CString &cstr)
{
	int len_rem = len;
	int line_width = 16;		/* number of bytes per line */
	int line_len;
	int offset = 0;		/* zero-based offset counter */
	const u_char *ch = payload;
 
	if (len <= 0)
		return;

	print_hex (ch, len_rem, offset, cstr);
	return;
}