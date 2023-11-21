#include <vector>
#include <ctime>
#include <fstream>

// #include <winsock2.h> before #include <windows.h>
#include "SocketComm/SocketComm.h"
#include "SerialComm/SerialComm.h"

using namespace std;

float Byte2Float(unsigned char* p){
    float ans=0;
	unsigned long long temp=0;
	temp=(*p<<0)+(*(p+1)<<8)+(*(p+2)<<16)+(*(p+3)<<24);
	ans=*(float*)&temp;
	return ans;
}

int main()
{
    vector<vector<double>> res(5000,vector<double>(9,0));
    int row=0,col=0;

    SocketComm ch("127.0.0.1",8712);
    SerialComm sh("COM7");

    bool chValid=ch.open();

    if(sh.write())
        cout<<"mark"<<endl;

    clock_t st=clock();
    clock_t ed=clock();
    while((double)(ed-st)/CLOCKS_PER_SEC<=4){
        ed=clock();
    }

    if(sh.write())
        cout<<"mark"<<endl;
    
	while(chValid && row<5000){
		char recData[4];
		int ret=recv(ch.getClientHandle(),recData,4,0);
        if(ret){
            unsigned char* p=reinterpret_cast<unsigned char*>(recData);
            res[row][col]=Byte2Float(p);
            ++col;
            if(col==9){
                col=0;
                ++row;
            }
        }
	}

    ofstream fWriter("file.txt");
    for(int i=0;i<5000;++i){
        if(res[i][8]==255)
            cout<<"OK "<<i<<endl;
        for(int j=0;j<9;++j)
            fWriter<<res[i][j]<<" ";
        fWriter<<"\n";
    }
    fWriter.flush();
    fWriter.close();

	return 0;
}
