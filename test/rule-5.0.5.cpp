int main(){
    float f=23.0;
    int t=f; //non compliant
    float f3=t; //non compliant
    float f2=(float)t; //compliant
    int q=(int)f; //non compliant
    return 0;
}