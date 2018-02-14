#updated Feb 2018 for NW compiler, clean stack
#NW3 trying to accommodate temporary labels.
import sys
from opdefsNW import *
from opfuncsNW import *
#print opsizes, jumpdefs
#print 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)
asminfile = open(sys.argv[1],'r')
asmdata = asminfile.read().expandtabs()
asmlines = asmdata.split('\n')
print len(asmlines), 'lines read'

def jumper(prefix,i,op,tokens,pc,labelcount):
	jumpcounts[op]+=1
	target=tokens[1]
	print	"jumping with ",op," ",target," "
	if target in labeldefs:
		print 'label is defined as %4X - ' % (labeldefs[target]),
		if (pc//256 == labeldefs[target]//256):
			print "**COULD convert to short branch"
		else:
			print "**COULD NOT convert to short branch"
	elif target=='lcc1802init':
		print ' NOT recursing for %s' % (target)
	else:
		print "(recursing to scanner)"
		recurstarget=scanner(prefix+">",i+1,pc+jumpdefs[op],labelcount,target)
		if (pc//256 == labeldefs[target]//256):
			print "**COULD convert to short branch"
		else:
			print "**COULD NOT convert to short branch"
	return jumpdefs[op]

labeldefs={}
def stolabel(label,address):
    labeldefs[label]=address;
    #print label,address

def scanner(prefix,beginline,beginpc,labelcount,target):
    print prefix,"scanner begins at line %d with PC %d, labelcount %d and target %s\n" %(beginline,beginpc,labelcount,target)
    progsize=beginpc;
    i=beginline
    ln=labelcount #used for distinguishing temporary symbols
    for line in asmlines[beginline:]:
    	#if line:
		#print '%3d %4X %s' % (i,progsize,asmlines[i])
	aline=line.split(";")[0] #get rid of any comment portion
	#print "!aline=",aline
        tokens=aline.split();
        if tokens:
		if not aline.startswith(' '): #if it has to be a label
			if not tokens[0].endswith(':'): #make sure it ends with a colon
				tokens[0]=tokens[0]+':'
		#print "!tokens=",tokens
		if tokens[0].endswith(':'):
			if tokens[0].startswith("$$"): #check for temporary label
				tokens[0]=tokens[0][2:-1]+"$$"+str(ln)+':'
			else:
				ln+=1
			stolabel(tokens[0][:-1],progsize);
			#print '%4X %s' %(progsize,tokens[0])
			if tokens[0][:-1]==target:
				return progsize
			else:
				tokens=tokens[1:]
		if tokens:
			if tokens[0].lower() in jumpdefs:
				#print "Oh Boy"
				if tokens[1].startswith("$$"): #for temporary labels
					tokens[1]=tokens[1][2:]+"$$"+str(ln) #add some distinction
				progsize+=jumper(prefix,i,tokens[0].lower(),tokens,progsize,ln)
			else:
				progsize+=process(tokens,progsize)
	i+=1;
                
    return -1

endofCcode=scanner("",0,0,0,'lcc1802init')
print "End of C Code is at %4X" %(endofCcode)

asmoutfile=open(sys.argv[1]+"O.asm",'w'); asmoutfile.truncate()
i=0
for line in asmlines:
	asmoutfile.write(line+'\n')
	i+=1
print i, 'lines written'
print jumpcounts
print len(labeldefs),labeldefs
