#include<iostream>
#include <bits/stdc++.h>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;


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


void present_row(int n)
{
    string line = "";
    fstream file; 

	file.open("table.txt"); 
	if (file.is_open())
	{
		string s, str="" ;   
        int i=0;   
		while(getline(file, s))
		{ 
            if( i != 0 )
            {   
                if(i==n)
                str += s + "\n";

                // if(i==n)
                //     str += s + "\n";
                // else
                //     str += " Student" + to_string(i) + "\n";
            }
            else
            {
                str += s + "\n";
            }
            i++;
        }
        ofstream myfile("buffer.txt");
	    myfile << str;
	    myfile.close();
    }    
}

void present_column(int n)
{
    string line = "";
    fstream file; 
    
	file.open("table.txt"); 
	if (file.is_open())
	{
		string s, str="";   
        int i=0;   
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

            if(i==0)
            {
                str += "            Faculty" + to_string(n) + "\n";
                //str += s + "\n";
            }
            
            else
            {   
                str.append(" Student");
                str.append(to_string(i)) ;

                str += "      ";
                
                // int j=n;
                // while(j>1)
                // {
                //     str +=  "         " ;
                //     j--;
                // }
                str += tokens[n] ;
                str += "\n";
            }
            
            i++;
        }
        ofstream myfile("buffer.txt");
	    myfile << str;
	    myfile.close();
    }    
}



void save_table(int a, int b)
{
    fstream file1;
    fstream file2;
    file1.open("table.txt");
    file2.open("buffer.txt");
    string line = "", LINE = "", start ="";

    if(a==1)
    {   
        // just catch that LINE from updated file 
        if(file2.is_open())
        {
            string s;
            int i=0;
            while(getline(file2,s)) 
            { 
                if(i==1)
                  LINE += s + "\n"; 
                i++;  
            }
        }

        if(file1.is_open())
        {
            string s;
            int i=0;
            while(getline(file1,s))
            {
                if(i==b)
                   line += LINE ; 
                else
                   line += s +"\n"; 
                i++;  
            }
        }

        ofstream myfile("table.txt");
	    myfile << line;
	    myfile.close();
        //cout << line;

    }

    else if(a==0)
    {   
        vector<string> newcol;
        if(file2.is_open()) // Collecting updated values in a vector
        {   
            string s;
            int i=0;
            
            while(getline(file2,s))
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

                if(tokens.size()>1)
                newcol.push_back(tokens[1]);
            }
        }

        // for(int i=0;i<newcol.size();i++)
        // {
        //     cout << newcol[i] << "*";
        // }


        if(file1.is_open())
        {
            string s;
            int i=0,flag=0;
            while(getline(file1,s)) 
            {   
                if(i==0)
                  line += s +"\n" ; 

                else
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
                    
                    line += " " + tokens[0] + "      " ;
                    
                    int j=1;
                    while(j<tokens.size())
                    {
                        if(j==b)
                          line += newcol[flag++] + "        ";
                        else  
                          line += tokens[j] + "        "; 
                        j++;
                    } 
                    line += "\n";

                }  
                i++;  
            }
        }        
    }

    ofstream myfile("table.txt");
    myfile << line;
    myfile.close();
    
    update_total();
        
    //cout << line;
}




int main()
{
    //---------------Reading 1 or 0 in "login.txt", Changing to 0 or 1 --------------------
                 
        // cout << read_num("login.txt") << "*";
        // change_num("login.txt");
        // cout << read_num("login.txt") << endl;

    //----------------------------------------------------  

        // cout << get_filestatus(4) << "*";
        // change_filestatus(4,7);
        // cout << get_filestatus(4) << endl;

    //----------------------------------------------------

        // add_to_queue(6,1,2,3,4);

        // vector<int> arrby4 = pop_queue();

        // for (auto i = arrby4.begin(); i != arrby4.end(); ++i)
        //     cout << *i << " ";
        
        // cout << endl << arrby4[1];    

    //-----------------------------------------------------    
        
        //add_student("CS19B032", 3,6,9,4);
        
        
        //present_row(3);
        //present_column(4);
        
    //-----------------------------------------------------

        //save_table(1,3);  // Row = 1, Column = 0
        //save_table(0,4); 
        //save_table(0,3); 

    //-----------------------------------------------------    
   
}