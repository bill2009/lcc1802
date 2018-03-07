#updated Feb 2018 for NW compiler, clean stack
#NW3 trying to accommodate temporary labels.
#NW4 cleanup
import sys
sys.path.append('/lcc42/examples/branchanal/')
from opdefsNW import *
from opfuncsNW import *
asminfile = open(sys.argv[1],'r')
asmdata = asminfile.read().expandtabs()
asmlines = asmdata.split('\n')
print len(asmlines), 'lines read'
jumpcount=0
repcount=0
def ireplace(old, new, text):
    """ 
    Replace case insensitive
    Raises ValueError if string not found
    """
    index_l = text.lower().index(old.lower())
    return text[:index_l] + new + text[index_l + len(old):] 
def jumper(prefix,i,op,tokens,pc,labelcount):
	global jumpcount,repcount
	if prefix=="":
		jumpcount+=1
	target=tokens[1]
	print	prefix,i,"jumping with ",op," ",target," "
	if target in labeldefs:
		#print 'label is defined as %4X - ' % (labeldefs[target]),
		if ((pc+1)//256 == labeldefs[target]//256):
			print "converting to short branch",prefix,i,op,jumpreps[op],asmlines[i],"B01"
			if prefix=="":
				asmlines[i]=ireplace(op,jumpreps[op],asmlines[i])+";WJRB01" #asmlines[i].replace(op,jumpreps[op])+" ;WJRBO1"
				repcount+=1
			op=jumpreps[op]
		else:
			print "**COULD NOT convert to short branch"
	elif target=='lcc1802init':
		pass #print ' NOT recursing for %s' % (target)
	else:
		#print "(recursing to scanner)"
		recurstarget=scanner(prefix+">",i+1,pc+jumpdefs[op],labelcount,target)
		try:
			if ((pc+1)//256 == labeldefs[target]//256):
				print "converting to short branch",prefix,i,op,jumpreps[op],asmlines[i],"B02"
				if prefix=="":
					asmlines[i]=ireplace(op,jumpreps[op],asmlines[i])+";WJRB02" #asmlines[i].replace(op,jumpreps[op])+" ;WJRBO2"
					repcount+=1
				op=jumpreps[op]
			else:
				pass #print "**COULD NOT convert to short branch"
		except:
			print "**label oops ",target
			print labeldefs
			x = raw_input("Press Enter to continue")
	print "%d %4x %s" % (i,pc,asmlines[i])
	return opsizes[op]

labeldefs={}
def stolabel(label,address):
    labeldefs[label]=address;
    print "*** LSTO ",label,address

def scanner(prefix,beginline,beginpc,labelcount,target):
    if not prefix:
    	print prefix,"scanner begins at line %d with PC %d, labelcount %d and target %s\n" %(beginline,beginpc,labelcount,target)
    progsize=beginpc;
    i=beginline
    ln=labelcount #used for distinguishing temporary symbols
    for line in asmlines[beginline:]:
    	if (line and (beginline==0)):
		print '%3d %4X %s' % (i,progsize,asmlines[i])
	aline=line.split(";")[0] #get rid of any comment portion
	#print "!aline=",aline
        tokens=aline.split();
        if tokens:
		if not aline.startswith(' '): #if it has to be a label
			if not tokens[0].endswith(':'): #make sure it ends with a colon
				tokens[0]=tokens[0]+':'
		#print "!tokens=",tokens
		if tokens[0].endswith(':'):
			if tokens[0]=='+':
				tokens[0]="$$plus"+tokens[0]
			if tokens[0].startswith("$$"): #check for temporary label
				tokens[0]=tokens[0][2:-1]+"$$"+str(ln)+':'
			else:
				ln+=1
			stolabel(tokens[0][:-1],progsize);
			print '%4X %s' %(progsize,tokens[0])
			if tokens[0][:-1]==target:
				return progsize
			else:
				tokens=tokens[1:]
		if tokens:
			if tokens[0].lower() in jumpdefs:
				if tokens[1]=='+':
					tokens[1]="$$plus"+tokens[1]
				if tokens[1].startswith("$$"): #for temporary labels
					tokens[1]=tokens[1][2:]+"$$"+str(ln) #add some distinction
				print i,prefix,tokens[0].lower()
				progsize+=jumper(prefix,i,tokens[0].lower(),tokens,progsize,ln)
			else:
				progsize+=process(tokens,progsize)
	i+=1;
                
    return -1

endofCcode=scanner("",0,0,0,'lcc1802init')
#print "End of C Code is at %4X" %(endofCcode)

asmoutfile=open(sys.argv[1].split('.')[0]+".basm",'w'); asmoutfile.truncate()
i=0
for line in asmlines:
	asmoutfile.write(line+'\n')
	i+=1
print i, 'lines written'
print "%d long jumps found, %d shortened" % (jumpcount,repcount)
print len(labeldefs),labeldefs
