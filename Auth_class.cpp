#include <iostream>
#include <cstdio>
//#include <unistd.h>
#include <vector>
#include <fstream>
#include <string.h>
#include "Admin_class.cpp"
#include "H_S_F_class.cpp"
#include <string.h>
#include <unordered_map>
using namespace std;

// extern noofusers = 4;

class Authentication
{
public:
   // char data_read[1000];
   char roles[4];
   vector<vector<string>> authfile;
   unordered_map<int , char> role_map;

   void import_data()
   {
      role_map[1002] = 'A';
      role_map[1003] = 'S';
      role_map[1004] = 'H';
      role_map[1005] = 'F';
      FILE *file_;
      char buffer[100];
      file_ = fopen("Authentication.team_10", "r");
      while (!feof(file_)) // to read file
      {
         // function used to read the contents of file
         fread(&buffer[0], sizeof(buffer), 1, file_);
         // cout << buffer;
      }
      fclose(file_);
      int len = strlen(buffer) / 21;
      // cout << len << endl;
      for (int i = 0; i < len; i++)
      {
         string temp = "";
         vector<string> tmp1;
         int tempo1 = i * 21;
         for (int j = 0; j < 8; j++)
         {
            string s(1, buffer[tempo1 + j]);
            temp.append(s);
         }
         tmp1.push_back(temp);
         tempo1 += 9;
         temp = "";
         for (int j = 0; j < 6; j++)
         {
            string s(1, buffer[tempo1 + j]);
            temp.append(s);
         }

         tmp1.push_back(temp);
         roles[i] = buffer[(i * 21) + 16];
         authfile.push_back(tmp1);
      }
      // cout << authfile[1][0] << endl;
   }

   void validate(int iid, int giid)
   {
      A a;
      V v;
      int flag = 0;
      char role = role_map[giid];
      for (int p = 0; p < 4; p++)
      {
         int str_to_id = 0;
         int len = authfile[p][1].size();
         for (int k = 0; k < len; k++)
         {
            str_to_id = (str_to_id * 10) + (authfile[p][1][k] - 48);
         }
         cout << str_to_id << endl;
         if (str_to_id == iid)
         {
            flag = 1;
            cout << "Welcome " << authfile[p][0] << endl;
            if (role == 'A')
               a.Start();
            else
               v.Start(role);
            break;
         }
      }
      if (flag == 0)
      {
         cout << "### Your account didnot have access to the machine" << endl;
         cout << "Kindly login with another account" << endl;
         system("login");
      }
   }
};

int main()
{

   Authentication auth;
   auth.import_data();
   while (true)
   {
      cout << "Select one of the operation" << endl;
      cout << "Enter 1 to login" << endl;
      cout << "Enter 2 to close the application" << endl;
      int option;
      cin >> option;
      if (option == 1)
      {
         cout << " ### Login with your FreeBSD account to proceed ######" << endl;
         // system("login");
         int id;
         id = getuid();
         //id = 100000;
         int gid;
         gid = getgid();
         //gid = 1003;
         cout << id << endl;
         cout << "### validating your freebsd account #######" << endl;
         auth.validate(id, gid);
      }
      else if (option == 2)
      {
         cout << "***Application Closed successfully***" << endl;
         break;
      }
      else
         cout << "No such option exist" << endl;
      cout << "-------------------------------------------------------" << endl;
   }
}
