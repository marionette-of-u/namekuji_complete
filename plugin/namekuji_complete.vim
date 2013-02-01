au FileType cpp,cc,cxx,h,hpp,hxx call <SID>NamekujiCompleteInit()

autocmd FileType cpp,cc,cxx,h,hpp,hxx setlocal completefunc=NamekujiComplete
autocmd FileType cpp,cc,cxx,h,hpp,hxx setlocal omnifunc=NamekujiComplete

let b:namekuji_complete_list = []

function! s:NamekujiCompleteInit()
  if !exists('g:namekuji_complete_binary')
    let g:namekuji_complete_binary = 'namekuji_complete'
  endif

  if !exists('g:namekuji_complete_clang_binary')
    let g:namekuji_complete_clang_binary = 'clang'
  endif

  if !exists('g:namekuji_complete_opts')
    let g:namekuji_complete_opts = ''
  endif
endfunction

function! NamekujiComplete(findstart, base)
  if a:findstart
    let l:buffer = getline(1, '$')
    let l:tempfile = expand('%:p:h') . '/' . localtime() . expand('%:t')
    try
      call writefile(l:buffer, l:tempfile)
    catch /^Vim\%((\a\+)\)\=:E482/
      return -1
    endtry
    let l:li = line('.')
    let l:co = col('.')
    let l:cm = g:namekuji_complete_binary . ' ' . g:namekuji_complete_clang_binary . ' ' . l:tempfile . ' ' . l:li . ' ' . l:co . ' ' . g:namekuji_complete_opts
    let l:clang_output = split(system(l:cm), "\n")
    call delete(l:tempfile)
    if len(l:clang_output) == 0
      return -1
    endif
    let b:namekuji_complete_list = []
    for l:element in l:clang_output
      if strlen(l:element) > 10 && l:element[0:9] ==? '<namekuji>'
        let l:elementf = split(l:element[10:], '#')
        if l:elementf[0][0] ==? '_'
        else
          let l:word = l:elementf[0]
          let l:menu = l:elementf[1]
          call add(b:namekuji_complete_list, { 'word': l:word, 'menu': l:menu, 'dup': 1 })
        endif
      endif
    endfor
    return col('.') - 1
  else
    return b:namekuji_complete_list
  endif
endfunction

