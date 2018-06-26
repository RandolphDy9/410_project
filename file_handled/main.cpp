#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// this is for printing the whole ascii
void readBits(long &read) {
	if (read == 0) {
		cout << 0;
	} else if (read == 1) {
		cout << 1;
	}
}

// this is only for printing the NRZL result to the console, NOT to the GtkWave.
void readNRZL(long &read) {
	if (read == 0) {
		cout << 1;
	} else if (read == 1) {
		cout << 0;
	}
}

// this is only for printing the Manchester result to the console, NOT to the GtkWave.
void readMan(long &read) {
	if (read == 0) {
		cout << 1;	
		cout << 0;
	} else if (read == 1) {
		cout << 0;	
		cout << 1;
	}
	cout << " ";
}

int main() {

	string name;
	int size;
	
	ofstream input("input.txt");
	string strEntered;	
	cout << "Enter String (10 characters or more)" << endl;
	cout << "(Press Enter if Done)" << endl;

	getline(cin, strEntered);
	system("cls");
	
	while (strEntered.length() < 10) {
		cout << "Input should be at least 10 characters.\nPlease re-enter: " << endl;
		getline(cin, strEntered);
		system("cls");
	}
	
	input << strEntered << endl;
	
	ifstream theFile("input.txt");
	
	getline(theFile, name);
	cout << endl << "Input: " << name << endl;

	size = name.length();	
	
	int store;
	int storeAscii[50];
	int storeBits[500];
	int x;
	const char *chars = name.c_str();
	
	// code for getting the binary value and stores it to storeAscii[x]. x is the iterator. Meaning per value x, naay 8 bits.
	for (x=0; x<size; x++) {
		store = (int)chars[x];	

		long rem, ib=1, sum=0;
		do {
			rem = store%2;
			sum = sum + (ib*rem);
			store = store / 2;
			ib = ib * 10;
			
		} while (store>0);		
		
		storeAscii[x] = sum;
	}
	
	// printing the binary equivalent
	cout << endl << "Binary equivalent: " << endl;
	for (x=0; x<size; x++) {
		cout << storeAscii[x] << " ";	
	}
	
	// code and printing hexadecimal value
	cout << endl << endl << "Hexadecimal Equivalent: " << endl;
	char hexaDeciNum[100];
	int i=0;
	
	for (x=0; x<size; x++) {
		store = (int)chars[x];
		
		while(store !=0) {   
	        // temporary variable to store remainder
	        int temp  = 0;
	         
	        // storing remainder in temp variable.
	        temp = store % 16;
	         
	        // check if temp < 10
	        if(temp < 10) {
	            hexaDeciNum[i] = temp + 48;
	            printf("%c", hexaDeciNum[i]);
	            i++;
	        } else {
	            hexaDeciNum[i] = temp + 55;
	            printf("%c", hexaDeciNum[i]);
	            i++;
	        }	         
	        store = store/16;	        
	    }
	    cout << " ";
	}

	// Little Endian kay ang LSB. aw dba nu? hahaha 
	cout << endl << "\nPassed to Graph in LITTLE ENDIAN:" << endl;
	for (x=size-1; x>=0; x--) {
		store = (int)chars[x];

		long rem=0, ib=1, sum=0;
		do {	
			rem = store%2;
			sum = sum + (ib*rem);
			store = store / 2;
			ib = ib * 10;

			readBits(rem);	// for printing

		} while (store>0);	
	}
	
	cout << endl << endl << "(*NOTE: The Graph bitrate used is 2 bits per nanosecond.)";
	// Ang reason nganong 2 bits per nano second kay ang output sa Manchester kay tag duha. dba? 01 ug 10. 
	// So bale ang pag output sad nakos NRZL kay tag duha. 00 ug 11 pero since 2 bits per nanosecond man so ma consider ghapun ug 0 and 1 ang output.
	
	
	// NOTE: Di ko kapa run sa code nga iusa ang duha (NRZL ug Manchester) sa usa lang ka output. So ako nalang gibalik ang process nya gitagsa 
	// nako silag graph. Ang output.vpd kay ang NRZL, nya ang outman.vpd kay ang Manchester. Run it with GtkWave.
	
	// UG MAG CODE MO BA, DILI ADVISABLE ANG DRY CODES HA! DRY MEANING "DON'T REPEAT YOURSELF". AKONG PAG CODE ARIS GRAPH KAY GIBALIK RMAN NAKO GUD.
	// DI NA ADVISABLE HA PERO MIGANA MAN SOOOOO HAHAHAHAHA. Paras inyong future codes ha! Avoid tag DRY ;)
	
	
	// Magic Code for GTKWAVE -- NRZL
	ofstream outFile;
	
	outFile.open("output.vpd");
	
	// Timescale
	outFile << "$timescale" << endl;
	outFile << "\tips" << endl;
	outFile << "$end" << endl;
	outFile << "$scope NRZ-L $end" << endl;
	outFile << "$var wire 1 ! NRZL $end" << endl;
	outFile << "$upscope $end" << endl;
	outFile << "$enddefinitions $end" << endl;
	outFile << "#0" << endl;
	outFile << "$dumpvars" << endl;
	outFile << "b0 !" << endl;
	outFile << "$end" << endl;
	
	cout << endl << endl << "Graph Output of NRZL: " << endl;
	
	// For reading bits in NRZL
	for (x=size-1; x>=0; x--) {
		store = (int)chars[x];	

		long rem=0, ib=1, sum=0;
		do {
			
			rem = store%2;
			sum = sum + (ib*rem);
			store = store / 2;
			ib = ib * 10;

			readNRZL(rem);	// for printing
			
		} while (store>0);	
	}	

	int r = 0;	
	for (x=size-1; x>=0; x--) {
		store = (int)chars[x];	

		long rem, ib=1, sum=0;
		do {
			rem = store%2;
			sum = sum + (ib*rem);
			store = store / 2;
			ib = ib * 10;
			
			if (rem == 1) {
				rem = 0;
			} else {
				rem = 1;
			}

		outFile << "b" << rem << " !" << endl;		// nagrepeat rani dba? Agi kay 2 bits per nanosec man ta. Para ang output tagduha sad ka bits.
		outFile << "#" << (r+1) << endl;
		r++;
		outFile << "b" << rem << " !" << endl;
		outFile << "#" << (r+1) << endl;
		r++;
	
		} while (store>0);
		
	}
	
	theFile.close();
	outFile.close();
	system("gtkwave output.vpd");	// Outputs NRZL to this file.

	
	
	// Magic Code for GTKWAVE -- MANCHESTER
	ofstream outFile2;
	
	outFile2.open("outman.vpd");
	
	// Timescale
	outFile2 << "$timescale" << endl;
	outFile2 << "\tips" << endl;
	outFile2 << "$end" << endl;
	outFile2 << "$scope Manchester $end" << endl;
	outFile2 << "$var wire 1 ! Manchester $end" << endl;	
	outFile2 << "$upscope $end" << endl;
	outFile2 << "$enddefinitions $end" << endl;
	outFile2 << "#0" << endl;
	outFile2 << "$dumpvars" << endl;
	outFile2 << "b0 !" << endl;
	outFile2 << "$end" << endl;
	
	cout << endl << "Graph Output of Manchester Encoding:" << endl;	
	
	// For reading bits in Manchester
	for (x=size-1; x>=0; x--) {
		store = (int)chars[x];	

		long rem=0, ib=1, sum=0;
		do {
			
			rem = store%2;
			sum = sum + (ib*rem);
			store = store / 2;
			ib = ib * 10;

			readMan(rem);	// for printing
			
		} while (store>0);	
	}		
	
	int m = 0;	
	for (x=size-1; x>=0; x--) {
		store = (int)chars[x];	

		long rem, rem1, rem2, ib=1, sum=0;
		do {
			rem = store%2;
			sum = sum + (ib*rem);
			store = store / 2;
			ib = ib * 10;
			
			if (rem == 1) {
				rem1 = 0;
				rem2 = 1;
			} else if (rem == 0) {
				rem1 = 1;
				rem2 = 0;
			}
			
		outFile2 << "b" << rem1 << " !" << endl;	// ang ipasa sa gtkWave.
		outFile2 << "#" << (m+1) << endl;
		m++;
		outFile2 << "b" << rem2 << " !" << endl;
		outFile2 << "#" << (m+1) << endl;
		m++;
	
		} while (store>0);
		
	}
	
	
	theFile.close();
	outFile2.close();
	system("gtkwave outman.vpd");	// outputs MANCHESTER to this file.
	system("PAUSE");
		
	return 0;
}

