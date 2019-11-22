#include<iostream>
#include<unistd.h>
using namespace std;
class Stop_N_Wait_woutARQ{
	int*pkt;
	int*rec_pkt;
	int size;
	int rcounter;
	public:
		Stop_N_Wait_woutARQ();
		void sender(int);
		void send_pkt(int );
		void reciever(int);
		bool sendAck();
		
};
Stop_N_Wait_woutARQ::Stop_N_Wait_woutARQ(){
	cout<<"\n\t***APPLICATION LAYER*** \n";
	cout<<"Enter number of packets to be sent: ";
	cin>>size;
	
	
	cout<<"\n\n\t***TRANSPORT LAYER*** \n";
	cout<<"\nSegmenting inputted data  \n ";
	sleep(1);
	cout<<"Attaching Port numbers... ";
	sleep(1);
	cout<<"\nSending segments to Network Layer  \n ";
	
	
	cout<<"\n\n\t***NETWORK LAYER*** \n";
	cout<<"\nPacketing recieved segments ...\n ";
	sleep(1);
	cout<<"Attaching IP addresses... ";
	cout<<"\nSending packets to Data Link Layer... ";
	
	cout<<"\n\n\t***DATA LINK LAYER*** \n";
	cout<<"\nRecieving packets from network layer... ";
	sleep(1);
	cout<<"\nFraming packets from network layer... ";
	sleep(1);
	pkt=new int[size];
	rec_pkt=new int[size];
	rcounter=0;
	for(int i=0;i<size;i++)
		pkt[i]=i+1;
	for(int i=0;i<size;i++)
		sender(i);
	cout<<"\n\nData frames recieved: ";
	for(int i=0;i<size;i++)
		cout<<rec_pkt[i]<<" ";
}
void Stop_N_Wait_woutARQ::sender(int no){
		cout<<"\nSending frames "<<pkt[no]<<"..."<<endl;
		send_pkt(pkt[no]);		
		cout<<"Aknowledgement recieved"<<endl;
	
}
void Stop_N_Wait_woutARQ::send_pkt(int pkt_no){
	cout<<"Waiting for Acknowledgement... "<<endl;
	sleep(2);
	reciever(pkt_no);
}
void Stop_N_Wait_woutARQ::reciever(int pkt_no){
	rec_pkt[rcounter]=pkt_no;
	rcounter++;
	sendAck();
}
bool Stop_N_Wait_woutARQ::sendAck(){
	cout<<"Sending Acknowledgement... "<<endl;
	sleep(2);
	return true;
}
int main(){
	Stop_N_Wait_woutARQ u1;
	return 0;	
}

