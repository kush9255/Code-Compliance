int main() {
    // violating case
    unsigned int a = 10;
 
    int b = -a; //compliant

    long l1=-30; //compliant
    
    char c2=-30; //compliant


    // valid case
    unsigned int c = -20; //non compliant
    
    unsigned long l2=-30; //Non compliant

    unsigned char c1=-20; //Non compliant
    
    return 0;
}
