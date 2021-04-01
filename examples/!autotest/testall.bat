@echo these programs are taken from 'The C Puzzle Book"
@echo testing with %1 and %2
call unittest 0leveltest
call unittest 1puzzlesc
call unittest 0leveltest
call unittest  1puzzlesc
call unittest  2puzztypesp
call unittest  2puzztypesflt
call unittest  2puzztypesfltmore
call unittest 3puzzinclsp
call unittest 4puzzctlflo
call unittest 5puzzstgc
call unittest 6puzzstgc2
call unittest 7puzzptrs
call unittest 8puzzptrs2
call unittest 9puzzstruc1
call unittest 91puzzstruc3
@echo And we're done 
pause
