/*
 * Copyright (C) 2004-2006 Felipe S�nchez-Mart�nez
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
 *  @author   Felipe S�nchez-Mart�nez - fsanchez@dlsi.ua.es
 */

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/wait.h>

#include <sys/times.h>
#include <sys/resource.h>

#include <cmath>

#include "Utils.H"

string Utils::translation_script;
string Utils::likelihood_script;
string Utils::translation_script2;
string Utils::likelihood_script2;
bool Utils::debug;

string 
Utils::translate(string script, const string& s) {
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

    char buf[256];
    int status;

    //Mandamos el texto 
    write(fd_in[1], (void*)s.c_str(), s.length());
    close(fd_in[1]);
    wait (&status); //Waiting for the child to finish
    if(status!=0) {
      cerr<<"Error: Child process could not execute the translation script.\n";
      cerr<<"Input string: ["<<s<<"]\n";
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
    return ret;
  }     
}

double 
Utils::likelihood(string script, const string& s) {
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
    char buf[256];
    int status;

    //Those descriptor that will not be use are closed
    close(fd_in[0]);
    close(fd_out[1]);
     
    //Mandamos el texto 
    write(fd_in[1], (void*)s.c_str(), s.length());
    close(fd_in[1]);
    wait (&status); //Waiting for the child to finish
    if(status!=0) {
      cerr<<"Error: Child process could not execute the likelihood script.\n";
      cerr<<"Input string: ["<<s<<"]\n";
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

vector<string>
Utils::split_string(const string& input, const string& delimiter) {  
  int pos;
  int new_pos;
  vector<string> result;
  string s="";
  pos=0;

  while (pos<(int)input.size()) {
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
Utils::print_debug(const double& d){
  if (debug)
    cerr<<d;
}
void 
Utils::print_debug(const int& i) {
  if (debug)
    cerr<<i;
}

double
Utils::log2(double v) {
  return (log(v)/log(2));
}
