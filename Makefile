# 编译器和选项
CXX = g++
CXXFLAGS = -Wall -std=c++11
INCLUDES = -I. -Iasn1_group
LIBS = -L. -lhost -lserial  # 添加-L.在当前目录查找库

# 目标文件
TARGET = CellNinja_desktop
OBJS = main.o decode_preprocess.o

# 库文件路径
LIB_PATH = /usr/local/lib
HOST_LIB = libhost.so

# 默认目标
all: $(TARGET)

# 编译和链接主程序
$(TARGET): $(OBJS) check_lib
	$(CXX) -o $@ $(OBJS) $(LIBS)

# 编译C++文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 确保库文件已复制到系统路径
check_lib:
	@if [ ! -f $(LIB_PATH)/$(HOST_LIB) ]; then \
		echo "Installing $(HOST_LIB) to $(LIB_PATH)"; \
		sudo cp $(HOST_LIB) $(LIB_PATH)/; \
		sudo ldconfig; \
	fi

# 运行程序
run: $(TARGET)
	LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ./$(TARGET)

# 清理中间文件和目标文件
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean check_lib run