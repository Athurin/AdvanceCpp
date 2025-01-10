由于之前写项目的时候，有第三方库以及准备好了工程模板，所以cmake的时候没有花很多心思。

这次想自己写一个工程模板来构建学习框架，构建的过程当中碰到了一些问题，浅浅记录一下。

## 1

首先说明一下，本人使用的是vs2022，Qt5的编译环境

其实如果只是单纯使用C++以及vs2022自带的库文件的话，cmake构建挺简单的。这里不过多赘述。
看看前几个项目中的cmakelists文件，就能明白如何构建普通c++项目，如何构建并生成静态库、动态库以及如何引用库文件等等。

麻烦的主要是Qt项目文件的配置。

以clock项目举例：
```cmake
cmake_minimum_required(VERSION 3.10) 

get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE) 

get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME) 
project(${CURRENT_DIR_NAME})

# 设置 Qt 库的地址（按需设置）右斜线 地址可能需要与系统环境变量一致
set(QT_PATH D:/Qt/qt_app/5.14.2/msvc2017_64)
# set(QT_PATH D:/Qt/qt_app/5.14.2/mingw73_64)
set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)

# 指明静态库所在路径 在 MinGW 环境下，静态库文件通常是以 .a 结尾
# 在 MSVC 环境下，静态库文件通常是以 .lib 结尾
# link_directories(${QT_PATH}/lib)
# find_library(QT5_CORE_LIBRARY Qt5Core.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_GUI_LIBRARY Qt5Gui.lib PATHS ${QT_PATH}/lib)


# 动态库连接 一次只接受一个参数
# find_library(QT5_CORE_LIBRARY Qt5Core.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_GUI_LIBRARY Qt5Gui.dll PATHS ${QT_PATH}/bin)


set(CMAKE_CXX_STANDARD 11) 

if(MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})



add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})


target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
) 
```
下面这段非常重要，指定了安装Qt的路径，方便查找lib文件夹下的静态库以及bin目录下的动态库
需要注意的是，这个Qt的安装路径下的编译方式一般有多个，
如果使用的vs2022就用msvc64版本，并将设置匹配的系统环境变量。
这里如果系统环境变量设置了mingw和msvc两个版本，最好根据编译器选择。
比如说，使用vs2022编译器，就把msvc环境变量放在mingW之前。否则，反之。
不然就会出现编译成功但是运行错误的情况。
由于动态库静态库的扩展名在不同的编译规则下不同，所以cmake脚本里面的内容也应该与环境变量保持一致。

```cmake
# 设置 Qt 库的地址（按需设置）右斜线 地址可能需要与系统环境变量一致
set(QT_PATH D:/Qt/qt_app/5.14.2/msvc2017_64)
# set(QT_PATH D:/Qt/qt_app/5.14.2/mingw73_64)
set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)
```

注释掉的部分是之前的试错，实际上不用这么麻烦。引用Qt模块只需要：
```cmake
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
) 
```
就可以了。


## 2

关于项目QhttpDemo的构建就比较麻烦一点了，Qt模块多了一个QNetwork以及使用了QEventloop
```cmake
cmake_minimum_required(VERSION 3.10) 

get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE) 

get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME) 
project(${CURRENT_DIR_NAME})

# 设置 Qt 库的地址（按需设置）右斜线 地址可能需要与系统环境变量一致
set(QT_PATH D:/Qt/qt_app/5.14.2/msvc2017_64)
# set(QT_PATH D:/Qt/qt_app/5.14.2/mingw73_64)
set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)

#第三方DLL库文件目录
set(DLL_SOURCE_DIR D:/github_proj/advanceC++/MyThirdparty)


# 指明静态库所在路径 在 MinGW 环境下，静态库文件通常是以 .a 结尾
# 在 MSVC 环境下，静态库文件通常是以 .lib 结尾
link_directories(${QT_PATH}/lib)
# find_library(QT5_CORE_LIBRARY Qt5Core.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_GUI_LIBRARY Qt5Gui.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_NETWORK_LIBRARY Qt5Gui.lib PATHS ${QT_PATH}/lib)

# 动态库连接 一次只接受一个参数
# find_library(QT5_CORE_LIBRARY Qt5Core.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_GUI_LIBRARY Qt5Gui.dll PATHS ${QT_PATH}/bin)

# 创建并设置libcrypto-1_1-x64库
# add_library(libcrypto-1_1-x64 SHARED IMPORTED)
# set_target_properties(libcrypto-1_1-x64 PROPERTIES IMPORTED_LOCATION "D:/github_proj/advanceC++/MyThirdparty/libcrypto-1_1-x64.dll")
# 创建并设置libssl-1_1-x64库
# add_library(libssl-1_1-x64 SHARED IMPORTED)
# set_target_properties(libssl-1_1-x64 PROPERTIES IMPORTED_LOCATION "D:/github_proj/advanceC++/MyThirdparty/libssl-1_1-x64.dll")

# 先获取DLL_SOURCE_DIR目录下所有.dll文件的列表，存储在DLL_FILES变量中
file(GLOB DLL_FILES "${DLL_SOURCE_DIR}/*.dll")

# 顺序错误 ；而且可执行文件名 也不是变量CURRENT_DIR_NAME
# 假设可执行文件名为${CURRENT_DIR_NAME}，.dll文件在${DLL_SOURCE_DIR}目录下，目标是复制到与可执行文件相同的目录
# add_custom_command(TARGET ${CURRENT_DIR_NAME} POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                          "${DLL_SOURCE_DIR}/libcrypto-1_1-x64.dll"
#                           $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )


set(CMAKE_CXX_STANDARD 11) 

if(MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)

# set(MAIN_CPP_FILES "main.cpp")  # 替换为实际的 main 函数所在文件名
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
set(MAIN_CPP_FILES "main.cpp")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")  # 移除已经添加的 main.cpp 文件

set(CPP_FILES ${MAIN_CPP_FILES} ${CPP_FILES})


qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})


# 先 生成可执行文件，再复制DLL库到可执行文件所在目录

add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})

# message("Target file directory: $<TARGET_FILE_DIR:${PROJECT_NAME}>")

# 为每个 DLL 文件生成复制命令
foreach(dll_file ${DLL_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${dll_file}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying ${dll_file} to the target directory"
    )
endforeach()

# 链接两个库
# target_link_libraries(${PROJECT_NAME} libcrypto-1_1-x64 libssl-1_1-x64)

target_link_libraries(${PROJECT_NAME} 
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
	Qt5::Network
)
```

使用了Evnetloop的项目会稍微麻烦一点，如果不注意构建顺序，可能会出现QEventLoop: Cannot be used without QApplication 
这样的报错。
需要将含有main函数的.cpp文件放在所有cpp文件的最优先的位置开始构建。
下面是实例代码：
```cmake
# set(MAIN_CPP_FILES "main.cpp")  # 替换为实际的 main 函数所在文件名
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
set(MAIN_CPP_FILES "main.cpp")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")  # 移除已经添加的 main.cpp 文件

set(CPP_FILES ${MAIN_CPP_FILES} ${CPP_FILES})
```

Qt在联网的时候，libssl-1_1-x64 与 libcrypto-1_1-x64 这两个.dll动态库文件是必不可少的
并且最好不要直接复制粘贴到Qt安装路径下面，而是根据需要，以第三方库的形式放在项目可执行文件.exe的同级目录下。

怎么使用cmake链接呢。这里使用直接在构建项目的时候就把第三方库文件夹下的.dll文件复制到可执行文件目录下。
简单省事。

```cmake
#第三方DLL库文件目录
set(DLL_SOURCE_DIR D:/github_proj/advanceC++/MyThirdparty)

# 先获取DLL_SOURCE_DIR目录下所有.dll文件的列表，存储在DLL_FILES变量中
file(GLOB DLL_FILES "${DLL_SOURCE_DIR}/*.dll")VERSION

# 先 生成可执行文件，再复制DLL库到可执行文件所在目录

add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})

# message("Target file directory: $<TARGET_FILE_DIR:${PROJECT_NAME}>")

# 为每个 DLL 文件生成复制命令
foreach(dll_file ${DLL_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${dll_file}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying ${dll_file} to the target directory"
    )
endforeach()
```

注意add_custom_command必须在add_executable之后执行

## 3

.gitignore文件是用来忽略那些不想控制版本的文件。（并不能使cmake在构建项目的时候忽略掉.git文件夹）

但是cmake当中如何忽略掉.git文件夹呢？
在顶级cmakelists.txt文件中加上：
```cmake
# 设置输出目录为顶层 bin 文件夹

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

set(EXCLUDE_DIRS ".git")
```

即可。

## 4

编译脚本start.bat
```bat
cd Debug
call "D:\vs2022\vs2022_app\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
cmake  -G "Visual Studio 17 2022"  -A x64 -DCMAKE_BUILD_TYPE=Debug  ../advanceCpp
msbuild /m advanceCpp.sln /p:Platform=x64 /p:Configuration=Debug
cd ../
pause
```

## 5

AdvanceCpp只是一个工程模板
完整的项目的目录结构应该是：

```
|---Debug
|---AdvanceCpp
    |---Cmakedemo
    |---......(其他项目)
    |---Cmakelists.txt
|---MyThirdparty(第三方库)
|---start.bat
```