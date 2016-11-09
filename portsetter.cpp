#include <iostream>
#include <stdexcept>
#include <fstream>
#include <map>
#include <vector>
using namespace std;


enum { M_AUTHOR = 0, M_CLASS, M_ERROR, M_INVALID_ARG, M_INVLAID_NUM_ARGS, M_CONNECTED, M_INVALID_PORT_NUMBER };

vector<string> MESSAGES;
string USAGE_FILE = "portsetter.usage.";
string ABOUT_FILE = "portsetter.about.";

int convertToPortNumber(string strPortNum) {
    std::string::size_type sz;
    int portNum = stoi(strPortNum,&sz);
    if (sz == strPortNum.size() && portNum > 0 && portNum < 65536) {
        return portNum;
    }
    else {
        throw std::invalid_argument("");
    }
}

void printFile(string fileName) {
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open()) {
        while ( getline (myfile, line)) {
            cout << line << "\n";
        }
        myfile.close();
    }
}

// Prints correct usage
void usage() {
    printFile(USAGE_FILE);
}

void about() {
    printFile(ABOUT_FILE);
}

void version() {
    cout << "v1.0.2";
}

// Prints an error message passed as a string followed by the correct usage
void error(string errorMessage) {
    cout << MESSAGES[M_ERROR] << ": " << errorMessage << endl;
    usage();
}

string getSystemLanguage() {
    string envars[] = {"LANG","LC_MESSAGES","LC_ALL","LANGUAGE"};
    string sysLang = "";
    for (string var : envars) {
        if (getenv(&var[0]) != NULL) {
            string value = getenv(&var[0]);
            if (value != "" && value != "C" && value != "C.UTF-8") sysLang = value;
        }
        
    }
    if (sysLang == "") return "en";
    string returnString = "";
    //take the first two characters of the system language string
    returnString.push_back(sysLang.at(0));
    returnString.push_back(sysLang.at(1));
    return returnString;
}

void setProgramLanguage() {
    string langSymbol = getSystemLanguage();
    //set usage file
    USAGE_FILE += langSymbol;
    USAGE_FILE += ".txt";
    
    ABOUT_FILE += langSymbol;
    ABOUT_FILE += ".txt";
    
    string fileName = "portsetter.messages.";
    fileName += getSystemLanguage();
    fileName += ".txt";
    ifstream myfile(fileName);
    string line;
    if (myfile.is_open()) {
        while ( getline(myfile,line)) MESSAGES.push_back(line);
    }
    myfile.close();
}

int main(int argc, char* args[]) {
    
    //set the language settings
    setProgramLanguage();
    
    //interperate the command line arguments
    enum { HELP_FLAG=1, ABOUT_FLAG, VERSION_FLAG, PORT_FLAG };
    
    enum { INVALID_PORT=1, INVALID_ARG_NUM, INVALID_ARG };
    
    
    map<string,int> FLAGS;
    
    FLAGS["-h"] = HELP_FLAG;
    FLAGS["--help"] = HELP_FLAG;
    FLAGS["-?"] = HELP_FLAG;
    FLAGS["-!"] = ABOUT_FLAG;
    FLAGS["--about"] = ABOUT_FLAG;
    FLAGS["-v"] = VERSION_FLAG;
    FLAGS["--version"] = VERSION_FLAG;
    FLAGS["-p"] = PORT_FLAG;
    FLAGS["--port"] = PORT_FLAG;
    
    //are there any flags?
    if (argc == 1) {
        usage();
        return 0;
    }
    
    //what flag is present
    string flag = args[1];
    
    int flag_type = FLAGS[flag];
    
    //is it a valid flag type?
    if (flag_type == 0){
        error(MESSAGES[M_INVALID_ARG]);
        return INVALID_ARG;
    }
    
    //takes care of the single flag commands like -? or -!
    if(flag_type == HELP_FLAG || flag_type == ABOUT_FLAG || flag_type == VERSION_FLAG) {
        if(argc != 2) {
                error(MESSAGES[M_INVLAID_NUM_ARGS]);
                return INVALID_ARG_NUM;
            }
        if(flag_type == HELP_FLAG) usage();
        if(flag_type == ABOUT_FLAG) about();
        if(flag_type == VERSION_FLAG) version();
        return 0;
    }
    
    //now take care of the port flag
    if(flag_type == PORT_FLAG) {
        if(argc < 3) {
            error(MESSAGES[M_INVLAID_NUM_ARGS]);
            return INVALID_ARG_NUM;
        } 
        
        //this string will either be -e or a number
        string portString = args[2];
        
        
        //is the -e flag set? if so then set portString to the port in the env
        if(portString == "-e" || portString == "--environment"){
            if (argc > 4) {
               error(MESSAGES[M_INVLAID_NUM_ARGS]);
               return INVALID_ARG_NUM;
            }
            char* PORT_VAR = (char*)"PORT";
            if (argc == 4) PORT_VAR = args[3];
            // char* envPort = getenv(PORT_VAR);
            // cout << "var: " << PORT_VAR << "=";
            // if (envPort)
            //     cout << envPort << endl;
            portString = getenv(PORT_VAR);
        }// end if
        else {
            if (argc > 3) {
                error(MESSAGES[M_INVLAID_NUM_ARGS]);
                return INVALID_ARG_NUM;
            }
        }// end else
        
        try{
            int portNumber = convertToPortNumber(portString);
            cout << MESSAGES[M_CONNECTED] << ": " << portNumber << endl;
            return 0;
        }
        catch (const std::invalid_argument& e) {
            error(MESSAGES[M_INVALID_PORT_NUMBER]);
            return INVALID_PORT;
        }
    }
}




