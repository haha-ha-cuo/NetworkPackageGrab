#include "TCPDecoder.hpp"
#include <iostream>
#include "Types.hpp"

using namespace std;

void TCPDecoder::packetHandle(const u_char* data_pkt)
{
	tH = (tcpHeader*)data_pkt;
	cout << endl;
	cout << "[Info]------- TCP Header -------" << endl;
	cout << "[Info]Source Port: " << ntohs(tH->sport) << endl;
	cout << "[Info]Destination Port: " << ntohs(tH->dport) << endl;
	cout << "[Info]Sequence Number: " << ntohl(tH->seqnum) << endl;
	cout << "[Info]Acknowledgment Number: " << ntohl(tH->acknum) << endl;
	cout << "[Info]Data Offset: " << (unsigned int)tH->dataOffsetReserved * 4 << " bytes" << endl;
	cout << "[Info]Flags: ";
	if (tH->flags & 0x01) cout << "FIN ";
	if (tH->flags & 0x02) cout << "SYN ";
	if (tH->flags & 0x04) cout << "RST ";
	if (tH->flags & 0x08) cout << "PSH ";
	if (tH->flags & 0x10) cout << "ACK ";
	if (tH->flags & 0x20) cout << "URG ";
	if (tH->flags & 0x40) cout << "ECE ";
	if (tH->flags & 0x80) cout << "CWR ";
	cout << endl;
	cout << "[Info]Window Size: " << ntohs(tH->window) << endl;
	cout << "[Info]Checksum: 0x" << hex << ntohs(tH->checksum) << dec << endl;
	cout << "[Info]Urgent Pointer: " << ntohs(tH->urgptr) << endl;
	cout << "[Info]------------------------------" << endl;
	cout << endl;
}