/*
*	crypto by Christine Wendell
*
*	This program encodes and decodes entered sentences
*	in either caesar or atbash ciphers based on user input.
*	The examples of cryptography used here are based on 
*	Classical Cryptography.
*/	

#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

//Builds the results string alphabetically
string buildADFGVX(string A, string B, string C, string D, string E, string F){
   string result = "";

   //Build the sentence by columns in alphabetical order
   for(int i = 0; i < A.length(); i++){
      result += A[i];
   }
   for(int i = 0; i < B.length(); i++){
      result += B[i];
   }
   for(int i = 0; i < C.length(); i++){
      result += C[i];
   }
   for(int i = 0; i < D.length(); i++){
      result += D[i];
   }
   for(int i = 0; i < E.length(); i++){
      result += E[i];
   }
   for(int i = 0; i < F.length(); i++){
      result += F[i];
   }

   return result;
}

//Sorts each letter into columns based on the keyword to
//prep for alphabetical sorting
string transpose(string polyb){
   //Each of these strings will reflect a letter from the key to act as columns
   string T, I, G, E, R, S = "";
   int oddList, evenList = 0;   //keeps track of when to switch lists

   //sorts the letters into columns based on the keyword
   for(int i = 0; i < polyb.length(); i++){
	//if the value is even (0,2,4)
	if(i % 2 == 0){
	   if(evenList == 0){
	      T += polyb[i];
	      evenList++;
	   }
	   else if(evenList == 1){
	      G += polyb[i];
	      evenList++;
	   }
	   else{
	      R += polyb[i];
	      evenList = 0;
	   }
	}
	//if the value is odd (1,3,5)
	if(i % 2 == 1){
	   if(oddList == 0){
	      I += polyb[i];
	      oddList++;
	   }
	   else if(oddList == 1){
	      E += polyb[i];
	      oddList++;
	   }
	   else{
	      S += polyb[i];
	      oddList = 0;
	   }
	}
   }
   //Since the columns will be sorted alphabetically, the order will
   //go: E, G, I, R, S, T
   return buildADFGVX(E, G, I, R, S, T);
}

//Builds the polybius square needed to initially encrypt the input
//into ADFGVX form
/* The polybius square form:
    A D F G V X
 A|
 D|
 F|
 G|
 V|
 X|
*/
string polybius(char curr, string square[]){
   //The polybius square requires these letters along the top and side
   //to encode each letter within the matrix
   string matrixVert = "ADFGVX";
   string matrixHoriz = "ADFGVX";
   string result = "";

   //Values are hard-coded due to the size of the key square remaining unchanged
   for(int i = 0; i < 6; i++){
	if(isupper(curr)) curr = tolower(curr);

	string squareLine = square[i];

	for(int j = 0; j < 6; j++){
	   if(curr == squareLine[j]){
	      //based on the letter's location in the square,
	      //insert the resulting adfgvx value pair into the string
	      result += matrixVert[i];
	      result += matrixHoriz[j];
	   } 
	}
   }
   return result;
}

//ADFGVX info from: https://en.wikipedia.org/wiki/ADFGVX_cipher
//Encrypts and returns a sentence encrypted with the ADFGVX cipher
string ADFGVXEncrypt(string input){
   //normally this cipher uses randomly generated keys, but due to its complexity
   //these will be hard-coded
   string keySquare[] = {"g9ysej", "vpwct5", "6xl320", "4mhadq", "obfi1k", "znu78r"};
   string result = "";
   string polybResult = "";

   //The key used for this encryption is "TIGERS"

   for(int i = 0; i < input.length(); i++){
	char currLett = input[i];
	
	//constructing the polybius square one character at a time
	polybResult += polybius(currLett, keySquare);
   }
   
   //performing columnar transposition on the results from the square
   result = transpose(polybResult);

   return result;
}

//Caesar Cipher info from: http://practicalcryptography.com/ciphers/classical-era/caesar/
//Encrypts a sentence using a caesar cipher and the shift provided by the user
string caesarEncrypt(string input, int shift){
   string result = "";

   for(int i = 0; i < input.length(); i++){
	//If uppercase, calculate the new letter using the shift
	if(isupper(input[i])){
	  int cur = input[i] + shift;
	  
	  //If the letter after shifting is out of alphabetic range,
	  //subtract and add the numeric values of Z and A to bring it in range
	  if(cur > 'Z') cur = cur -'Z' + 'A' - 1;

	  char resultCur = cur;

	  result += resultCur;
	}
	//If lowercase, perform the same calculation but 
	//different values to reach the lowercase values in ASCII
	else if(islower(input[i])){
	  
	  int cur = input[i] + shift;

          //If the letter after shifting is out of alphabetic range,
          //subtract and add the numeric values of z and a to bring it in range
	  if(cur > 'z') cur = cur - 'z' + 'a' - 1;

	  char resultCur = cur;

	  result += resultCur;
	}
	//If any punctuation or spaces are detected,
	//just add them into the result
	else result += input[i];
   }
   return result;
}

//Caesar Cipher info from: http://practicalcryptography.com/ciphers/classical-era/caesar/
//Decrypts a sentence using a caesar cipher and the shift provided by the user
string caesarDecrypt(string input, int shift){
   string result = "";

   for(int i = 0; i < input.length(); i++){
        //If uppercase, calculate the new letter using the shift
        if(isupper(input[i])){
           int cur = input[i] - shift;

	   //If the letter after shifting is out of alphabetic range,
           //subtract and add the numeric values of Z and A to bring it in range
	   if(cur < 'A') cur = cur + 'Z' - 'A' + 1;

	   char resultCur = cur;

	   result += resultCur;
        }
        //If lowercase, perform the same calculation but
        //different values to reach the lowercase values in ASCII
        else if(islower(input[i])){
           int cur = input[i] - shift;
	
	   //If the letter after shifting is out of alphabetic range,
           //subtract and add the numeric values of z and a to bring it in range
	   if(cur < 'a') cur = cur + 'z' - 'a' + 1;

	   char resultCur = cur;

	   result += resultCur;
        }
        //If any punctuation or spaces are detetced,
        //just add them into the result
        else result += input[i];
   }
   return result;
}

//atBash information from: http://practicalcryptography.com/ciphers/classical-era/atbash-cipher/
//Encrypts a sentence using an atbash cipher and returns the result
string atBashEncrypt(string input){
   string upperKey = "ZYXWVUTSRQPONMLKJIHGFEDCBA";   //Keys are to help translate each letter to atbash
   string lowerKey = "zyxwvutsrqponmlkjihgfedcba";
   string upperBase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  //Bases are to help find the location in the key string
   string lowerBase = "abcdefghijklmnopqrstuvwxyz";  //the letters need to be directed to

   string result = "";

   //Loops through the string and translate each character with the uppercase
   //or lowercase keys respectively
   for(int i = 0; i < input.length(); i++){
	char currLetter = input[i];
	char atBash, resLetter;

	if(isupper(currLetter)){
	    atBash = upperBase.find(currLetter);

	    resLetter = upperKey[atBash];

	    result += resLetter;
	}
	else if(islower(currLetter)){
	    atBash = lowerBase.find(currLetter);

	    resLetter = lowerKey[atBash];

	    result += resLetter;
	}
	//This allows punctuation and spaces to be inserted into the result
	else result += input[i];
   }
   return result;
}

//atBash information from: http://practicalcryptography.com/ciphers/classical-era/atbash-cipher/
//Decrypts a sentence using an atbash cipher and returns the result
string atBashDecrypt(string input){
   string upperKey = "ZYXWVUTSRQPONMLKJIHGFEDCBA";   //Keys are to help translate each letter to atbash
   string lowerKey = "zyxwvutsrqponmlkjihgfedcba";
   string upperBase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  //Bases are to help find the location in the key string
   string lowerBase = "abcdefghijklmnopqrstuvwxyz";  //the letters need to be directed to

   string result = "";

   //Loops through the string and translate each character with the uppercase
   //or lowercase keys respectively
   for(int i = 0; i < input.length(); i++){
        char currLetter = input[i];
        char atBash, resLetter;

        if(isupper(currLetter)){
            atBash = upperKey.find(currLetter);

            resLetter = upperBase[atBash];

            result += resLetter;
        }
        else if(islower(currLetter)){
            atBash = lowerKey.find(currLetter);

            resLetter = lowerBase[atBash];

            result += resLetter;
        }
	//This allows punctuation and spaces to be inserted into the result
        else result += input[i];
   }
   return result;
}

int main(){

   char mode, cipherType;  //holds the user's choices of mode and type of cipher
   string sentence;	   //holds the inputted message from the user
   int shift;		   //holds the value for the caesar shift
   int started = 0;	   //flag variable that indicates the user has quit the program

   cout << endl << endl;
   cout << "	*******Welcome to Crypto!*******	";
   cout << endl << endl;

   while(started == 0){
   	cout << "Enter 'e' to Encrypt, 'd' to Decrypt, or 'q' to Quit:" << endl;
   	cin >> mode;

	cout << endl;

	//Initializes the Encryption cycle ignoring case
   	if(mode == 'e' || mode == 'E'){
	   cout << "Enter the sentence you would like to encode:" << endl;
	  
	   cin.ignore();	//This clears the whitespace left by cin for getline to work
	   getline(cin, sentence);
	   cin.clear();		//clearing the cin buffer

           cout << endl << "Enter 'c' to use Caesar Cipher, 'a' for Atbash Cipher, or 'x' for ADFGVX Cipher:" << endl;
           cin >> cipherType;

	   //Initializes the Caesar Cipher cycle ignoring case
           if(cipherType == 'c' || cipherType == 'C'){
	     cout << endl << "Enter numeric shift amount: " << endl;

	     cin >> shift;	

	     string result = caesarEncrypt(sentence, shift);

	     cout << endl << "Original Sentence: " << sentence;
	     cout << endl << "Encrypted Message: " << result << endl << endl;		

           }
	   //Initializes the Atbash Cipher cycle ignoring case
           else if(cipherType == 'a' || cipherType == 'A'){
	     string result = atBashEncrypt(sentence);

	     cout << endl << "Original Sentence: " << sentence;
	     cout << endl << "Encrypted Message: " << result << endl << endl;

           }
	   //Initializes the ADFGVX Cipher cycle ignoring case
	   else if(cipherType == 'x' || cipherType == 'X'){
	     string result = ADFGVXEncrypt(sentence);

	     cout << endl << "Original Sentence: " << sentence << endl << endl;
	     cout << "When using the keyword 'TIGERS': " << endl;
	     cout << "Encrypted Message: " << result << endl << endl;	    

	   }
	   //Throws an error if unexpected data is entered
           else cout << "Entered values must be 'c', 'a', or 'x'" << endl;

   	}
	//Initializes the Decryption cycle ignoring case
   	else if(mode == 'd' || mode == 'D'){
	   cout << "Enter the sentence you would like to decode: " << endl;
	   
	   cin.ignore();	//This ignores the whitespace left by cin so getline can work
	   getline(cin, sentence);
	   cin.clear();		//clearing the buffer

	   cout << endl << "Enter 'c' to use Caesar Cipher or 'a' for Atbash Cipher" << endl;

	   cin >> cipherType;

	   //Initializes the Caesar Cipher cycle ignoring case
	   if(cipherType == 'c' || cipherType == 'C'){
	     cout << endl << "Enter numeric shift amount: "<< endl;

	     cin >> shift;

	     string result = caesarDecrypt(sentence, shift);

	     cout << endl << "Original Sentence: " << sentence << endl;
	     cout << "Decoded Message: " << result << endl << endl;
           }
	   //Initializes the Atbash Cipher cycle ignoring case
           else if(cipherType == 'a' || cipherType == 'A'){
	     string result = atBashDecrypt(sentence);

	     cout << endl << "Original Sentence: " << sentence << endl;
	     cout << "Decoded Message: " << result << endl << endl; 
           }
           else cout << "Entered values must be 'c' or 'a'" << endl;

   	}
	//Detects if the user wants to quit ignoring case
	else if(mode == 'q' || mode == 'Q'){
	   cout << "	   *******Bye for now!*******" << endl << endl;
	   started = 1;
	}
	//Throws an error if unexpected data is entered
   	else cout << "Entered values must be 'e', 'd', or 'q'" << endl;
   }	
   return 0;
};
