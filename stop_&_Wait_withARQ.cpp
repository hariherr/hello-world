#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<time.h>
using namespace std;
class Stop_N_Wait_withARQ{
	int*pkt;
	int*rec_pkt;
	int size;
	int rcounter;
	int ack_no;
	int ts;
	bool isRec,isPktLost,isAckLost;
	public:
		Stop_N_Wait_withARQ();
		void sender();
		int send_pkt(int,int );
		int reciever(int,int);
};
Stop_N_Wait_withARQ::Stop_N_Wait_withARQ(){
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
	rec_pkt=new int[size+3];
	rcounter=0;
	ts=5;
	ack_no=-1;						//arbitary value	
	srand(time(0));					//to start rand
	isRec=false;					//flag for when all packets are recieved 
	isPktLost=isAckLost=false;
	for(int i=0;i<size;i++)
		pkt[i]=i+1;                           
	sender();
	cout<<"\n\nData frames recieved: ";
	for(int i=0;i<rcounter;i++)
		cout<<rec_pkt[i]<<" ";
}
void Stop_N_Wait_withARQ::sender(){
	int i=0;
	int seq_no,next_pkt;
	bool istime=true,isAck=true;					//isAck is initialised with true so that first pkt can get into
	
	do{
		if(!isAck||!istime){				//resend when either or both of ack is not recieved or timer goes off
											//condition always false for first packet
			next_pkt=i-1;
			cout<<"resending frame "<<pkt[next_pkt]<<endl;
		}	
		else{								//sending next packet
			next_pkt=i;
			i++;
		}
		
		seq_no=next_pkt;
		cout<<"\n frame "<<pkt[next_pkt]<<", 			frame_seq_no. "<<seq_no<<" 		sent"<<endl;
		ack_no=send_pkt(pkt[next_pkt],seq_no);			
		isAck=false;						//send next pkt only if ack recieved ie isAck=true,as we are working with current pkt, isAck=false
		istime=true;
		ts=5;								//restting timer for new packet transmission
						
		while(istime&&!isAck){		//if timer is still valid and valid acknowledgement has not been recieved
			ts--;
			if(ack_no==next_pkt+1){							//if timer is still valid and valid acknowledgement is recieved
				isAck=true;
				cout<<"Acknowledgement recieved! ack_no. "<<ack_no<<endl;
			}
			else if(ts==0||ack_no==-1){
				cout<<"Timer expired! ";
//				if(isPktLost==true)
////					cout<<"			\\\\pkt lost \n";
//				else if(isAckLost==true)
////					cout<<"			\\\\ack lost \n";
				istime=false;
			}
		}	
	}while(!isRec);
}

int Stop_N_Wait_withARQ::send_pkt(int pkt_no,int seq){
	cout<<"Waiting for Acknowledgement... "<<endl;
	sleep(2);										//use rand?##
	int ack=reciever(pkt_no,seq);
	if(ack==rand() % (size - 0 + 1)){				//case for when ack gets lost
		sleep(2);
		isAckLost=true;
//		ts=0;
		return -1;
	}
	else
		return(ack);
}
int Stop_N_Wait_withARQ::reciever(int pkt_no,int seq){
	int k=rand() % (size-0+1);						//randomising the packet that has to be lost
	if(k==seq)	{							//if required packet hasnt been recieved 
											//then reciever sits idle,waiting for next pkt
		isPktLost=true;
		return(seq);
	}		
	else if(ack_no==seq||ack_no==-1){		//data is recieved only when one pkt arrives or expecting pkt matches incoming pkt 
		if(rec_pkt[rcounter-1]!=pkt_no){	//duplicacy handling
			rec_pkt[rcounter]=pkt_no;
			rcounter++;
			if (rcounter==size)
				isRec=true;
		}
		cout<<"Sending Acknowledgement for frame seq_no. "<<seq<<" ... "<<endl;
		sleep(2);
		return (seq+1);							//ack1 is sent against pkt0 as in, now recieve wants pkt 1
	}
}
int main(){
	Stop_N_Wait_withARQ u1;
	return 0;	
}

