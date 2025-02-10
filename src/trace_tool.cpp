#include "pin.H"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class FunctionTracer {
public:
    std::vector<std::string> functionNames; // 存储调用的函数名（允许重复）

    void AddFunction(const std::string& funcName) {
        functionNames.push_back(funcName);
    }

    void DumpToFile(const std::string& filename) {
        std::ofstream outFile(filename);
        for (const auto& name : functionNames) {
            outFile << name << std::endl;
        }
        outFile.close();
    }
};

FunctionTracer tracer;  // 创建一个全局对象

// 该函数在每次函数调用时执行
VOID FunctionEntry(VOID* ip, std::string* name) {
    std::cout << "Function called: " << *name << " at address: " << ip << std::endl;
    tracer.AddFunction(*name);
}

// 对每个函数进行插桩
VOID InstrumentFunction(IMG img, VOID* v) {
    bool isMainImg = IMG_IsMainExecutable(img);
    if (!isMainImg) {
        return;
    }
    for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec)) {
        for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn)) {
            RTN_Open(rtn);

            std::string* funcName = new std::string(RTN_Name(rtn));
            if (funcName->empty() || funcName->find(".plt") != std::string::npos) {
                RTN_Close(rtn);
                continue;
            }
            // std::cout << "Instrumenting function: " << *funcName << " isMainImg: " << isMainImg << std::endl;
            
            // 在函数入口插桩
            RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)FunctionEntry,
                           IARG_INST_PTR,
                           IARG_PTR, funcName,
                           IARG_END);

            RTN_Close(rtn);
        }
    }
}

// 在程序结束时执行
VOID Fini(INT32 code, VOID* v) {
    tracer.DumpToFile("traced_functions.txt");
}

// Pin 工具的主入口
int main(int argc, char* argv[]) {
    if (PIN_Init(argc, argv)) {
        std::cerr << "This is a function tracing tool using Pin." << std::endl;
        return 1;
    }

    PIN_InitSymbols();

    IMG_AddInstrumentFunction(InstrumentFunction, 0);
    PIN_AddFiniFunction(Fini, 0);

    PIN_StartProgram(); // 启动目标程序
    return 0;
}
