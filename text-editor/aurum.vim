" Language: aurum
" Maintainer: Ramon Asuncion
" Latest Revision: 18 June 2023

scriptencoding utf-8

" Quit when a syn file was already loaded.
if v:version > 580
  hi clear
  if exists('syntax_on')
    syntax reset
  endif
endif

let g:colors_name='aurum'

if !has('gui_running') && &t_Co != 256
  finish
endif

set background=dark

set iskeyword=a-z,A-Z,-,*,_,!,@
syn keyword aurumTodos TODO XXX FIXME NOTE

" Background 
highlight Normal ctermbg=235 guibg=#1c1b19
highlight NonText ctermbg=235 guibg=#1c1b19

" Control structures
syn keyword aurumControlKeywords if then else while end
highlight aurumControlKeywords ctermfg=196 guifg=#ff0000

" Constants and built-in values
syn keyword aurumConstants final true false
highlight aurumConstants ctermfg=208 guifg=#ff8700

" Stack manipulation
syn keyword aurumStackKeywords dup swap roll over peek pop rot drop dump
highlight aurumStackKeywords ctermfg=118 guifg=#87ff00

" Memory operations
syn keyword aurumMemoryKeywords memory write read store fetch
highlight aurumMemoryKeywords ctermfg=81 guifg=#00afff

" Misc keywords
syn keyword aurumMiscKeywords break not systemcall ascii
highlight aurumMiscKeywords ctermfg=220 guifg=#ffd700

" Comments
syn region aurumCommentLine start="//" end="$"   contains=aurumTodos
syn region aurumCommentBlock start="/\*" end="\*/" contains=aurumTodos
highlight aurumCommentLine ctermfg=8 guifg=#808080
highlight aurumCommentBlock ctermfg=8 guifg=#808080

" String literals
syn region aurumString start=/\v"/ skip=/\v\\./ end=/\v"/ contains=aurumEscapes
highlight aurumString ctermfg=149 guifg=#98BC37

" Char literals
syn region aurumChar start=/\v'/ skip=/\v\\./ end=/\v'/ contains=aurumEscapes
highlight aurumChar ctermfg=39 guifg=#00afff

" Escape literals \n, \r, ....
syn match aurumEscapes display contained "\\[nr\"']"
highlight aurumEscapes ctermfg=214 guifg=#ffaf00

" Number literals
syn match aurumNumber /\d\+/
highlight aurumNumber ctermfg=38 guifg=#00d7af

" Set highlights
highlight default link aurumTodos Todo
highlight default link aurumCommentLine Comment
highlight default link aurumCommentBlock Comment
highlight default link aurumChar Character
highlight default link aurumEscapes SpecialChar
highlight default link aurumControlKeywords Keyword
highlight default link aurumConstants Constant
highlight default link aurumStackKeywords Statement
highlight default link aurumMemoryKeywords Function
highlight default link aurumMiscKeywords Keyword

let b:current_syn = "aurum"
