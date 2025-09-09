```cpp
// 把字符串按 delim 分割成 vector<string>
    vector<std::string> split(const std::string& s, char delim = ' ') 
    {
        std::vector<std::string> tokens;
        std::string item;
        std::stringstream ss(s);
        while (std::getline(ss, item, delim))
            if (!item.empty())           // 跳过空串
                tokens.push_back(item);
        return tokens;
    }
```

