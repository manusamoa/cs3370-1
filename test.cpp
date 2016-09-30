#include <iostream>
using namespace std;

// Runs a test by checking both the console output and expected int return number
bool runTest(string arguments, int expectedResponse, string expectedOutput) {
    arguments = "./portsetter.cpp.o " + arguments + " >testOut.txt";
    char* charPntr = &arguments[0];
    int statusCode = system(charPntr)/256;
    if(statusCode != expectedResponse) {
        system("rm testOut.txt");
        return false;
    }
    
    string diffArguments = "diff testOut.txt " + expectedOutput + " >garbage.txt";
    if(system(&diffArguments[0]) != 0) {
        system("rm testOut.txt");
        system("rm garbage.txt");
        return false;
    }
    system("rm testOut.txt");
    system("rm garbage.txt");
    return true;
    
}

// prints the test results in a pretty way :)
int printTestResult(string testName, bool testSuccess) {
    if (testSuccess) {
        cout << testName << " -Pass" << endl;
        return 0;
    }
    else {
       cout << testName << " -Fail" << endl;
       return 1;
    }
}

int main() {
    cout << "\nRunning Tests: " << endl;
    int totalFailedTests = 0;
    //Run Successful Tests
    totalFailedTests += printTestResult("No parameters prints usage and returns 0", runTest("",0,"portsetterTest/usageSuccess.txt"));
    totalFailedTests += printTestResult("'-h' argument prints usage and returns 0", runTest("-h",0,"portsetterTest/usageSuccess.txt"));
    totalFailedTests += printTestResult("'--help' argument prints usage and returns 0", runTest("--help",0,"portsetterTest/usageSuccess.txt"));
    totalFailedTests += printTestResult("'-p 4040' listens on port 4040 and returns 0", runTest("-p 4040",0,"portsetterTest/portSuccess.txt"));
    totalFailedTests += printTestResult("'--port 4040' listens on port 4040 and returns 0", runTest("--port 4040",0,"portsetterTest/portSuccess.txt"));
    
    //Run Error Catching Tests
    totalFailedTests += printTestResult("'help' argument should triggger invalid argument error and return 3", runTest("help",3,"portsetterTest/invalidArgument.txt"));
    totalFailedTests += printTestResult("'-help' argument should trigger invalid argument error and return 3", runTest("-help",3,"portsetterTest/invalidArgument.txt"));
    totalFailedTests += printTestResult("'--h' argument should trigger invalid argument error and return 3", runTest("--h",3,"portsetterTest/invalidArgument.txt"));
    totalFailedTests += printTestResult("'-h --help' arguments should trigger invalid argument number error and return 2", runTest("-h --help",2,"portsetterTest/invalidArgumentNumber.txt"));
    totalFailedTests += printTestResult("'-hs' argument should trigger invalid argument error and return 3", runTest("-hs",3,"portsetterTest/invalidArgument.txt"));
    totalFailedTests += printTestResult("'-p --port 9' arguments should trigger invalid argument number error and return 2", runTest("-p --port 9",2,"portsetterTest/invalidArgumentNumber.txt"));
    totalFailedTests += printTestResult("'-p 77 33' arguments should trigger invalid argument number error and return 2", runTest("-p 77 33",2,"portsetterTest/invalidArgumentNumber.txt"));
    totalFailedTests += printTestResult("'-p 0' arguments should trigger invalid portnumber error and return 1", runTest("-p 0",1,"portsetterTest/invalidPortnumber.txt"));
    totalFailedTests += printTestResult("'--port' argument should trigger invalid argument number error and return 2", runTest("--port",2,"portsetterTest/invalidArgumentNumber.txt"));
    totalFailedTests += printTestResult("'-p 90642' argument should trigger invalid portnumber error and return 1", runTest("-p 90642",1,"portsetterTest/invalidPortnumber.txt"));
    totalFailedTests += printTestResult("'-x 45321' arguments should trigger invalid argument error and return 3", runTest("-x 45321",3,"portsetterTest/invalidArgument.txt"));
    totalFailedTests += printTestResult("'-P 714' arguments should trigger invalid argument error and return 3", runTest("-P 714",3,"portsetterTest/invalidArgument.txt"));
    
    
    
    cout << "\nTotal Failed Tests: "  << totalFailedTests << endl;
    if(totalFailedTests == 0) {
        cout << "All Tests Pass!" << endl;
    }
    return 0;
}


