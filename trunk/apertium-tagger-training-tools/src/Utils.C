/*
 * Copyright (C) 2004-2006 Felipe Sánchez-Martínez
 * Copyright (C) 2006 Universitat d'Alacant
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */
/**
 * Utility functions. (source file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include <iostream>
#include <fstream>

#include <cstdio>

#include <sys/types.h>
#include <sys/wait.h>

#include <sys/times.h>
#include <sys/resource.h>

#include <apertium/utf_converter.h>

#include "Utils.H"

string Utils::translation_script;
string Utils::likelihood_script;
string Utils::translation_script2;
string Utils::likelihood_script2;
bool Utils::debug;

wstring 
Utils::translate(string script, const wstring& s) {
  int fd_in[2]; //Pipe descriptor (the father writes text to the child)
  int fd_out[2];//Pipe descriptor (the child writes the result to the father)

  if (pipe(fd_in)!=0) 
    cerr<<"Error creating input pipe\n";
  if (pipe(fd_out)!=0) 
    cerr<<"Error creating output pipe\n";
  
  if (fork()==0) { //Code for the child
    close(0);
    dup(fd_in[0]);
    close(1);
    dup(fd_out[1]);
     
    //Those descriptor that will not be use are closed
    close(fd_in[1]); 
    close(fd_out[0]);
     
    execlp(script.c_str(), script.c_str(), NULL);

    //If execution arrives at this point, we are in trouble
    perror("Error");
    close(fd_in[0]);
    close(fd_out[1]);
    exit(EXIT_FAILURE);
  } else { //Code for the father
    //Those descriptor that will not be used are closed
    close(fd_in[0]);
    close(fd_out[1]);

    //char buf[256];
    int status;

    //Mandamos el texto 
    //write(fd_in[1], (void*)s.c_str(), s.length());
    //close(fd_in[1]);
    FILE *fw=fdopen(fd_in[1], "w");
    for (int i=0; i<s.length(); i++)
      fputwc(s[i],fw);
    fclose(fw);
    

    wait (&status); //Waiting for the child to finish
    if(status!=0) {
      cerr<<"Error: Child process could not execute the translation script.\n";
      cerr<<"Input string: ["<<UtfConverter::toUtf8(s)<<"]\n";
      close(fd_out[0]);
      exit(EXIT_FAILURE);
    }

    //Now the father reads the result from the child
    //string ret="";
    //int nread;
    //do {
    //  nread=read(fd_out[0], buf, 255);
    //  buf[nread]='\0';
    //  ret+=buf;
    //} while (nread==255);        
    //close(fd_out[0]);
    wstring ret=L"";
    FILE *fr=fdopen(fd_out[0], "r");
    wint_t c;
    do {
      c=fgetwc(fr);
      if (c!= WEOF)
	ret+=c;
    } while (c != WEOF);
    fclose(fr);

    return ret;
  }     
}

double 
Utils::likelihood(string script, const wstring& s) {
  int fd_in[2]; //Pipe descriptor (the father writes text to the child)
  int fd_out[2];//Pipe descriptor (the child writes the result to the father)

  string str=UtfConverter::toUtf8(s);

  if (pipe(fd_in)!=0) 
    cerr<<"Error creating input pipe\n";
  if (pipe(fd_out)!=0) 
    cerr<<"Error creating output pipe\n";
  
  if (fork()==0) { //Code for the child
    close(0);
    dup(fd_in[0]);
    close(1);
    dup(fd_out[1]);
     
    //Those descriptor that will not be use are closed
    close(fd_in[1]); 
    close(fd_out[0]);
     
    execlp(script.c_str(), script.c_str(), NULL);

    //If execution arrives at this point, we are in trouble
    perror("Error");
    close(fd_in[0]);
    close(fd_out[1]);
    exit(EXIT_FAILURE);
  } else { //Code for the father
    char buf[256];
    int status;

    //Those descriptor that will not be use are closed
    close(fd_in[0]);
    close(fd_out[1]);
     
    //Mandamos el texto 
    write(fd_in[1], (void*)str.c_str(), str.length());
    close(fd_in[1]);
    wait (&status); //Waiting for the child to finish
    if(status!=0) {
      cerr<<"Error: Child process could not execute the likelihood script.\n";
      cerr<<"Input string: ["<<str<<"]\n";
      close(fd_out[0]);
      exit(EXIT_FAILURE);
    }

    //Now the father reads the result from the child
    string ret="";
    int nread;
    do {
      nread=read(fd_out[0], buf, 255);
      buf[nread]='\0';
      ret+=buf;
    } while (nread==255);
        
    close(fd_out[0]);
    //cerr<<"CAD:"<<ret.c_str()<<endl;
    return atof(ret.c_str());
  }     
}

int 
Utils::get_usage_time()
{
  int t;
  struct rusage usage;
  getrusage(RUSAGE_SELF,&usage);
  t=usage.ru_utime.tv_sec+usage.ru_stime.tv_sec;

  getrusage(RUSAGE_CHILDREN,&usage);
  t+=usage.ru_utime.tv_sec+usage.ru_stime.tv_sec;

  return t;
}

vector<wstring>
Utils::split_wstring(const wstring& input, const wstring& delimiter) {  
  int pos;
  int new_pos;
  vector<wstring> result;
  wstring s=L"";
  pos=0;

  while (pos<input.size()) {
    new_pos=input.find(delimiter, pos);
    if(new_pos<0)
      new_pos=input.size();
    s=input.substr(pos, new_pos-pos);
    result.push_back(s);
    pos=new_pos+1;
  }
  return result;
}

vector<string>
Utils::split_string(const string& input, const string& delimiter) {  
  int pos;
  int new_pos;
  vector<string> result;
  string s="";
  pos=0;

  while (pos<input.size()) {
    new_pos=input.find(delimiter, pos);
    if(new_pos<0)
      new_pos=input.size();
    s=input.substr(pos, new_pos-pos);
    result.push_back(s);
    pos=new_pos+1;
  }
  return result;
}

void
Utils::print_debug(const string& s) {
  if (debug)
    cerr<<s;
}

void
Utils::print_debug(const wstring& s) {
  if (debug)
    cerr<<UtfConverter::toUtf8(s);
}

void 
Utils::print_debug(const double& d){
  if (debug)
    cerr<<d;
}
void 
Utils::print_debug(const int& i) {
  if (debug)
    cerr<<i;
}

// wstring
// Utils::string2wstring(const string& s) {
//   //WARNING
//   wchar_t auxws[s.length()+1];
//   mbstowcs(auxws, optarg, strlen(optarg)+1);
//   auxws[strlen(optarg)]=L'\0';

//   return auxws;
// }

