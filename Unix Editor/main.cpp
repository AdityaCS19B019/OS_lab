#include<iostream>
#include <bits/stdc++.h>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;
#include "Marksheet.h"




void add_student(string x, int a, int b, int c, int d)
{
    string filename("table.txt");
    fstream file;

    int total = a + b + c + d;

    file.open(filename, std::ios_base::app | std::ios_base::in);

    if (file.is_open())
        file << "\n " << x << "      " << a << "        " << b << "        " << c << "        " << d << "      " << total ;
}


string read_num(string s)
{
    // Create a text string, which is used to output the text file
    string myText;

    // Read from the text file
    ifstream MyReadFile(s);

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
    // Output the text from the file
    //cout << myText;
    string str = myText;
    return str;
    }
    // Close the file
    MyReadFile.close();

    return "";
}

void change_num(string s)
{
    // Create and open a text file
  fstream NewFile(s);

  // Write to the file
   if(read_num(s)=="1")
   {
       NewFile << "0";
   }
   else
   {
       NewFile << "1";
   }
  
  // cout << read_num(s);
  // Close the file
  NewFile.close();

}


string get_filestatus(int n)
{
    fstream file; 
    string filename("file_status.txt");

	file.open("file_status.txt"); 
	if (file.is_open())
	{
		string s;        
        int i;
		while(getline(file, s))
		{ 
			//cout << s << endl;
            //cout << to_string(s[0]) << "*" << to_string(n+48) << endl;
            if(to_string(s[0]) == to_string(n+48))
            {
              i = int(s[2])-48;
              //cout << i << endl;
              return to_string(i);
            }  
              //cout << s[2] << endl;
        }
    }           
    return "";
}

void change_filestatus(int n,int m)
{
    fstream file; 
    string filename("file_status.txt");

	file.open("file_status.txt"); 
	if (file.is_open())
	{
		string s, str="" ;   
        int i;   
		while(getline(file, s))
		{ 
			char s1[s.size()+1];
			strcpy(s1, s.c_str());
			char* token = strtok(s1, " ");
			vector<string> tokens;

            while (token != NULL)
			{
				tokens.push_back(token);
				token = strtok(NULL, " ");
			}
            
            if(tokens[0] != to_string(n))
            {   
                str += s + "\n";
            }   
            else
            {
                i = int(s[0])-48;
                str += to_string(i) + " " + to_string(m) + "\n";
            }
        }
        ofstream myfile("file_status.txt");
	    myfile << str;
	    myfile.close();
    }        

}


void add_to_queue(int x,int a,int b,int c,int d)
{
    string filename("queue.txt");
    fstream file;

    file.open(filename, std::ios_base::app | std::ios_base::in);

    if (file.is_open())
        file << x << " " << a << " " << b << " " << c << " " << d << " " << endl;
}

vector<int> pop_queue()
{
    string line = "";
    fstream file; 
    string filename("queue.txt");

	file.open("queue.txt"); 
	if (file.is_open())
	{
		string s, str="" ;   
        int i=0;   
		while(getline(file, s))
		{ 
            if( i != 0 )
            {
                str += s + "\n";
            }
            else
            {
                line = s;
                i = 2;
            }
        }
        ofstream myfile("queue.txt");
	    myfile << str;
	    myfile.close();
    }    

    char s1[line.size()+1];
	strcpy(s1, line.c_str());
    char* token = strtok(s1, " ");
    vector<int> tokens;

    while (token != NULL)
    {
        int i=stoi(token);
        tokens.push_back(i);
        token = strtok(NULL, " ");
    }    

    return tokens;

}





int main(){

    int option = 2;
    int rc = 0;
    string str1 , str2;
    

    // cout << "Press 1 to Dean or Press 2 to faculty or press 3 for Student\n";
    // cin >> edit ;

    switch (option){
        case 1 :
            Dean();
            break;
        case 2 :
            cout << "Enter Faculty ID : \n";
            cin >> rc;
           
           Faculty(rc);
           break;
        case 3 :
            cout << "Enter Student ID : \n";
            cin >> rc;
            Student(rc);
            break;
        default :
            cout << " Invalid Option Choosen \n";
            break;
    }

}


