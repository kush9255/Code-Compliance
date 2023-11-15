int fun(){
    return 3;
}
bool fn3(){
    return true;
}
int main(){
    int sum=0;
    int pass=0;
   for(int i=10;i;i--){   //non compliant
    sum+=i;
   }
   int k=2;
   if(k){    //non compliant
   pass=1;
   }
   if(k>=0)   //compliant

    if(k=9){  //non compliant
        
    }
    if (int saa = fun()) //compliant
   while(k){
    sum+=k;
    k--;
   }

    if(k && k<=3)  //non compliant
   while ( bool flag = fn3 ( ) )
   return 0;
}