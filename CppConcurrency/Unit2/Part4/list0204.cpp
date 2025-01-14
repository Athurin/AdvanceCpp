#include <thread>
#include <string>
#include <iostream>

void open_document_and_display_gui(std::string const& filename)
{
    std::cout<<"文件"<<filename<<"正在工作..."<<"\n";
}

bool done_editing()
{
    return true;
}

enum command_type
{
    open_new_document
};


struct user_command
{
    command_type type;

    user_command():
        type(open_new_document)
    {}
};

user_command get_user_input()
{
    return user_command();
}

std::string get_filename_from_user()
{
    return "foo.doc";
}

void process_user_input(user_command const& cmd)
{}

//全局变量，避免嵌套定义
int i = 1000; //设置最多打开的文件数量，避免无休止地创建新线程
void edit_document(std::string const& filename)
{
    open_document_and_display_gui(filename);
    
    while(done_editing())
    {
        user_command cmd=get_user_input();
        if(cmd.type==open_new_document)
        {
            std::string const new_name=get_filename_from_user();
            std::thread t(edit_document,new_name); //1
            t.detach(); //2
            i--;

            if (i < 0) break;
        }
        else
        {
            process_user_input(cmd);
        }
    }
}

int main()
{
    edit_document("bar.doc");
}
