int main(){
    int a[2]={10,20};
    enum { COLOUR_0, COLOUR_1, COLOUR_2, COLOUR_COUNT } colour;

    int b=a[COLOUR_1]; //compliant

if ( COLOUR_0 == colour )  // Compliant

if ( ( COLOUR_0 + COLOUR_1 ) == colour )  // Compliant

if ( colour < COLOUR_COUNT )  // Non-compliant

 if(COLOUR_1 * COLOUR_2)  // Non-compliant

 int c= COLOUR_2/COLOUR_1;  // Non-compliant

 int d=COLOUR_2%COLOUR_1; //// Non-compliant
    return 0;
}