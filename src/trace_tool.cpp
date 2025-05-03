#include "pin.H"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

const std::vector<std::string> excludeList = {
    ".plt", "sanitizer", "asan", "@plt", "__lsan", "__interceptor",
    "__interception", "cmplog", "__afl", "sancov", "ubsan", "fuzzer", "Fuzzer"
};
class FunctionTracer {
public:
    std::vector<const std::string*> functionNames;

    FunctionTracer() {
        functionNames.reserve(1000000);
    }

    void AddFunction(const std::string* funcName) {
        functionNames.push_back(funcName);
    }

    void DumpToFile(const std::string& filename) {
        std::ofstream outFile(filename);
        for (const auto* name : functionNames) {
            outFile << *name << std::endl;
        }
        outFile.close();
    }
};

FunctionTracer tracer;

std::unordered_map<ADDRINT, const std::string*> funcNameCache;

VOID FunctionEntry(VOID* ip, const std::string* name) {
    tracer.AddFunction(name);
}

bool shouldExclude(const std::string& name) {
    return name.empty() || std::any_of(excludeList.begin(), excludeList.end(),
        [&name](const std::string& pattern) {
            return name.find(pattern) != std::string::npos;
        });
}

VOID InstrumentFunction(IMG img, VOID* v) {
    if (!IMG_IsMainExecutable(img)) {
        return;
    }
    for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec)) {
        for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn)) {
            RTN_Open(rtn);
            std::string rtnName = RTN_Name(rtn);
            if (shouldExclude(rtnName)) {
                RTN_Close(rtn);
                continue;
            }
            ADDRINT addr = RTN_Address(rtn);
            const std::string* cachedName = nullptr;
            auto it = funcNameCache.find(addr);
            if (it == funcNameCache.end()) {
                cachedName = new std::string(rtnName);
                funcNameCache[addr] = cachedName;
            } else {
                cachedName = it->second;
            }
            
            RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)FunctionEntry,
                           IARG_INST_PTR,
                           IARG_PTR, cachedName,
                           IARG_END);
            RTN_Close(rtn);
        }
    }
}

VOID WriteToFile() {
    tracer.DumpToFile("traced_functions.txt");
}

VOID Fini(INT32 code, VOID* v) {
    WriteToFile();
    std::cout << "Function tracing finished." << std::endl;
}

VOID ThreadFini(THREADID tid, const CONTEXT* ctxt, INT32 code, VOID* v) {
    WriteToFile();
}

int main(int argc, char* argv[]) {
    if (PIN_Init(argc, argv)) {
        std::cerr << "This is a function tracing tool using Pin." << std::endl;
        return 1;
    }

    PIN_InitSymbols();

    IMG_AddInstrumentFunction(InstrumentFunction, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_AddThreadFiniFunction(ThreadFini, 0);

    PIN_StartProgram();
    return 0;
}
