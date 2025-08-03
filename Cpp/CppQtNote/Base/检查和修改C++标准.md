#### 方法 A：Visual Studio IDE 图形界面

1. 右键项目 → **Properties（属性）**
    
2. **Configuration Properties → C/C++ → Language**
    
3. **C++ Language Standard** 下拉框里选：
    
    - `ISO C++20 Standard (/std:c++20)`
        
    - 或 `Preview - Features from the Latest C++ Working Draft (/std:c++latest)`（如果你想用 C++23 特性）

如果使用cmake构建项目，应该检查cmake有没有
`set(CMAKE_CXX_STANDARD 11)` 修改需要的版本

