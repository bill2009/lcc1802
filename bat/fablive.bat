
@echo fabrizio's build
set fn=reduced_full_lcc1802_combo


c:\lcc42\bin\lcc.exe -Icross_lib -Icross_lib/sleep -Icross_lib/display -Icross_lib/display/graphics_mode -Icross_lib/display/alt_print -Icross_lib/include ^
-Icross_lib/sound -Icross_lib/sound/cc65/atmos -Icross_lib/sound/cc65/c264 -Icross_lib/sound/cc65/pokey -Icross_lib/sound/cc65/sid -Icross_lib/sound/cc65/vic20 ^
-Icross_lib/sound/z88dk/bit_bang -Icross_lib/sound/z88dk/psg -Icross_lib/sound/lcc1802/comx -Icross_lib/sound/generic -Icross_lib/text -Icross_lib/input -Icross_lib/rand ^
-Icross_lib/display/redefine_characters -Icross_lib/display/tiles -Igames/shoot/generated_assets -Igames/shoot/ -Igames/shoot ^
-target=xr18CX  "-Wf-volatile" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" "-Wf-mulcall" -DNUM_OF_TILES=19  "-Wa-D LCCCX=1" "-Wa-D CODELOC=0x4401"   ^
-D__COMX__ -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15 ^
-DFULL_GAME -DFORCE_NARROW -DLCC1802_GRAPHICS -DREDEFINED_CHARS -DGHOSTS_NUMBER=6 -DBULLETS_NUMBER=3 ^
-DNO_COLOR -DSIMPLE_STRATEGY ^
-DNO_BLINKING -DTINY_TEXT ^
-DLCC1802_JOYSTICK ^
-DLESS_TEXT -DNO_HINTS -DNO_TITLE -DNO_SET_SCREEN_COLORS -DNO_MESSAGE -DNO_INITIAL_SCREEN ^
-DANIMATE_PLAYER -DNINTH_CHAR_LINE ^
-D__LCC1802__ -DFORCE_NO_CONIO_LIB -DNO_SET_SCREEN_COLORS -DLESS_TEXT -DALT_RAND -DALT_PRINT -DINITIAL_SLOWDOWN=21150 -DNO_SLEEP -DNO_RANDOM_LEVEL ^
-DFORCE_GHOSTS_NUMBER=7 ^
-S games\%fn%.c
pause


c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -v -I %fn%.asm -O %fn%.oasm
c:\apps\pypy\pypy3 c:\lcc42\examples\liveness\liveness9.py %fn%
c:\lcc42\bin\copt c:\lcc42\include\liveness.opt -v -I %fn%.lasm -O %fn%.olasm
C:\lcc42\bin\asw -cpu 1802 -quiet -i C:\lcc42\include -L  -o %fn%.p -D LCCNOLONG -D LCCCX=1 -D CODELOC=0x4401 %fn%.olasm
C:\lcc42\bin\p2hex -r $-$ %fn%.p %fn%.hex

