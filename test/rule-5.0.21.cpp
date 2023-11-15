int main(){
    int a=20;
    
    int b=~a; //non compliant
    
    unsigned int d=20; //compliant

    unsigned int z=~d;

    z|=2; // compliant
    d>>=2;// compliant
    d^=1;  // compliant
    if(~d==12) //compliant 

    
    int e=a|1 ; //non compliant
    b>>=2; //non compliant
    if(~b==12) // non compliant 
    b<<=3;  //non comploant

    return 0; 
}