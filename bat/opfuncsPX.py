from opdefsPX import *
def opsizer(opsize,tokens,currentpc):
    op=tokens[0].lower()
    operands=" ".join(tokens[1:]).split(",") #half-baked wjr
    #print "opsizing op=",op," operands=",operands
    if op=='db':
        #print "=db=>",operands
        if len(tokens)>=3 and tokens[2]=='dup':
              return int(tokens[1]) #tokens[1] because dup is separated from the count by a space
        else:
              #print "db length is ",len(operands)
              return len(operands)
    elif op=='align':
        boundary=int(operands[0])
        if (currentpc%boundary)==0:
            return 0
        else:
            return boundary-(currentpc%boundary)
    elif op=='pagefit':
        needed=int(operands[0])
        left=255-currentpc%256 #room left on page #wjt 21-4-2
        #print "pagefit ",needed," with ",left
        if (needed<=left):
            return 0
        else:
            return left
    elif op=='org':
        print "I Hope this ORG is not harmful! ",operands[0]
        return 0
    else:
        print '**************opsizer oops',tokens,opsize
        x = raw_input("Press Enter to continue")
       
            
        
    return 999
    
def process(tokens,currentpc):
    global opdefs
    try:
        #print 'processing ',tokens,"**",tokens[0],"**",opsizes[tokens[0].lower()],currentpc
        opsize=opsizes[tokens[0].lower()]
    except:
        print 'process oops', tokens
        opsize=0
        print tokens[0],opsizes
        x = raw_input("Press Enter to continue")

    if opsize>=0:
        thisopsize=opsize
    else:
        thisopsize=opsizer(opsize,tokens,currentpc)
    #print tokens[0],opsize,thisopsize
    return thisopsize
