int main(){
    int a=2;
    int b=0;
    bool flg=false;
    
    int c=a?2:10;       //non compliant
    
    int d =a<2?14:1;    //compliant
    
    int e=flg?19:23;    //compliant
}