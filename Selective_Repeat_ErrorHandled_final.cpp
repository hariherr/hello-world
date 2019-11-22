#include<iostream>
#include<cmath>
#include<unistd.h>
#include<cstdlib>
#include<time.h>
using namespace std;
						//Selective repeat. All cases handled.Error handled and verified through error introduction.
struct Frame{
	int seq_no;
	bool isRecieved,isAck;
	Frame():isRecieved(false),isAck(false){};
};
class DLL{
	int n;
	int m;
	int s;
//	int timer;
	Frame* F;
	int win_send,win_rec;
	static int Rn;					// Rn is first frame in reciever's sliding window
	static int Sn;					// next frame to be sent by sender
	static int outF;				// number of outstanding frames
	static bool isResent;
	int NAKforF;
	int* ack;
//	int* rec;
	
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
int DLL::Sn=0;
bool DLL::isResent=false;
void DLL::to_transportLayer(int mb,int num){
	cout<<"\n\n\t***TRANSPORT LAYER*** \n";
	m=mb;
	win_send=pow(2,m-1);							// window size= 2^(m-1);
	win_rec=win_send;								//reciever window size = sender's window size in SELECTIVE REPEAT
	s=pow(2,m);
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
	F=new Frame[n];
	ack=new int[n];						// array to store ack recieved at sender's end
	for(int i=0;i<n;i++)
		ack[i]='\0';					//initialising ack array so that we can use this default value to later send NAKS in case of out of order frames
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
		F[i].seq_no=i%(s);
//	timer=win_send;								//setting timer to max window size
	cout<<"\nFrames constructed with sequence numbers:  ";
	for(int i=0;i<n;i++)
		cout<<F[i].seq_no<<" ";
	cout<<endl;
	//Sending to physical layer
	
	for(int i=0;i<n;){
		if(!isResent){
			cout<<"\n\n\t\tSending window with frames { ";
			int j=i;
			int count_f=0;							//to count number of frames being sent at a time. should always be <= win_send
			while(count_f<win_send&&j<n){			// j<n for sending only no. of frame<=win_send at the last window
				cout<<F[j].seq_no<<" ";
				j++;
				count_f++;
			}
			cout<<"} \n";
			//sending window
			count_f=0;
			j=i;
			while(count_f<win_send&&j<n){
				Sn=j+1;							// next frame to send, saving it in Sn. j not set at next frame just yet
				to_phyLayer(&F[j]);				// sending packet object to physical layer at sender's side
				sleep(1);
				j++;							// next frame to send
				count_f++;						//increments as frames are sent
			}
			cout<<"\nWhole window sent ";
		}
	//	cout<<"\n\n\tAcknowledgements recieved:  ";
//		int Sf=i;							//Sf points to first outstanding frame
		int l='\0';
		for(int k=i;k<ack_i;k++){
	//		cout<<ack[k]<<" ";				//ack array stores acknowledgements for recieved frames
			if(ack[k]==F[i].seq_no+1){		// if corresponding ack exists in ack for a frame, then ack recived by sender and therefore, frame is no longer outstanding
			
				F[i].isAck=true;			//we check if acknowledgements are recieved at reciever end 
				
//				Sf++;						// sf now points to next outStanding frame
				i++;						//incremented to next frame in Packet array
			}	
			if(ack[k]<0||ack[k]=='\0'){
				l=i;
				i++;
				continue;
			}
		}
		if(l!='\0'){
			cout<<"\nAs negative acknowledgement recieved for frame "<<F[l].seq_no<<", resending frame ";
			isResent=true;
			to_phyLayer(&F[l]);
			i=Sn;
		}
//		cout<<"\n# i="<<i<<" 	Sn="<<Sn<<" 	Sf="<<Sf<<endl;

		
		
	}
	cout<<"\n\t\tAcknowledgements recieved for all frames and no more outstanding frames exists, DATA TRANSMISSION SUCCESSFUL!!! ";	
}
int DLL::Rn=0;						// setting inital expected frame seq number for reciever
void DLL::to_phyLayer(Frame* f){														// to physical layer at sender's end
	cout<<"\nSending frame "<<f->seq_no<<" to the reciever... ";
	sleep(3);
	if(f->seq_no!=rand() % (s))
		reciever(f);
}
void DLL::reciever(Frame* f){

//	cout<<"# Rn="<<Rn<<" ack_ii="<<ack_i<<endl;
	if(isResent){
		cout<<"\nrecieving resent frame "<<f->seq_no<<endl; 						// for recieving resent frames
		ack[NAKforF]=f->seq_no+1;
		f->isRecieved=true;
		f->isAck=true;					// the sender replied to the negative acknowlegement, therefore, frame acknoeledgeed
		NAKforF='\0';
		int k=0;
		while(ack[k]!='\0'&&k!=ack_i){
												// loop will iterate till \0 is not found, i.e in case of when a frame is resent and well recieved,
												// its following frames may already have been recieved. therefore, this is done to skip over those recieved frames and get to the 
												// new frame that can be expected	
			k++;
		}
		Rn=F[k].seq_no;								//Rn now stores the seq no of frame next expected.i.e same as Sn
		isResent=false;
		cout<<"\nAll frames in current window recieved.Sending to n/w Layer...\n ";
	}
	else if(Rn<=f->seq_no||F[Sn].seq_no>f->seq_no){								// if the incoming frame is part of the reciever window, then accept it
		cout<<"\nRecieving frame "<<f->seq_no<<"...";
		sleep(1);
		cout<<"\nframe "<<f->seq_no<<" well recieved ";
		if(f->seq_no==Rn){
			cout<<"\nsending acknowlegment number "<<f->seq_no+1<<" for frame "<<f->seq_no;				// acknowledgements from reciever will always be sent for well recieved packets
																					// however due to noisy channel, they might get lost so, some won't be recied by sender,therefore,
																					// might not get stored in ack array
			f->isRecieved=true;																			
			ack[ack_i]=f->seq_no+1;
			ack_i++;
			Rn++;
			Rn=Rn%(s);
		}
		else{						// for reciving out of order frames
			
			ack_i++;									//skipping position in ack array for frame not recieved
			for(int i=0;i<ack_i;i++){
				if(ack[i]<0||ack[i]=='\0'){				//condition ordering important cause, there might be cases where the NAK for a certain frame as already been sent and
														// and its ack is modified to a negative value in prior iterations. If the NAK frame not recieved before another frame comes in,
														// then the NAK fram no longer has \0 ack. it has -ve ack. therefore, be careful in conditions
					NAKforF=i;
					break;
				}
			}
			cout<<"\nFrame "<<f->seq_no<<" recieved out of order as expected frame "<<F[NAKforF].seq_no<<endl;
			cout<<"\nsending NAK "<<F[NAKforF].seq_no<<" for frame "<<F[NAKforF].seq_no<<endl;
			f->isRecieved=true;
			if(F[NAKforF].seq_no!=0)
				ack[NAKforF]=(-1*F[NAKforF].seq_no);	
			else
				ack[NAKforF]='\0';									// negative seq number to represent a negative ack
			ack[ack_i]=f->seq_no+1;
			ack_i++;
		}
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
