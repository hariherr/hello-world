#include<iostream>
#include<cmath>
#include<unistd.h>
#include<cstdlib>
#include<time.h>
using namespace std;
struct Frame{
	int seq_no;
	bool isRecieved,isAck;
	Frame():isRecieved(false),isAck(false){};
};
class DLL{
	int n;
	Frame* P;
	int win_send,win_rec;
	static int Rn;					// for reciever to recieve a frame only with seq number Rn
	static int outF;				// number of outstanding frames
	int* ack;
	int ack_i;
	public:				
					// exhibiting layer functionality at sender's side and not on reciever's side. only the reciever() handles all functionality at reciever's end
		void sender();
		void reciever(Frame*);
		void to_transportLayer(int,int);
		int from_transportLayer();
		void from_nwLayer();
		void to_phyLayer(Frame*);
		void to_nwLayer();
		void from_phyLayer();
};
int DLL::outF=0;
void DLL::to_transportLayer(int m,int num){
	cout<<"\n\n\t***TRANSPORT LAYER*** \n";
	
	win_send=pow(2,m)-1;							// window size= 2^m-1;
	win_rec=1;										//reciever window size always 1 for GBN
	
	n=num;
	cout<<"\nSegmenting inputted data  \n ";
	sleep(1);
	cout<<"Attaching Port numbers... ";
	sleep(1);
	cout<<"\nSending segments to Network Layer  \n ";
	to_nwLayer();
};
void DLL::to_nwLayer(){
	cout<<"\n\n\t***NETWORK LAYER*** \n";
	
	cout<<"\nPacketing recieved segments ...\n ";
	sleep(1);
	cout<<"Attaching IP addresses... ";
	P=new Frame[n];
	ack=new int[n];						// array to store ack recieved at sender's end
	ack_i=0;							//index to keep track of ack in ack array
	sender();
}
void DLL::from_nwLayer(){
	sleep(2);
	cout<<"\nSending packets to Data Link Layer... ";
}
void DLL::sender(){
//	bool isAckLost=false;
	from_nwLayer();
	cout<<"\n\n\t***DATA LINK LAYER*** \n";
	cout<<"\nRecieving packets from network layer... ";
	sleep(1);
	cout<<"\nFraming packets from network layer... ";
	sleep(1);
	cout<<"\nAttaching sequence numbers onto frames... ";
	for(int i=0;i<n;i++)
		P[i].seq_no=i%(win_send+1);
	cout<<"\nFrames constructed with sequence numbers:  ";
	for(int i=0;i<n;i++)
		cout<<P[i].seq_no<<" ";
	cout<<endl;
	//Sending to physical layer
	
	for(int i=0;i<n;){
		
		cout<<"\n\n\t\tSending window with frames { ";
		int j=i;
		int count_f=0;							//to count number of frames being sent at a time. should always be <= win_send
		while(count_f<win_send&&j<n){			// j<n for sending only no of frame<=win_send at the last window
			cout<<P[j].seq_no<<" ";
			j++;
			count_f++;
		}
		cout<<"} \n";
		//sending window
		int Sn;
		count_f=0;
		j=i;
		while(count_f<win_send&&j<n){
			outF++;							// frames sent but not acknowledged yet
			to_phyLayer(&P[j]);				// sending packet object to physical layer at sender's side
			sleep(1);
			j++;							// next frame to send
			count_f++;						//increments as frames are sent
			Sn=j;							// next frame to send
			
		}
		cout<<"\nWhole window sent ";
		cout<<"\n\n\tAcknowledgements recieved:  ";
		int Sf=i;							//Sf points to first outstanding frame
		for(int k=i;k<ack_i;k++){
			cout<<ack[k]<<" ";				//ack array stores acknowledgements for recieved frames
			if(ack[k]==P[i].seq_no+1){		// if corresponding ack exists in ack for a frame, then ack recived by sender and therefore, frame is no longer outstanding
				P[i].isAck=true;
				outF--;						//decrement outstanding as now frame acknowledged
				Sf++;						// sf now points to next outStanding frame
				i++;						//incremented to next frame in Packet array
			}	
		}
		if(outF!=0){
			i=Sf;
			cout<<"\nResending window as Ack or Frame suring transmission lost\n";
		}
		
		
	}
	cout<<"\n\t\tAcknowledgements recieved for all frames and no more outstanding frames exists, DATA TRANSMISSION SUCCESSFUL!!! ";	
}
int DLL::Rn=0;						// setting inital expected frame seq number for reciever
void DLL::to_phyLayer(Frame* f){														// to physical layer at sender's end
	cout<<"\nSending frame to the reciever... ";
	sleep(3);
	reciever(f);
}
void DLL::reciever(Frame* f){
	
	if(f->seq_no==Rn){
		cout<<"\nRecieving frame "<<f->seq_no;
		sleep(1);
		cout<<"\nframe "<<f->seq_no<<" well recieved ";
		cout<<"\nsending acknowlegment number "<<f->seq_no+1<<" for frame "<<f->seq_no;				// acknowledgements from reciever will always be sent for well recieved packets
																					// however due to noisy channel, they might get lost so, some won't be recied by sender,therefore,
																					// might not get stored in ack array
		f->isRecieved=true;																			
		ack[ack_i]=f->seq_no+1;
		ack_i++;
		Rn++;
		Rn=Rn%(win_send+1);
	}	
}
int main(){
	int n,wn;
	string data;
	cout<<"\n***APPLICATION LAYER*** \n";
	cout<<"\nEnter data to be sent to n/w Layer ";
	cin>>data;
	cout<<"\nEnter number of packets to send ";
	cin>>n;
	cout<<"\nEnter number window size bits ";
	cin>>wn;
	DLL L1;
	L1.to_transportLayer(wn,n);
	
}
