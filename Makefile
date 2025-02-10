# PIN 工具目录 (请根据你的 Pin 安装路径修改)
PIN_ROOT ?= /home/seondst/Desktop/Code/pin-3.28
PIN_KIT  := $(PIN_ROOT)/source/tools

# 编译器
CXX      := g++
CXXFLAGS := -Wall -Wextra -O2 -std=c++11 -fPIC -DPIN_CRT -D__PIN__ -I$(PIN_ROOT)/source/include/pin -I$(PIN_ROOT)/source/include/pin/gen

# 链接选项
LDFLAGS  := -shared -Wl,-Bsymbolic -Wl,--no-undefined

# 目标
TOOL_NAME := trace_tool
TOOL_SRC  := trace_tool.cpp
TOOL_OBJ  := trace_tool.so

# 生成目标
all: $(TOOL_OBJ)

$(TOOL_OBJ): $(TOOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(TOOL_OBJ)
