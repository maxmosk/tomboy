%require  "3.2"
%language "c++"
%skeleton "lalr1.cc"

%defines
%define api.value.type variant


%start program

%%
program:    %empty
;
%%