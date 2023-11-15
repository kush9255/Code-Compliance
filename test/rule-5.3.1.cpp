int main(){
    int a=10,b=20,c=31,d=29;
if ((a < b) && (c < d))     // Compliant
if (1 && (c > d))          // Non-compliant
if ((a < b) && (c + d))    // Non-compliant
if(!0)      // Non-compliant
if(!2)       // Non-compliant
if(!false)  // compliant
return 0;
}