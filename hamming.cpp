#include<iostream>
#include<string>
#include<cmath>
#include<cstdlib>
using namespace std;

class hamming
{
	private:
		 string msg;
		 int sizeofmsg;
		 int r=1;
	public:
		 hamming(string msg);
	     bool receiver(int *arr);
	     int* sender();	 
	     int create_error(int *arr);
};
	hamming::hamming(string msg)
             {
   	            this->msg=msg;//msg will be in binary
                 this->sizeofmsg=msg.length();
                  while(true)
        	           {
        	 	         if(pow(2,r)>=(r+sizeofmsg+1))
        	 	  	        break;
        	            	r++; 	
			             }
			 }
    int * hamming::sender()
                  {   
        	          
                      int *arr = new int[sizeofmsg+r];
		              for(int i=0,j=0,k=0;i<(sizeofmsg+r);i++)
		                  {   
			                  if(!(i+1==(int)pow(2,j)))
		                        {  char ch=msg[k++];
					               arr[i] = (ch-'0');
	
		          			      }
				                 else
				                    j++;
		     	
			                 }
			
			 
		             for(int i=0;i<r;i++)
		                {   
			               int parity=5;
		                  for(int j=(int)pow(2,i)-1;j<(sizeofmsg+r);j=j+(int)pow(2,i))
                              {    int p=j;
                   	              for(int k=0;k<(int)pow(2,i);k++)
                   	                 {
						   
                   	               if(!(j==(int)pow(2,i)-1)&&j<(sizeofmsg+r))
                   	                 {  if(parity==5)
                   	                      parity=arr[j];
                   	                   else
						                 parity=parity^arr[j];  
							          }
							           j++;
						                }
									   		        
					             }
					
					   
					    arr[(int)pow(2,i)-1]=parity;
				  	 	  
                		}
			return arr;
	}
	int hamming::create_error(int *arr)
	{
		cout<<"do you want to generate error then press 1\n";
		  int choic;int indx = -1;
		  cin>>choic;
		  if(choic==1)
		    {
		    	indx=rand()%10;
		    	cout<<"index is"<<indx;
		    	if(arr[indx]!=0)
		    	   arr[indx]=0;
		    	 else
				    arr[indx]=1;  
			}
			return indx;
	     
	}
 bool hamming::receiver(int *arr)
               {          
			  
	            for(int i=0;i<r;i++)
		            {   
			           int parity=5;
		               for(int j=(int)pow(2,i)-1;j<(sizeofmsg+r);j=j+(int)pow(2,i))
                          {    
                   	         for(int k=0;k<(int)pow(2,i);k++)
                   	            {
						   
                   	               if(j<(sizeofmsg+r))
                   	                {
								
                   	                  if(parity==5)
                   	                    parity=arr[j];
                   	                  else
						                 parity=parity^arr[j];  
						             }
							         j++;
						            }
									   		        
					           }
		                    
					         if(parity!=0)
                               return false;				   	 	  
		                }
		    return true;
		
	    }   
int main()
{ 
    cout<<"enetr the message in binary form\n";
     string msg;
     cin>>msg;
     hamming g(msg);
     cout<<"message is sending\n";
     int *p= g.sender();



  int errbit=g.create_error(p);
 if(g.receiver(p))
 {
      cout<<"the message is corrsct\n";
  }
   else
   { 
       cout<<"the message is incorrect";
	    cout<<"changed bit is"<<errbit<<endl; 
	}
	return 0;
		}		
