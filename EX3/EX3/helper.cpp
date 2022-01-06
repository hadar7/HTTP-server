using namespace std;
#include "helper.h"



void parse(string& firstline, string& header, string& body, char buffer[1024])
{
	char* token;
	int currentPos = 0, lastPos;
	string bufferAsString(buffer);

	lastPos = currentPos = bufferAsString.find("\n");
	firstline = bufferAsString.substr(0, currentPos);

	currentPos = bufferAsString.find("\r\n\r\n");
	header = bufferAsString.substr(lastPos + 1, currentPos - lastPos);

	body = bufferAsString.substr(bufferAsString.find("\r\n\r\n") + 4);
}

int getRequsetType(string requestFirstLine)
{
	string reqType = requestFirstLine.substr(0, requestFirstLine.find(" "));
	int requestNum = 0;

	requestNum = reqType.compare("GET") ? requestNum : 1;
	requestNum = reqType.compare("PUT") ? requestNum : 2;
	requestNum = reqType.compare("HEAD") ? requestNum : 3;
	requestNum = reqType.compare("TRACE") ? requestNum : 4;
	requestNum = reqType.compare("OPTIONS") ? requestNum : 5;
	requestNum = reqType.compare("DELETE") ? requestNum : 6;
	requestNum = reqType.compare("POST") ? requestNum : 7;

	return requestNum;
}

string getLang(string firstLine)
{
	string lang;
	int firstPos = firstLine.find("?lang=");
	if (firstPos == -1)
		return "en";

	lang = firstLine.substr(firstPos + 6, 2);
	return lang;
}

string getFileName(string firstLine)
{
	string fileName;
	int spacePos = firstLine.find(" ");

	fileName = firstLine.substr(spacePos + 1, firstLine.length());
	spacePos = fileName.find(" ");
	int paramPos = fileName.find("?");
	if (paramPos > 0)
	{
		fileName = fileName.substr(1, paramPos - 1);
	}
	else
	{
		fileName = fileName.substr(1, spacePos - 1);
	}
	return fileName;
}



void doHeader(string* Aheader , int bodyLength , int type )
{
	if (type==TRACE)
	{
		(*Aheader).append("Content-Type: message/html\r\n");
	}
	else
	{
		(*Aheader).append("Content-Type: text/html\r\n");
	}
	(*Aheader).append("Content-Length: ");
	(*Aheader).append(to_string(bodyLength));
	(*Aheader).append("\r\n");
	(*Aheader).append("Connection: keep-Alive");
}
void doPut(string* AstatusLine, string RfirstLine, string Rbody)
{
	string fileName = "C:\\temp\\" + getFileName(RfirstLine);
	if (isFileExists(fileName))
	{
		*AstatusLine = "HTTP/1.1 200 OK";
	}
	else
	{
		*AstatusLine = "HTTP/1.1 201 Created";
	}
	ofstream file;
	file.open(fileName);
	if (!file)
	{
		*AstatusLine = "HTTP/1.1 500 Internal Server Error";
	}
	file << Rbody.c_str() << endl;
	file.close();

}
bool isFileExists(string fileName)
{
	ifstream f(fileName.c_str());
	return f.good();
}
void doPost(string Rbody, string* AstatusLine)
{
	cout << "Post request body:" << Rbody << endl;
	*AstatusLine = "HTTP/1.1 200 OK";
}
void doOptions(string* Astatusline)
{
	*Astatusline = "HTTP/1.1 200 OK";
}
void doTrace(string* Abody, string* AstatusLine , string Rbody)
{
	*AstatusLine = "HTTP/1.1 200 OK";
	*Abody = Rbody;
}
void doDelete(string* AstatusLine, string RfirstLine)
{
	string fileName = "C:\\temp\\" + getFileName(RfirstLine);
	if (remove(fileName.c_str())==0)
	{
		*AstatusLine = "HTTP/1.1 200 OK";
	}
	else
	{
		*AstatusLine = "HTTP/1.1 204 No Content";
	}
}
void doGetOrHead(int type ,string* Abody, string* AstatusLine, string RfirstLine)
{
	string lang = getLang(RfirstLine);
	string fileName = "C:\\temp\\" + getFileName(RfirstLine);
	int namePos = fileName.find(".");
	string newFileName = fileName.substr(0, namePos);
	string endFileName = fileName.substr(namePos, fileName.length());
	if (lang.compare("he") == 0)
	{
		newFileName.append("-he");
	}
	else if (lang.compare("en") == 0)
	{
		newFileName.append("-en");
	}
	else if (lang.compare("fr") == 0)
	{
		newFileName.append("-fr");
	}
	else
		*AstatusLine = "HTTP/1.1 404 Not Found";

	newFileName.append(endFileName);
	if (type == GET)
	{
		writeFile(newFileName, Abody, AstatusLine);
	}
	else
		*AstatusLine = "HTTP/1.1 200 OK";
}
void writeFile(string newFileName  , string* Abody, string * AstatusLine)
{
	char line[180];
	ifstream file;
	file.open(newFileName);
	if (!file.fail())
	{
		while (!file.eof())
		{
			file.getline(line, 180);
			(*Abody).append(line);
			(*Abody).append("\n");
		}
		file.close();
		*AstatusLine ="HTTP/1.1 200 OK";
		return;
	}
	file.close();
	*AstatusLine = "HTTP/1.1 404 Not Found";

}

