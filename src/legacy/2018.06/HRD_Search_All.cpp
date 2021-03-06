#include<iostream>
#include<fstream>
#include<vector>
#include<list>
using namespace std;
const unsigned char Output_Width=8;
const string Square_str="\u2588\u2588";
struct Block_Data{
    unsigned char Address;
    unsigned char Style;
};
vector<unsigned int> Data_All;
vector<unsigned int> Source;
vector<unsigned int> Solve_Data;
list<unsigned int> Hash[0xFFFF];
ofstream File_Output;
ifstream File_Input;
struct Block_Data Block[12];
unsigned char Table[21];
unsigned char Space_1,Space_2,Space_num_1,Space_num_2,Last_Move;
unsigned int Search_num,Solve_num;
bool Done,End=false;
void Solve(unsigned int Data);
void Find_Next();
void Move_Block(unsigned char num,unsigned char Direction_1,unsigned char Direction_2);
void Add_List(unsigned int Data);
void Output_str(unsigned int Data);
unsigned int Input(char Data_str[8]);
bool Check_Error(unsigned int Start_Data);
void Calculate(unsigned int Start_Data);
void Output();
void OutputData(unsigned int Data){
    unsigned char i;
    for(i=0;i<8;i++){
        if((0x0F&Data>>(7-i)*4)==0x0){File_Output<<"0";}
        if((0x0F&Data>>(7-i)*4)==0x1){File_Output<<"1";}
        if((0x0F&Data>>(7-i)*4)==0x2){File_Output<<"2";}
        if((0x0F&Data>>(7-i)*4)==0x3){File_Output<<"3";}
        if((0x0F&Data>>(7-i)*4)==0x4){File_Output<<"4";}
        if((0x0F&Data>>(7-i)*4)==0x5){File_Output<<"5";}
        if((0x0F&Data>>(7-i)*4)==0x6){File_Output<<"6";}
        if((0x0F&Data>>(7-i)*4)==0x7){File_Output<<"7";}
        if((0x0F&Data>>(7-i)*4)==0x8){File_Output<<"8";}
        if((0x0F&Data>>(7-i)*4)==0x9){File_Output<<"9";}
        if((0x0F&Data>>(7-i)*4)==0xA){File_Output<<"A";}
        if((0x0F&Data>>(7-i)*4)==0xB){File_Output<<"B";}
        if((0x0F&Data>>(7-i)*4)==0xC){File_Output<<"C";}
        if((0x0F&Data>>(7-i)*4)==0xD){File_Output<<"D";}
        if((0x0F&Data>>(7-i)*4)==0xE){File_Output<<"E";}
        if((0x0F&Data>>(7-i)*4)==0xF){File_Output<<"F";}
    }
}

int main(){
    char Name[8];
    unsigned int Start_Data,i,k;
    cout<<"Welcome to the HRD!  (by Dnomd343)"<<endl<<endl;

File_Output.open("All HRD.txt");
cout<<hex;
    for(i=0;i<=0x3ffffc;i++){
        for(k=1;k<=12;k++){
            Start_Data=0;
            Start_Data=(Start_Data|(i<<10))|k;
            if(Check_Error(Start_Data)==true){OutputData(Start_Data);File_Output<<endl;}//Output_str(Start_Data);cout<<endl;}
        }
    }
return 0;
    i=0;
    while(1){
        cout<<"HRD style:";
        cin>>Name;
        Start_Data=Input(Name);
	if(End==true){return 0;}
        Calculate(Start_Data);
        cout<<"Press ENTER to show the Best-Solve...";
        cin.get();
        for(i=0;i<Solve_Data.size();i++){
       	    cin.get();
            for(unsigned int k=0;k<=28;k++){cout<<endl;}
            cout<<"Step "<<i<<" -> ";
            Output_str(Solve_Data[i]);
            cout<<endl;
            Solve(Solve_Data[i]);
            Output();
        }
        cout<<"Show has been compete!"<<endl<<endl<<endl;
        cin.clear();
    }
    return 0;
}
void Calculate(unsigned int Start_Data){
    unsigned int i;
    Data_All.clear();
    Source.clear();
    Solve_Data.clear();
    for(i=0;i<0xFFFF;i++){Hash[i].clear();}
    if(Check_Error(Start_Data)==false){cout<<"Something Error!!!"<<endl;goto err;}
    Data_All.push_back(Start_Data|0xF0);
    Source.push_back(0);
    Solve(Data_All[0]);
    Output();
    cout<<"Start to calculate : ";
    Output_str(Data_All[0]&(~0xF0));
    cout<<endl<<"Waiting..."<<endl;
    i=0;
    Done=false;
    while(1){
        if(i==Data_All.size()){cout<<"No Solve!"<<endl;goto err;}
        Search_num=i;
        Solve(Data_All[i]);
        Find_Next();
        i++;
        if(Done==true){break;}
    }
    while(1){
        Solve_Data.push_back(Solve_num);
        Solve_num=Source[Solve_num];
        if(Solve_num==0){break;}
    }
    Solve_Data.push_back(0);
    for(i=0;i<Solve_Data.size()/2;i++){swap(Solve_Data[i],Solve_Data[Solve_Data.size()-i-1]);}
    for(i=0;i<Solve_Data.size();i++){Solve_Data[i]=Data_All[Solve_Data[i]];}
    cout<<"Calculate Done!"<<endl;
    err:;
    cout<<"Step:"<<Solve_Data.size()-1<<"  Pool:"<<Data_All.size()<<endl;
}
void Add_List(unsigned int Data){
    list<unsigned int>::iterator poi;
    poi=Hash[Data>>16].begin();
    while(poi!=Hash[Data>>16].end()){
        if((Data&0xFFFFFC0F)==(Data_All[*poi]&0xFFFFFC0F)){goto out;}
        ++poi;
    }
    Hash[Data>>16].push_back(Data_All.size());
    Data_All.push_back(Data);
    Source.push_back(Search_num);
    out:;
}
bool Check_Error(unsigned int Start_Data){
    unsigned char addr,i;
    bool Check[21];
    unsigned char Style_0,Style_1,Style_2,Style_3,Style_4;
    Style_0=Style_1=Style_2=Style_3=Style_4=0;
    for(i=1;i<=20;i++){Check[i]=false;}
    for(i=0;i<=11;i++){
        Block[i].Address=0;
        Block[i].Style=5;
    }
    Solve(Start_Data);
    for(i=0;i<=11;i++){
        addr=Block[i].Address;
        if(Block[i].Style==0){
            Style_0++;
            if(addr<1||addr>20){return false;}
            if(Check[addr]==true){return false;}
            Check[addr]=true;
        }
        if(Block[i].Style==1){
            Style_1++;
            if(addr<1||addr>19){return false;}
            if((addr%4)==0){return false;}
            if(Check[addr]==true||Check[addr+1]==true){return false;}
            Check[addr]=true;
            Check[addr+1]=true;
        }
        if(Block[i].Style==2){
            Style_2++;
            if(addr<1||addr>16){return false;}
            if(Check[addr]==true||Check[addr+4]==true){return false;}
            Check[addr]=true;
            Check[addr+4]=true;
        }
        if(Block[i].Style==3){
            Style_3++;
            if(addr<1||addr>20){return false;}
            if(Check[addr]==true){return false;}
            Check[addr]=true;
        }
        if(Block[i].Style==4){
            Style_4++;
            if(addr<1||addr>15){return false;}
            if((addr%4)==0){return false;}
            if(Check[addr]==true||Check[addr+1]==true||Check[addr+4]==true||Check[addr+5]==true){return false;}
            Check[addr]=true;
            Check[addr+1]=true;
            Check[addr+4]=true;
            Check[addr+5]=true;
        }
    }
    if(Style_0!=2){return false;}
    if((Style_1+Style_2)!=5){return false;}
    if(Style_3!=4){return false;}
    if(Style_4!=1){return false;}
    return true;
}
void Move_Block(unsigned char Move_num,unsigned char Direction_1,unsigned char Direction_2){
    Table[Block[Move_num].Address]=12;
    if(Block[Move_num].Style==1){Table[Block[Move_num].Address+1]=12;}
    if(Block[Move_num].Style==2){Table[Block[Move_num].Address+4]=12;}
    if(Block[Move_num].Style==4){Table[Block[Move_num].Address+1]=12;Table[Block[Move_num].Address+4]=12;Table[Block[Move_num].Address+5]=12;}
    Table[Space_1]=12;
    Table[Space_2]=12;
    if(Direction_1==1){Block[Move_num].Address-=4;}
    else if(Direction_1==2){Block[Move_num].Address+=4;}
    else if(Direction_1==3){Block[Move_num].Address--;}
    else if(Direction_1==4){Block[Move_num].Address++;}
    if(Direction_2==1){Block[Move_num].Address-=4;}
    else if(Direction_2==2){Block[Move_num].Address+=4;}
    else if(Direction_2==3){Block[Move_num].Address--;}
    else if(Direction_2==4){Block[Move_num].Address++;}
    Table[Block[Move_num].Address]=Move_num;
    if(Block[Move_num].Style==1){Table[Block[Move_num].Address+1]=Move_num;}
    if(Block[Move_num].Style==2){Table[Block[Move_num].Address+4]=Move_num;}
    if(Block[Move_num].Style==4){Table[Block[Move_num].Address+1]=Move_num;Table[Block[Move_num].Address+4]=Move_num;Table[Block[Move_num].Address+5]=Move_num;}
    unsigned int Data=0;
    unsigned char addr=1,num=1;
    bool jump=false,Use[13];
    for(unsigned char i=0;i<=12;i++){Use[i]=false;}
    while(1){
        while(Use[Table[addr]]==true){addr++;}
        if(Table[addr]==12){
            Data=Data|(0<<((16-num)*2));
            num++;
            addr++;
            jump=true;
        }
        if(jump==false){
            if(Block[Table[addr]].Style==4){
                Data=Data|num;
                Use[0]=true;
            }
            else{
                if(Table[addr]==Move_num){Data=Data|(num<<4);}
                Data=Data|(Block[Table[addr]].Style<<((16-num)*2));
                Use[Table[addr]]=true;
                num++;
            }
        }
        else{jump=false;}
        if(num==12){break;}
    }
    Add_List(Data);
    if(Block[0].Address==14){Done=true;Solve_num=Data_All.size()-1;}
    if(Direction_2==1){Block[Move_num].Address+=4;}
    else if(Direction_2==2){Block[Move_num].Address-=4;}
    else if(Direction_2==3){Block[Move_num].Address++;}
    else if(Direction_2==4){Block[Move_num].Address--;}
    if(Direction_1==1){Block[Move_num].Address+=4;}
    else if(Direction_1==2){Block[Move_num].Address-=4;}
    else if(Direction_1==3){Block[Move_num].Address++;}
    else if(Direction_1==4){Block[Move_num].Address--;}
    Table[Block[Move_num].Address]=Move_num;
    if(Block[Move_num].Style==1){Table[Block[Move_num].Address+1]=Move_num;}
    if(Block[Move_num].Style==2){Table[Block[Move_num].Address+4]=Move_num;}
    if(Block[Move_num].Style==4){Table[Block[Move_num].Address+1]=Move_num;Table[Block[Move_num].Address+4]=Move_num;Table[Block[Move_num].Address+5]=Move_num;}
    Table[Space_1]=Space_num_1;
    Table[Space_2]=Space_num_2;
}
void Find_Next(){
    unsigned char i;
    for(i=0;i<=0x0B;i++){
        if(i!=Last_Move){
            if(Block[i].Style==4){
                if((Block[i].Address>=5)&&(Block[Table[Block[i].Address-4]].Style==0)&&(Block[Table[Block[i].Address-3]].Style==0)){Move_Block(i,1,0);}
                if((Block[i].Address<=12)&&(Block[Table[Block[i].Address+8]].Style==0)&&(Block[Table[Block[i].Address+9]].Style==0)){Move_Block(i,2,0);}
                if(((Block[i].Address%4)!=1)&&(Block[Table[Block[i].Address-1]].Style==0)&&(Block[Table[Block[i].Address+3]].Style==0)){Move_Block(i,3,0);}
                if(((Block[i].Address%4)!=3)&&(Block[Table[Block[i].Address+2]].Style==0)&&(Block[Table[Block[i].Address+6]].Style==0)){Move_Block(i,4,0);}
            }
            else if(Block[i].Style==1){
                if((Block[i].Address>=5)&&(Block[Table[Block[i].Address-4]].Style==0)&&(Block[Table[Block[i].Address-3]].Style==0)){Move_Block(i,1,0);}
                if((Block[i].Address<=16)&&(Block[Table[Block[i].Address+4]].Style==0)&&(Block[Table[Block[i].Address+5]].Style==0)){Move_Block(i,2,0);}
                if(((Block[i].Address%4)!=1)&&(Block[Table[Block[i].Address-1]].Style==0)){
                    Move_Block(i,3,0);
                    if(((Block[i].Address%4)!=2)&&(Block[Table[Block[i].Address-2]].Style==0)){Move_Block(i,3,3);}
                }
                if(((Block[i].Address%4)!=3)&&(Block[Table[Block[i].Address+2]].Style==0)){
                    Move_Block(i,4,0);
                    if(((Block[i].Address%4)!=2)&&(Block[Table[Block[i].Address+3]].Style==0)){Move_Block(i,4,4);}
                }
            }
            else if(Block[i].Style==2){
                if(((Block[i].Address%4)!=1)&&(Block[Table[Block[i].Address-1]].Style==0)&&(Block[Table[Block[i].Address+3]].Style==0)){Move_Block(i,3,0);}
                if(((Block[i].Address%4)!=0)&&(Block[Table[Block[i].Address+1]].Style==0)&&(Block[Table[Block[i].Address+5]].Style==0)){Move_Block(i,4,0);}
                if((Block[i].Address>=5)&&(Block[Table[Block[i].Address-4]].Style==0)){
                    Move_Block(i,1,0);
                    if((Block[i].Address>=9)&&(Block[Table[Block[i].Address-8]].Style==0)){Move_Block(i,1,1);}
                }
                if((Block[i].Address<=12)&&(Block[Table[Block[i].Address+8]].Style==0)){
                    Move_Block(i,2,0);
                    if((Block[i].Address<=8)&&(Block[Table[Block[i].Address+12]].Style==0)){Move_Block(i,2,2);}
                }
            }
            else if(Block[i].Style==3){
                if((Block[i].Address>=5)&&(Block[Table[Block[i].Address-4]].Style==0)){
                    Move_Block(i,1,0);
                    if((Block[i].Address>=9)&&(Block[Table[Block[i].Address-8]].Style==0)){Move_Block(i,1,1);}
                    if(((Block[i].Address%4)!=1)&&(Block[Table[Block[i].Address-5]].Style==0)){Move_Block(i,1,3);}
                    if(((Block[i].Address%4)!=0)&&(Block[Table[Block[i].Address-3]].Style==0)){Move_Block(i,1,4);}
                }
                if((Block[i].Address<=16)&&(Block[Table[Block[i].Address+4]].Style==0)){
                    Move_Block(i,2,0);
                    if((Block[i].Address<=12)&&(Block[Table[Block[i].Address+8]].Style==0)){Move_Block(i,2,2);}
                    if(((Block[i].Address%4)!=1)&&(Block[Table[Block[i].Address+3]].Style==0)){Move_Block(i,2,3);}
                    if(((Block[i].Address%4)!=0)&&(Block[Table[Block[i].Address+5]].Style==0)){Move_Block(i,2,4);}
                }
                if(((Block[i].Address%4)!=1)&&(Block[Table[Block[i].Address-1]].Style==0)){
                    Move_Block(i,3,0);
                    if(((Block[i].Address%4)!=2)&&(Block[Table[Block[i].Address-2]].Style==0)){Move_Block(i,3,3);}
                    if((Block[i].Address>=5)&&(Block[Table[Block[i].Address-5]].Style==0)){Move_Block(i,3,1);}
                    if((Block[i].Address<=16)&&(Block[Table[Block[i].Address+3]].Style==0)){Move_Block(i,3,2);}
                }
                if(((Block[i].Address%4)!=0)&&(Block[Table[Block[i].Address+1]].Style==0)){
                    Move_Block(i,4,0);
                    if(((Block[i].Address%4)!=3)&&(Block[Table[Block[i].Address+2]].Style==0)){Move_Block(i,4,4);}
                    if((Block[i].Address>=5)&&(Block[Table[Block[i].Address-3]].Style==0)){Move_Block(i,4,1);}
                    if((Block[i].Address<=16)&&(Block[Table[Block[i].Address+5]].Style==0)){Move_Block(i,4,2);}
                }
            }
        }
    }
}
void Solve(unsigned int Data){
    unsigned char i,k=1;
    unsigned char dat,addr=Data&0x0F;
    Space_1=Space_2=Space_num_1=Space_num_2=0;
    Last_Move=(Data>>4)&0x0F;
    for(i=1;i<=20;i++){Table[i]=0xFF;}
    for(i=1;i<=12;i++){
        while(Table[k]!=0xFF){k++;if(k>20){break;}}
        if(i==addr){
            Block[0].Address=k;
            Block[0].Style=4;
            Table[k]=Table[k+1]=Table[k+4]=Table[k+5]=0;
            while(Table[k]!=0xFF){k++;if(k>20){break;}}
        }
        if(i==12){break;}
        dat=(Data>>(32-i*2))&0x03;
        Block[i].Address=k;
        Table[k]=i;
        if(dat==0){Block[i].Style=0;if(Space_1==0){Space_1=k;Space_num_1=i;}else{Space_2=k;Space_num_2=i;}}
        else if(dat==1){Block[i].Style=1;Table[k+1]=i;}
        else if(dat==2){Block[i].Style=2;Table[k+4]=i;}
        else if(dat==3){Block[i].Style=3;}
    }
}
void Output(){
    unsigned int i,k,m;
    unsigned int Start_x,Start_y,End_x,End_y;
    bool Graph[(Output_Width+1)*4+1][(Output_Width+1)*5+1];
    for(k=0;k<=(Output_Width+1)*5;k++){
        for(i=0;i<=(Output_Width+1)*4;i++){
            Graph[i][k]=false;
        }
    }
    for(m=0;m<=0x0B;m++){
        if(Block[m].Style!=0){
            Start_x=((Block[m].Address-1)%4)*(Output_Width+1)+1;
            Start_y=(Block[m].Address-1)/4;
            Start_y=Start_y*(Output_Width+1)+1;
            if((Block[m].Style==1)||(Block[m].Style==4)){
                End_x=Start_x+Output_Width*2;
            }
            else{
                End_x=Start_x+Output_Width-1;
            }
            if((Block[m].Style==2)||(Block[m].Style==4)){
                End_y=Start_y+Output_Width*2;
            }
            else{
                End_y=Start_y+Output_Width-1;
            }
            for(i=Start_x;i<=End_x;i++){
                for(k=Start_y;k<=End_y;k++){
                    Graph[i][k]=true;
                }
            }
        }
    }
    for(i=1;i<=(Output_Width+1)*4+3;i++){cout<<Square_str;}
    cout<<endl<<Square_str;
    for(k=0;k<=(Output_Width+1)*5;k++){
        for(i=0;i<=(Output_Width+1)*4;i++){
            if(Graph[i][k]==false){cout<<"  ";}else{cout<<Square_str;}
        }
        cout<<Square_str<<endl<<Square_str;
    }
    for(i=1;i<=(Output_Width+1)*4+2;i++){cout<<Square_str;}
    cout<<endl;
}
void Output_str(unsigned int Data){
    unsigned char i;
    for(i=0;i<8;i++){
        if((0x0F&Data>>(7-i)*4)==0x0){cout<<"0";}
        if((0x0F&Data>>(7-i)*4)==0x1){cout<<"1";}
        if((0x0F&Data>>(7-i)*4)==0x2){cout<<"2";}
        if((0x0F&Data>>(7-i)*4)==0x3){cout<<"3";}
        if((0x0F&Data>>(7-i)*4)==0x4){cout<<"4";}
        if((0x0F&Data>>(7-i)*4)==0x5){cout<<"5";}
        if((0x0F&Data>>(7-i)*4)==0x6){cout<<"6";}
        if((0x0F&Data>>(7-i)*4)==0x7){cout<<"7";}
        if((0x0F&Data>>(7-i)*4)==0x8){cout<<"8";}
        if((0x0F&Data>>(7-i)*4)==0x9){cout<<"9";}
        if((0x0F&Data>>(7-i)*4)==0xA){cout<<"A";}
        if((0x0F&Data>>(7-i)*4)==0xB){cout<<"B";}
        if((0x0F&Data>>(7-i)*4)==0xC){cout<<"C";}
        if((0x0F&Data>>(7-i)*4)==0xD){cout<<"D";}
        if((0x0F&Data>>(7-i)*4)==0xE){cout<<"E";}
        if((0x0F&Data>>(7-i)*4)==0xF){cout<<"F";}
    }
}
unsigned int Input(char Data_str[8]){
    unsigned int i,Dat=0;
    if((Data_str[0]=='e')&&(Data_str[1]=='x')&&(Data_str[2]=='i')&&(Data_str[3]=='t')){End=true;}
    for(i=0;i<8;i++){
        if(Data_str[i]=='0'){Dat=Dat|(0x0<<(28-i*4));}
        if(Data_str[i]=='1'){Dat=Dat|(0x1<<(28-i*4));}
        if(Data_str[i]=='2'){Dat=Dat|(0x2<<(28-i*4));}
        if(Data_str[i]=='3'){Dat=Dat|(0x3<<(28-i*4));}
        if(Data_str[i]=='4'){Dat=Dat|(0x4<<(28-i*4));}
        if(Data_str[i]=='5'){Dat=Dat|(0x5<<(28-i*4));}
        if(Data_str[i]=='6'){Dat=Dat|(0x6<<(28-i*4));}
        if(Data_str[i]=='7'){Dat=Dat|(0x7<<(28-i*4));}
        if(Data_str[i]=='8'){Dat=Dat|(0x8<<(28-i*4));}
        if(Data_str[i]=='9'){Dat=Dat|(0x9<<(28-i*4));}
        if(Data_str[i]=='A'||Data_str[i]=='a'){Dat=Dat|(0xA<<(28-i*4));}
        if(Data_str[i]=='B'||Data_str[i]=='b'){Dat=Dat|(0xB<<(28-i*4));}
        if(Data_str[i]=='C'||Data_str[i]=='c'){Dat=Dat|(0xC<<(28-i*4));}
        if(Data_str[i]=='D'||Data_str[i]=='d'){Dat=Dat|(0xD<<(28-i*4));}
        if(Data_str[i]=='E'||Data_str[i]=='e'){Dat=Dat|(0xE<<(28-i*4));}
        if(Data_str[i]=='F'||Data_str[i]=='f'){Dat=Dat|(0xF<<(28-i*4));}
    }
    return Dat;
}

