" Vim syn file
" Language: aurum
" Maintainer: Ramon Asuncion
" Latest Revision: 17 June 2023

" Quit when a syn file was already loaded.
if exists("b:current_syntax")
    finish
endif

set iskeyword=a-z,A-Z,-,*,_,!,@
syn keyword aurumTodos TODO XXX FIXME NOTE

" Control structures
syn keyword aurumControlKeywords if then else while end
highlight aurumControlKeywords ctermfg=214

" Constants and built-in values
syn keyword aurumConstants final true false

" Stack manipulation
syn keyword aurumStackKeywords dup swap roll over peek pop rot drop

" Memory operations
syn keyword aurumMemoryKeywords memory write read store fetch
highlight aurumMemoryKeywords ctermfg=DarkMagenta guifg=DarkSlateBlue

" Misc keywords
syn keyword aurumMiscKeywords break not systemcall ascii

" Comments
syn region aurumCommentLine start="//" end="$"   contains=aurumTodos
syn region aurumCommentBlock start="/\*" end="\*/" contains=aurumTodos

" String literals
syn region aurumString start=/\v"/ skip=/\v\\./ end=/\v"/ contains=aurumEscapes
highlight aurumString ctermfg=blue

" Char literals
syn region aurumChar start=/\v'/ skip=/\v\\./ end=/\v'/ contains=aurumEscapes

" Escape literals \n, \r, ....
syn match aurumEscapes display contained "\\[nr\"']"

" Number literals
syn match aurumNumber /\d\+/
highlight aurumNumber ctermfg=red guifg=red

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
