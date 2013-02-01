**namekuji_complete**
=================

Description
-----------

C++11 限定.   
clang\_complete の設定が難しすぎるので自分で簡易的な補完スクリプトを書いてみた.   
namekuji_complete.cpp をコンパイル, 実行できる環境 (C++11) と clang, neocomplcache (https://github.com/Shougo/neocomplcache) に依存.

Installation
============

* bin/namekuji_complete.cpp をコンパイルし, 実行可能なファイルを生成する.
* clang を http://clang.llvm.org/ からビルドし, 実行可能なファイルを生成する.
* neocomplcache を使えるようにする.
* `.vimrc` に後述する Setting examples に倣った設定を記述.

Setting examples
============

```vim
"---------------------------------------------------------------------------
" namekuji_complete

" namekuji_complete の実行形式ファイル
let g:namekuji_complete_binary = 'C:/namekuji_complete/namekuji_complete.exe'
" clang の実行形式ファイル
let g:namekuji_complete_clang_binary = 'C:/clang/clang.exe'
" その他オプション
let g:namekuji_complete_opts = '-IC:/MinGW/msys/1.0/local/include -IC:/MinGW/lib/gcc/mingw32/4.7.2/include -IC:/MinGW/lib/gcc/mingw32/4.7.2/include/c++ -IC:/MinGW/lib/gcc/mingw32/4.7.2/include/c++/mingw32'

"---------------------------------------------------------------------------
" neocomplcache
" 自動で起動できるようにしておく
let g:neocomplcache_enable_at_startup = 1

" 必須
if !exists('g:neocomplcache_filename_include_exts')
  let g:neocomplcache_filename_include_exts = {}
endif
let g:neocomplcache_filename_include_exts.cpp = ['', 'h', 'hpp', 'hxx']

" 必須
if !exists('g:neocomplcache_omni_patterns')
  let g:neocomplcache_omni_patterns = {}
endif
let g:namekuji_omni_pattern = '[^.[:digit:] *\t]\%(\.\|->\)\|\h\w*::'
let g:neocomplcache_omni_patterns.cpp = g:namekuji_omni_pattern
let g:neocomplcache_omni_patterns.cc = g:namekuji_omni_pattern
let g:neocomplcache_omni_patterns.cxx = g:namekuji_omni_pattern
let g:neocomplcache_omni_patterns.h = g:namekuji_omni_pattern
let g:neocomplcache_omni_patterns.hpp = g:namekuji_omni_pattern
let g:neocomplcache_omni_patterns.hxx = g:namekuji_omni_pattern
```
