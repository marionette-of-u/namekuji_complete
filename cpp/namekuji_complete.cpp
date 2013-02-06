#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <functional>
#include <cstdlib>
#include <cctype>
#include "clang-c/Index.h"

std::string rep(std::string str, const std::string &p, const std::string &q, const int alnum){
    std::size_t off = 0;
    for(; ; ){
        std::size_t i = str.find(p, off);
        if(i == str.npos){ break; }
        if(alnum){
            if((i > 0 && !std::isalnum(str[i - 1])) || (i + p.size() < str.size() - 1 && !std::isalnum(str[i + p.size()]))){
                str.replace(i, p.size(), q);
            }
        }else{
            str.replace(i, p.size(), q);
        }
        off = i + q.size();
    }
    return str;
}

typedef std::map<std::string, std::set<std::string>> map_type;

std::string make_completion_string(CXCompletionString completion_string, std::string &strxx){
    std::string key;
    for(std::size_t i = 0, n = clang_getNumCompletionChunks(completion_string); i < n; ++i){
        const CXCompletionChunkKind kind = clang_getCompletionChunkKind(completion_string, i);

        if(kind == CXCompletionChunk_Optional){
            make_completion_string(clang_getCompletionChunkCompletionString(completion_string, i), strxx);
            strxx += " = def";
            continue;
        }

        if(kind == CXCompletionChunk_VerticalSpace){
            continue;
        }

        CXString text = clang_getCompletionChunkText(completion_string, i);
        const char *cstr = clang_getCString(text);
        if(cstr){ strxx += cstr; }
        if(kind == CXCompletionChunk_ResultType && !strxx.empty() && strxx.back() != '*' && strxx.back() != '&'){
            strxx += " ";
        }
        if(kind == CXCompletionChunk_TypedText && key.empty()){
            key = cstr;
        }
        clang_disposeString(text);
    }
    return key;
}

std::pair<std::string, std::string> make_completion_result(CXCompletionResult *completion_result){
    std::string info;
    std::string word = make_completion_string(completion_result->CompletionString, info);
    return std::make_pair(word, info);
}

int main(int argc, char *argv[]){
    //char *argv[] = {
    //    "dummy",
    //    "C:/clang/main.cpp",
    //    "l", "c",
    //    "-cc1",
    //    "-std=c++11",
    //    "-code-completion-macros",
    //    "-code-completion-patterns",
    //    "-IC:/MinGW/msys/1.0/local/include",
    //    "-IC:/MinGW/lib/gcc/mingw32/4.7.2/include",
    //    "-IC:/MinGW/lib/gcc/mingw32/4.7.2/include/c++",
    //    "-IC:/MinGW/lib/gcc/mingw32/4.7.2/include/c++/mingw32"
    //};
    //const int argc = sizeof(argv) / sizeof(*argv);

    if(argc < 4){ return 0; }
    CXIndex idx = clang_createIndex(0, 0);
    if(!idx){ return 0; }

    CXTranslationUnit u = clang_parseTranslationUnit(idx, argv[1], argv + 4, argc - 4, 0, 0, (CXTranslationUnit_IncludeBriefCommentsInCodeCompletion << 1) - 1);
    if(!u){ return 0; }
    clang_reparseTranslationUnit(u, 0, 0, CXReparse_None);

    int line = strtol(argv[2], 0, 10), column = strtol(argv[3], 0, 10);
    CXCodeCompleteResults *res = clang_codeCompleteAt(u, argv[1], line, column, nullptr, 0, (CXCodeComplete_IncludeCodePatterns << 1) - 1);
    if(!res){ return 0; }

    map_type map;
    for(std::size_t i = 0, i_ = res->NumResults; i < i_; ++i){
        map_type::mapped_type set;
        auto r = make_completion_result(res->Results + i);
        map[r.first].insert(r.second);
    }

    for(auto iter = map.begin(), end = map.end(); iter != end; ++iter){
        std::cout << iter->first;
        for(auto set_iter = iter->second.begin(), set_end = iter->second.end(); set_iter != set_end; ++set_iter){
            std::cout << "#" << rep(*set_iter, "typename", "class", 1);
        }
        std::cout << "\n";
    }
 
    return 0;
}
