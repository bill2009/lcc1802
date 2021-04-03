#18-02-10 linear two pass scan - no recursion
#18-02-13 trying to include aligns with jumps.
#21-04-02 fixing pagefit process to be more like aligns
import sys
from opdefsPX import *
from opfuncsPX import *
asminfile = open(sys.argv[1],'r')
asmdata = asminfile.read().expandtabs()
asmlines = asmdata.split('\n')
print len(asmlines), 'lines read'
jumpcount=0;aligncount=0;pagefitcount=0
skipcount=0; skipneeded=0
repcount=0;repacount=0;reppfcount=0
progsize=0
i=0
tln=0;pln=0;mln=0 #temp label numbers for $$,+ -
labeldefs={}
labelrefs=[]
def ireplace(old, new, text): #    Replace case insensitive
    index_l = text.lower().index(old.lower())
    return text[:index_l] + new + text[index_l + len(old):] 
def gotlabel(token):
    global tln,pln,mln
    label=token.split(':')[0]
    if label.startswith("$$"):
        label+=str(tln)
    elif label.startswith("+"): #check for temporary label
        label+=str(pln)
        pln+=1
    elif label.startswith("-"): #check for temporary label
        mln+=1
        label+=str(mln)
    else:
        tln+=1
    labeldefs[label]=progsize

def fmtlabel(label):
    global tln,pln,mln
    if label.startswith("$$"):
        label+=str(tln)
    elif label.startswith("+"): #check for temporary label
        label+=str(pln)
    elif label.startswith("-"): #check for temporary label
        label+=str(mln)
    return label

for i,line in enumerate(asmlines):
    #print line
    if len(line.split())==0:
        continue
    brop=line.split()[0].lower() 
    if brop in ojumpdefs:
        asmlines[i]=ireplace(brop,ojumpdefs[brop],asmlines[i])
        #print "**********************",asmlines[i]
    elif brop in skipdefs:
        asmlines[i]=ireplace(brop,skipdefs[brop],asmlines[i])
        asmlines.insert(i," pagefit 5")
    line=asmlines[i]
    aline=line.split(";")[0] #get rid of any comment portion
    tokens=aline.split();
    if tokens:
        #print "%d %X %s" % (i+1,progsize,aline)
        if (not aline.startswith(' ')) or tokens[0].endswith(':'): #if it has a label
            gotlabel(tokens[0])
            tokens=tokens[1:]           #get rid of it
        if tokens:
            if tokens[0].lower() in jumpdefs:
                jumpcount+=1
                labelrefs.append([i,progsize,tokens[0].lower(),fmtlabel(tokens[1].split(",")[0])])
            elif tokens[0].lower()=="align":
                aligncount+=1
                labelrefs.append([i,progsize,tokens[0].lower(),tokens[1].split(",")[0],process(tokens,progsize)])
                #print "align ",i,progsize,tokens[0].lower(),tokens[1].split(",")[0],process(tokens,progsize)
            elif tokens[0].lower()=="pagefit":
                pagefitcount+=1
                labelrefs.append([i,progsize,tokens[0].lower(),tokens[1].split(",")[0],process(tokens,progsize)])
                #print "pagefit ",i,progsize,tokens[0].lower(),tokens[1].split(",")[0],process(tokens,progsize)
            #elif tokens[0].lower()=='pagefit':
            #    skipcount+=1
            #    needed=process(tokens,progsize)
            #    skipneeded+=needed
            #    if (needed>0):
            #        asmlines[i]='    align 256,0xE2'
            #    else:
            #        asmlines[i]='    align 1,0xE2'
            progsize+=process(tokens,progsize)

    i+=1;
print "pass 1 completed. ",len(labeldefs)," labels found. ",len(labelrefs)," jumps/aligns/pagefits found."
#print labelrefs,labeldefs
adjb=0;adjl=0
repacount=0
i=0
for ref in labelrefs: # line index, location, branch op(or align), label referenced(or alignment)
    line=ref[0]; loc=ref[1]; brop=ref[2]; label=ref[3]
    adjamt=0
    if brop=="align":
        repacount+=1
        newsize=process([brop,label],loc)
        adjamt=newsize-ref[4]
        labelrefs[i][4]=newsize
        #print "*A*%4d %4x %s" %(line+1,loc,brop), label, ref[4],newsize, adjamt
    elif brop=="pagefit":
        reppfcount+=1
        newsize=process([brop,label],loc)
        adjamt=newsize-ref[4]
        labelrefs[i][4]=newsize
        #print "*PF*%4d %4x %s" %(line+1,loc,brop), label, ref[4],newsize, adjamt
    else:
        #print line,asmlines[line],loc,labeldefs[label]
        if (loc+1)//256==labeldefs[label]//256:
            asmlines[line]=ireplace(brop,jumpreps[brop],asmlines[line])
            #print "%4d %4x %s %4x" %(line+1,loc,asmlines[line],labeldefs[label])
            repcount+=1
            adjamt=-3
    if not adjamt==0:
        for adjref in labelrefs: # line index, location, branch op, label referenced
            if adjref[0]>line: # for jumps further down the way
                if adjref[2]=="align":
                    pass #print "A%d %x "%(adjref[0],adjref[1]),adjref[2],adjref[3],adjref[4]
                adjref[1]+=adjamt #get adjusted
                adjb+=1
        for k, labloc in labeldefs.iteritems():
            if labloc>loc:
                labeldefs[k] +=adjamt
                adjl+=1
    i+=1
#print labeldefs
#for k,labloc in sorted(labeldefs.iteritems(), key=lambda x: x[1]):
#    print "%04x %s" % (labloc,k)
#for key, value in sorted(labeldefs.iteritems(), key=lambda x: x[1]): 
#    print("{} : {}".format(key, value))
print repcount,"+",repacount,"+",reppfcount," fixup cycles ",adjb," branch fixups ",adjl," label fixups"
asmoutfile=open(sys.argv[1].split('.')[0]+".basm",'w'); asmoutfile.truncate()
asmoutfile.write("    include pxbrmacs.inc\n")
i=1
for line in asmlines:
    asmoutfile.write(line+'\n')
    i+=1
asmoutfile.write("    include pxbrcode.inc\n\n")
i+=2
print i, 'lines written'
print "%d long jumps found, %d shortened" % (jumpcount,repcount)
print "%d long skips found, %d bytes added" % (skipcount,skipneeded)
