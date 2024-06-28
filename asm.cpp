/*****************************************************************************
TITLE: Assembler																																
AUTHOR: Yuvraj Verma 
ROLL NUMBER: 2201CS80
Declaration of Authorship
This cpp file, asm.cpp, is part of the assignment of CS210 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/


#include<iostream>
#include<vector>
#include<map>
#include<string>
#include <bits/stdc++.h>
using namespace std;

// opcode and the value of operand
struct format{
    int opc;
    int oprd;
};

//list of errors stored in vector 
vector<pair<int,string>> error;
void err(int l, string s){
    error.push_back({l,s});
}

//function to check if the label is in valid format
bool crlabel(string l){
    int n=l.size();
    int f=l[0];
    if (!((f>=65 && f<=90) || (f>=97 && f<=122)) && f!='_')
    {
        return false;
    }
    for (int i = 1; i < n; i++)
    {
        f=l[i];
        if (!((f>=65 && f<=90) || (f>=97 && f<=122)) && f!='_' && !(f>=48 && f<=57))
    {
        return false;
    }
        
    }
    return true;
}
//label value
map<string,int> label_line;
// check for duplicate 
map<string,int> label_check;
// checking valid second expression
int secondexp(int l , string s){
    if(label_check[s]>0){
        return label_line[s];
    }
    const char* c=s.c_str();
    char* strend;
    long ans;
    if(s.size()>1 && s[0]=='0'){ 
        if (s[1]=='b')
        {
            ans=strtol(c+2,&strend,2);
        }
        else if (s[1]=='x')
        {
            ans=strtol(c+2,&strend,16);
        }
        else
        {
            ans=strtol(c+2,&strend,8);
        }
    }
    else
    {
        ans=strtol(c,&strend,10);
    // cout<<ans<<endl;
    }
    // cout<<strend-c;
    if ((int)s.size()!=strend-c)
    {
        int f=s[0];
        if (crlabel(s)==true)
        {
           err(l,"Label: '" + s + "' not declared in the scope");
        }
        else if (f>=48 && f<=57){
            err(l,s+ " is a wrong number format");
        }
        else
        {
            err(l ,"Invalid expression");
        }
        
    }
    
    return ans;
}
// storing opcodes
map<string,pair<int,int>> opcode{ 
    {"data",{0,1}},
    {"SET",{-1,1}},
    {"ldc",{0,1}},
    {"adc",{1,1}},
    {"ldl",{2,1}},
    {"stl",{3,1}},
    {"ldnl",{4,1}},
    {"stnl",{5,1}},
    {"add",{6,0}},
    {"sub",{7,0}},
    {"shl",{8,0}},
    {"shr",{9,0}},
    {"adj",{10,1}},
    {"a2sp",{11,0}},
    {"sp2a",{12,0}},
    {"call",{13,1}},
    {"return",{14,0}},
    {"brz",{15,1}},
    {"brlz",{16,1}},
    {"br",{17,1}},
    {"HALT",{18,0}}
};


// decimal number to hex number with length s
string toHex(int n, int s) {
    stringstream ss;
    ss<<hex<<n;
    string ans(ss.str());
    ans=string(max(0,s-(int)ans.size()),'0')+ans;
    if((int)ans.size()>s)ans=ans.substr(ans.size()-s,s);
    return ans;
}
//making label
void lbl(string s,int l,int lc){ 
    if (!(opcode[s].first==0 && opcode[s].second==0))
    {
        err(lc,"Label Name cannot be Mneumonic");
        return;
    }
    
    if(crlabel(s)==false){
        err(lc,"Invalid label format: "+ l);
        return;
    }
    if(label_check[s]==0){
        label_line[s]=l;
        label_check[s]++;
    }else{
        err(lc,"Label: " + s + " has been already declared");
    }
}
// finding the position of character in a string
int position(string a ,char c){
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i]==c)
        {
           return i;
        }
        
    }
    return -1;
}
// getting a substring from l to r
string getstr(string s, int l, int r){
    string ans="";
    for (int i = l; i <=r; i++)
    {
        ans+=s[i];
    }
    return ans;
}

vector<format> plines;
//main function 
int main(int argc , char* argv[]){
    string check="";
    if (argc!=2)
    {
        cout<<"Wrong command: command format: \n./asm file.asm\nKndly enter again";
        return 0;
    }
    string fname=string(argv[1]);
    ifstream input(fname);
    int k=position(fname,'.');
    fname=getstr(fname,0,k-1);
    string line;
    int ln=0; //program counter
    int lc=0; //line number 
    vector<vector<string>> instructions; //stores tokens of each line
    vector<string> code; // stores all the lines
    while (getline(input,line))
    {
    
        lc++;
        code.push_back(line);
        istringstream hh(line);
        string word;
        vector<string> inst; //tokens of the current line
        string label=""; // Label found in this line.
        int wc=0; 
        while(hh>>word){
            wc++;
            int d=0;
            if (word[0]==';')
            {
                break;
            }
            
            if (position(word,':')>=0)
            {
                if (wc==1 && word[0]==':')
                {
                    error.push_back({lc,"Invalid Label"});
                }
                else if (word[0]==':' && wc==2)//label has already come i.e. yuvi : 
               {
                  label=inst[0];
                  inst.clear();
                  word=getstr(word,1,word.size()-1);
               }
               else if (wc>=2)
               {
                    err(lc,"Invalid Label");
               }
               else// yuvi: 
               {
                 int p=position(word,':');
                 label=getstr(word,0,p-1);
                 word=getstr(word,p+1,word.size()-1);
               }
            }
            if(position(word,';')>=0){
                int k=position(word,';');
                word=getstr(word,0,k-1); // Discarding everything after ;
                
            }
               if (word!="")
               {
                    inst.push_back(word);
               }
                
               
            }
            if (label!="")
            {
                //handles the set instruction
                if(inst.size()>1 &&inst[0]=="SET" && inst.size()>1)
                {
                    lbl(label,secondexp(ln,inst[1]),lc);
                }
                else
                {
                    lbl(label,ln,lc);
                }
                
            }
  
            instructions.push_back(inst);
            if (inst.size()!=0 && inst[0]!="SET" )
            {
                ln++;
            }
        }
    
    //Second pass
            ofstream listingfile(fname+".l");
            ofstream logfile(fname+".log");
            ofstream binfile(fname+".o",ios::out|ios::binary);
            
            for (int i = 0; i < instructions.size(); i++)
            {
                if (instructions[i].size()==0)
                {
                    plines.push_back({-1,0});
                    continue;
                }
                if ((opcode[instructions[i][0]].first==0 && opcode[instructions[i][0]].second==0))
                {
                    err(i+1,instructions[i][0]+ " Mnemonic is Invalid");
                    plines.push_back({-1,0});
                }
                else
                {
                    if (instructions[i].size()<1+opcode[instructions[i][0]].second)
                    {
                        err(i+1,"Operand is missing");
                    }
                    format j;
                    j.opc=opcode[instructions[i][0]].first;
                    if (opcode[instructions[i][0]].second!=0)
                    {
                        if (instructions[i].size()>=2)
                        {
                            j.oprd=secondexp(i,instructions[i][1]);
                        }
                        else
                        {
                            j.oprd=secondexp(i,"0");
                        }
                        
                    }
                    else
                    {
                        j.oprd=0;
                    }
                    if(instructions[i][0]=="data"){  
                j.opc=j.oprd&0xff; 
                j.oprd=j.oprd>>8; 
            }
                if (instructions[i].size()>1+opcode[instructions[i][0]].second)
                {
                    err(i+1 , "Operand is missing");
                }
                plines.push_back(j);
                }
                
            }
            

        sort(error.begin(),error.end());
        if (error.size()==0)
        {
            cout<<"Compilation is successful"<<endl;
        }
        else
        {
            cout<<"Check the found errors in logfile";
            for (int i = 0; i < error.size(); i++)
        {
            logfile<<"ERROR: Line "<<error[i].first<<": "<<error[i].second<<endl;
        }
        }
        
        int line_num=0;
        map<int,int> c2;
        c2[13]++;
        c2[15]++;
        c2[16]++;
        c2[17]++;
        for (int i = 0; i < code.size(); i++)
        {
            listingfile<<toHex(line_num,8)<<" ";
            if (plines[i].opc!=-1)
            {
                int curropc=plines[i].opc;
                int curropr=plines[i].oprd;
                if (c2[curropc]>0)
            {
                curropr-=line_num+1;
            }
                char bin_instr[4]={(char)curropc,(char)(curropr&0xff),(char)((curropr&0xff00)>>8),(char)((curropr&0xff0000)>>16)};
            listingfile<<toHex(curropr,6)<<toHex(curropc,2)<<' ';
            binfile.write(bin_instr,4);
            line_num++;
            }
            listingfile<<code[i]<<'\n';
        }
        
       return 0;
    
}
