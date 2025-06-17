// Array and I/O operations test
int arr[5];
float farr[3];
string sarr[2];

// Initialize arrays
arr[0] = 10;
arr[1] = 20;
arr[2] = 30;
arr[3] = 40;
arr[4] = 50;

farr[0] = 1.5;
farr[1] = 2.5;
farr[2] = 3.5;

sarr[0] = "Hello";
sarr[1] = "World";

// Print array elements
print "Array elements:";
print arr[0];
print arr[1];
print arr[2];
print arr[3];
print arr[4];

print "Float array elements:";
print farr[0];
print farr[1];
print farr[2];

print "String array elements:";
print sarr[0];
print sarr[1];

// Input operations
int x;
float y;
string z;

print "Enter an integer:";
input x;
print "You entered:";
print x;

print "Enter a float:";
input y;
print "You entered:";
print y;

print "Enter a string:";
input z;
print "You entered:";
print z;

// Array operations with input
print "Enter array index (0-4):";
input x;
if (x >= 0 && x < 5) {
    print "Enter new value:";
    input arr[x];
    print "New value at index";
    print x;
    print "is:";
    print arr[x];
} else {
    print "Invalid index!";
} 