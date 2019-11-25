#include<iostream>
#include<string>
using namespace std;

class Crc
{
	private:
		string data;
		string key;

	public:
		Crc()
		{
			data="";
			key="";
		}
		string sender()
		{
			string o_data=data;
			data.append(key.length()-1, '0');
			cout << "Sending data = " << data << " with key = " << key << endl;

		  	for(int i=0;i<data.length()-key.length();++i)
		  	{
		  		clog << "# with i: " << i << endl;
		  		if(data[i] == '1')
		  		for(int j=0;j<key.length();++j)
		  		{
		  			cout<< " and j: " << j << ", data[i+j]=" << data[i+j]<<" and key[j]= "<<key[j];
		  	  		data[i+j]=(data[i+j]-48)^(key[j]-48)+48;
		  	  		cout<<" after XOR = " << data[i+j] << endl;
				}
		  	}
		  	o_data=o_data+data.substr(data.length()-key.length()+1);
		  	cout<<"remaider data is "<<o_data<<endl;
		  	return o_data;
		}
		void get_data()
		{
			cout<<"Enter the data";
			cin>>data;

			cout<<"Enter the key";
			cin>>key;
		}
		void reciever(string o_data)
		{
			for(int i=0;i<o_data.length()-key.length();++i)
		  	{
		  		clog << "# with i: " << i << endl;
		  		if(o_data[i] == '1')
		  		for(int j=0;j<key.length();++j)
		  		{
		  			cout<< " and j: " << j << ", data[i+j]=" << o_data[i+j]<<" and key[j]= "<<key[j];
		  	  		o_data[i+j]=(o_data[i+j]-48)^(key[j]-48)+48;
		  	  		cout<<" after XOR = " << o_data[i+j] << endl;
				}
		  	}
		  	cout<<"o_data is"<<o_data<<endl;
		  	for(int i=0;i<o_data.length();++i)
			  {
			  	if(o_data.find('1') != string::npos)
			        cout<<"false";
			  }
		}
		void show_data()
		{
			cout<<"data is\n" << data;

		}



};
int main()
{
	Crc A,B,C;
	A.get_data();
	string data=A.sender();
	A.reciever(data);
	A.show_data();
	return 0;
}
