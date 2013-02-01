#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <functional>

int main(int argc, char *argv[]){
    if(argc < 5){ return 0; }
    const std::string
        clang_path  = argv[1],
        target_path = argv[2],
        str_line    = argv[3],
        str_col     = argv[4];
    std::string other_options;
    for(int i = 5; i < argc; ++i){
        other_options += " ";
        other_options += argv[i];
    }
    std::string command = clang_path + " -cc1 -std=c++11  -fsyntax-only -code-completion-macros -code-completion-patterns -code-completion-at=" + target_path + ":" + str_line + ":" + str_col + " " + target_path;
    if(!other_options.empty()){
        command += other_options;
    }
    std::system((command + " > namekuji_complete_log.txt").c_str());
    std::ifstream ifile("namekuji_complete_log.txt");
    if(ifile.fail()){
        return 0;
    }
    std::string log_line;
    while(std::getline(ifile, log_line)){
        std::size_t i = log_line.find(" : ");
        if(i == log_line.npos){ continue; }
        log_line = log_line.substr(12, log_line.size() - 12);
        i = log_line.find(" : ");
        auto del = [](std::string &str, const std::string &target, const int space) -> std::string &{
            std::size_t off = 0;
            for(; ; ){
                std::size_t i = str.find(target, off);
                if(i == str.npos){ break; }
                str.erase(str.begin() + i, str.begin() + i + target.size());
                if(space && str[i - 1] != '*' && str[i - 1] != '&'){
                    str.insert(i, " ");
                    ++i;
                }
                off = i;
            }
            return str;
        };
        auto rep = [](std::string &str, const std::string &p, const std::string &q, const int alnum) -> std::string &{
            std::size_t off = 0;
            for(; ; ){
                std::size_t i = str.find(p, off);
                if(i == str.npos){ break; }
                if(alnum){
                    if((i > 0 && !std::isalnum(str[i - 1])) || (i < str.size() - 1 && !std::isalnum(str[i + 1]))){
                        str.replace(i, p.size(), q);
                    }
                }else{
                    str.replace(i, p.size(), q);
                }
                off = i + q.size();
            }
            return str;
        };
        auto def = [](std::string &str) -> std::string &{
            std::size_t off = 0;
            for(; ; ){
                std::size_t i = str.find("{#", off);
                if(i == str.npos){ break; }
                str.insert(str.find("#>", i), " = def");
                off = i + 2;
            }
            return str;
        };
        auto p = std::make_pair(log_line.substr(0, i), std::string(""));
        if(p.first == "Pattern"){ continue; }
        p.second = log_line.substr(i + 3, log_line.size() - i - 3);
        p.second = del(del(del(del(del(del(def(rep(p.second, "typename", "class", 1)), "[#", 0), "#]", 1), "<#", 0), "#>", 0), "{#", 0), "#}", 0);
        std::cout << "<namekuji>" << p.first << "#" << p.second << "\n";
    }
	return 0;
}
