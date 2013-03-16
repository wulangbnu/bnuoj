#ifndef SPOJHANDLER_H_INCLUDED
#define SPOJHANDLER_H_INCLUDED

#include "curl/curl.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <map>
#include <vector>

#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/user.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#define MAX_DATA_SIZE 655350
#define NEED_JUDGE 2
#define MAX_WAIT_TIME 240

using namespace std;


char dec2hexChar(short int n)
{
    if ( 0 <= n && n <= 9 ) return char( short('0') + n );
    else if ( 10 <= n && n <= 15 )return char( short('A') + n - 10 );
    else return char(0);
}
short int hexChar2dec(char c)
{
    if ( '0'<=c && c<='9' ) return short(c-'0');
    else if ( 'a'<=c && c<='f' ) return ( short(c-'a') + 10 );
    else if ( 'A'<=c && c<='F' ) return ( short(c-'A') + 10 );
    else return -1;
}

string escapeURL(const string &URL)
{
    string result = "";
    for ( unsigned int i=0; i<URL.size(); i++ )
    {
        char c = URL[i];
        if (
            ( '0'<=c && c<='9' ) ||
            ( 'a'<=c && c<='z' ) ||
            ( 'A'<=c && c<='Z' ) ||
            c=='/' || c=='.'
        ) result += c;
        else {
            int j = (short int)c;
            if ( j < 0 ) j += 256;
            int i1, i0;
            i1 = j / 16;
            i0 = j - i1*16;
            result += '%';
            result += dec2hexChar(i1);
            result += dec2hexChar(i0);
        }
    }
    return result;
}

string deescapeURL(const string &URL)
{
    string result = "";
    for ( unsigned int i=0; i<URL.size(); i++ )
    {
        char c = URL[i];
        if ( c != '%' ) result += c;
        else {
            char c1 = URL[++i];
            char c0 = URL[++i];
            int num = 0;
            num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
            result += char(num);
        }
    }
    return result;
}

char username[1000];
char password[1000];
char tfilename[1000];
char judger_string[200];
char logfile[1000];
int lowprivid;
map <string,string> corrlang;

void init() {

    FILE * fin=fopen("config.ini","r");
    fscanf(fin,"%s%d%s%s%s%s",judger_string,&lowprivid,logfile,username,password,tfilename);
    fclose(fin);
    strcat(judger_string,"\nSPOJ");
    corrlang["1"]="41";
    corrlang["2"]="11";
    corrlang["3"]="10";
    corrlang["4"]="22";
    corrlang["5"]="4";
    corrlang["6"]="27";
    corrlang["7"]="5";
    corrlang["8"]="3";
    corrlang["9"]="17";
    corrlang["10"]="7";
}

void writelog(char *);

#endif // SPOJHANDLER_H_INCLUDED
