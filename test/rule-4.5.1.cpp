int main(){
bool
 b1 = true;
bool
 b2 = false;
 if ( b1 & b2 ) // Non-compliant

 
if ( b1 < b2 )  // Non-compliant

if (b1 | b2)  // Non-compliant


if ( b1 ^ b2 )  // Non-compliant

if(~b1 && ~b2) //Non-compliant

if ( b1 == false ) // Compliant

if ( b1 == b2 ) // Compliant

 
if ( b1 != b2 ) // Compliant

 
if ( b1 && b2 ) // Compliant


if ( !b1 )  // Compliant

int p=b1?3:7;  // Compliant

 return 0;
} // namespace std;

