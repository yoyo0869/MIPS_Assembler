/*****************************************************************************

TITLE:Assembler																																
AUTHOR:Yuvraj Verma
ROLL NUMBER:2201CS80

Declaration of Authorship

This cpp file, emu.cpp, is part of the assignment of CS209-210 at the 
department of Computer Science and Engg, IIT Patna . 

*****************************************************************************/

#include <bits/stdc++.h>
using namespace std;
int32_t ra=0,rb=0,pc=0,sp=0x100000; // The 4 registers. SP initialized to a random address far from 0.

struct instr{
    int opcode;
    int32_t operand=0;
};

string mnemonics[19]={"ldc","adc","ldl","stl","ldnl","stnl","add","sub","shl","shr","adj","a2sp","sp2a","call","return","brz","brlz","br","HALT"};

map<string,pair<int,int>> opcodes;

void build(){ // Table for mnemonics in the form {name, {opcode, num of operands}}
    opcodes["ldc"]={0,1};
    opcodes["adc"]={1,1};
    opcodes["ldl"]={2,1};
    opcodes["stl"]={3,1};
    opcodes["ldnl"]={4,1};
    opcodes["stnl"]={5,1};
    opcodes["add"]={6,0};
    opcodes["sub"]={7,0};
    opcodes["shl"]={8,0};
    opcodes["shr"]={9,0};
    opcodes["adj"]={10,1};
    opcodes["a2sp"]={11,0};
    opcodes["sp2a"]={12,0};
    opcodes["call"]={13,1};
    opcodes["return"]={14,0};
    opcodes["brz"]={15,1};
    opcodes["brlz"]={16,1};
    opcodes["br"]={17,1};
    opcodes["HALT"]={18,0};
    opcodes["data"]={0,1};
    opcodes["SET"]={-1,1};
}

string toHex(int number, int len) {
    string hexString;
    hexString.resize(len);
    for (int i = len - 1; i >= 0; --i) {
        int digit = number & 0xF;
        hexString[i] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        number >>= 4;
    }
    return hexString;
}

vector<instr>instructions; // For storing instructions read as binary in numeric {opcode , operand} format. Indexed by PC.

map<int32_t,int32_t>mem{}; // Implementing memory.

ofstream tracefile;

void printTrace(int32_t opd,int opc){ // Prints values of the 4 registers and the executed mnemonic and operand.
    tracefile<<"A: "<<toHex(ra,8)<<" B: "<<toHex(rb,8)<<" PC: "<<toHex(pc,8)<<" SP: "<<toHex(sp,8)<<" "<<mnemonics[opc];
    if(opc<=18&&opc>=0&&opcodes[mnemonics[opc]].second)tracefile<<" "<<opd;
    tracefile<<'\n';
}

map<int32_t,vector<int32_t>>memtable{}; // For printing upto 4 memory values in a single line. 

void printMemDump(){
    cout<<"Memory Dump:\n\n";
    memtable.clear();
    for(auto p:mem){
        auto addr=p.first;
        addr/=4; 
        addr*=4;
        if(memtable.find(addr)!=memtable.end())memtable[addr].push_back(p.second);
        else if(memtable.find(addr+1)!=memtable.end()) memtable[addr+1].push_back(p.second);
        else if(memtable.find(addr+2)!=memtable.end()) memtable[addr+2].push_back(p.second);
        else memtable[p.first].push_back(p.second);
    }
    for(auto p:memtable){
        cout<<toHex(p.first,8)<<" ";
        for(auto v:p.second)cout<<toHex(v,8)<<" ";
        cout<<'\n';
    }
}

map<int,int> pccoun;

int main(int argc,char* argv[]){
    streampos filesize;
    if(argc!=3){
        cout<<"Usage: ./emu [option] file.o\nWhere [option] is one of the following:\n\t-trace : Show trace of the executed instruction.\n\t-after : Show memory dump after program execution.\n\t-before : Show memory dump before program execution.\n";
        return 0;
    }
    build();
    string filename=string(argv[2]);
    ifstream input(filename);
    string temp="";
    char cur=filename[0];
    int coun=1;
    while(cur!='.'){
    	temp+=cur;
    	if(coun==(int)filename.size()) break;
    	cur=filename[coun];
    	coun++;
    }
    filename=temp;
    ifstream file(argv[2],ios::binary);
    string flag=argv[1];
    if(flag=="-trace"){
        tracefile.open(filename+".trace");
        cout<<"Writing trace file.\n";
    }
    
    file.seekg(0,std::ios::end);
    filesize=file.tellg();
    file.seekg(0,std::ios::beg);
    vector<unsigned char>fdata(filesize); 
    file.read(reinterpret_cast<char*>(&fdata[0]),filesize); // Reads the entire file into this vector.
    for(int i=0;i<((int)fdata.size());i+=4){ // Converting binary data to integer format. Reads 4 bytes from vector in each iteration.
        instr next={fdata[i],0};
        next.operand=fdata[i+1]|((int32_t)fdata[i+2]<<8)|((int32_t)fdata[i+3]<<16);
        if(next.operand&0x800000)next.operand-=0x1000000;
        mem[i/4]=(int32_t)next.opcode|(next.operand<<8); // Instructions stored in memory addresses starting from 0x0
        instructions.push_back(next);
    }
    int executions=0; // Number of instructions executed.
    if(flag=="-before")printMemDump();
    bool halted=false;
	// if(flag=="-trace"){
		// tracefile<<"A: "<<toHex(ra,8)<<" B: "<<toHex(rb,8)<<" PC: "<<toHex(pc,8)<<" SP: "<<toHex(sp,8)<<'\n';
	// }
	bool isINF=false;
    while(!halted){
        if(pc>=(int)instructions.size()){
            cout<<"Warning: Reached end of file without recieving HALT instruction. Program terminated.\n";
            break;
        }
        int tm=pc;
        int opc=instructions[pc].opcode;
        int32_t opd=instructions[pc].operand;
        // Specific behaviour for each opcode. Self explalatory.
        if(opc==0){
            rb=ra;
            ra=opd;
        }else if(opc==1){
            ra+=opd;
        }else if(opc==2){
            rb=ra;
            ra=mem[sp+opd];
        }else if(opc==3){
            mem[sp+opd]=ra;
            ra=rb;
        }else if(opc==4){
            ra=mem[ra+opd];
        }else if(opc==5){
            mem[ra+opd]=rb;
        }else if(opc==6){
            ra=rb+ra;
        }else if(opc==7){
            ra=rb-ra;
        }else if(opc==8){
            ra=rb<<ra;
        }else if(opc==9){
            ra=rb>>ra;
        }else if(opc==10){
            sp+=opd;
        }else if(opc==11){
            sp=ra;
            ra=rb;
        }else if(opc==12){
            rb=ra;
            ra=sp;
        }else if(opc==13){
            rb=ra;
            ra=pc;
            pc+=opd;
        }else if(opc==14){
            pc=ra;
            ra=rb;
        }else if(opc==15){
            if(ra==0)pc+=opd;
        }else if(opc==16){
            if(ra<0)pc+=opd;
        }else if(opc==17){
            pc+=opd;
        }else if(opc==18){
            halted=true;
        }else{
            cout<<"ERROR: Unknown opcode "<<opc<<" at PC: "<<pc;
            return 0;
        }
        if(pc+1==tm && (flag=="-trace" || flag=="-after" || flag=="-before")){
        	isINF=true;
        	if(flag=="-after" || flag=="before") break;
        	cout<<"~~~~INFINITE LOOP DETECTED!!~~~~"<<'\n';
        	tracefile<<"~~~~INFINITE LOOP DETECTED!!~~~~"<<'\n';
        	break;
        }
        if(flag=="-trace")printTrace(opd,opc);
        pc++;
        executions++;
    }
    if(flag=="-before" && isINF) cout<<"INFINITE LOOP WAS DETECTED -> MEMORY DUMP printed upto it."<<'\n';
    else if(flag=="-after"){
    	printMemDump();
    	// cout<<"INFINITE LOOP WAS DETECTED -> MEMORY DUMP printed upto it."<<'\n';
    }
    cout<<executions<<" instructions executed.\n";
    return 0;
}