#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
const int GET = 1;
const int PUT = 2;
const int HEAD = 3;
const int TRACE = 4;
const int OPTIONS = 5;
const int DEL = 6;
const int POST = 7;

void doPut(string* AstatusLine, string RfirstLine, string Rbody);
void doPost(string Rbody, string* AstatusLine);
void doOptions(string* Astatusline);
void doTrace(string* Abody, string* AstatusLine, string Rbody );
void writeFile(string newFileName, string* Abody, string* AstatusLine);
void doDelete(string* AstatusLine, string RfirstLine);
void doGetOrHead(int type, string* Abody, string* AstatusLine, string RfirstLine);
bool isFileExists(string fileName);
void doHeader(string* Aheader , int bodyLength , int type);
void parse(string& firstline, string& header, string& body, char buffer[1024]);
int getRequsetType(string requestFirstLine);
string getLang(string firstLine);

