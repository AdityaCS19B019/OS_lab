#include<iostream>
#include <bits/stdc++.h>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;
#include "Marksheet.cpp"




void add_student(string x)
{
    FILE *fp = fopen("data.txt", "r");
    if (!fp) {
        cout << "Unable to open file" << endl;
        die("fopen");
        exit(-1);
    };

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    string s ;
    string data = "";
    int line_no = 0;
    while ((linelen = getline(&line, &linecap, fp)) != -1) {
      while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
        linelen--;
        s = string(line, line + linelen);

        if(linelen > 0){
            data += s +"\n" ;
          line_no++;   
        }   
    }
    free(line);
    fclose(fp);

    data += " " + x + "      -        -        -        -        00       ";

    int len = data.length();
    int fd1;
    char buf[len+1];
    strcpy(buf, data.c_str());
 

    fd1 = open("data.txt", O_RDWR | O_CREAT, 0644);
    if (fd1 == -1 || fd1 == 0) {
        perror("error adding user to datafile : ");
        exit(-1);
    }

    write(fd1, buf, strlen(buf));
    close(fd1);

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

    int option;
    int rc = 0;
    string str1 , str2;
    int t = 0;

    cout << "Press 1 to Dean or Press 2 to faculty or press 3 for Student\n";
    cin >> option ;
    string sss="";

    switch (option){
        case 1 :
            cout << "Press 1 to add student\n";
            cin >> t;
            if(t) 
            {
                cout << "Enter name of Student\n";
                cin >> sss;
                cout << sss;
                add_student(sss);
                cout << " added student";
            }
            // else
            // Dean();
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


