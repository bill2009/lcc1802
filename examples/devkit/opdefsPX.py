#20-06-21 added a few ops: b1/bn1,rshr,MACEXP_DFT,save,restore, 'function'
opsizes={
    'xbr': 5,'xbnz':5,'xbz':5,'xbnf':5,'xbdf':5,
    'or':1,'ori':2,'xor':1,'xri':2,
    'and':1,'ani':2,
    'out':1,'inp':1,
    'sep':1,'ldi':2,'plo':1,'phi':1,'glo':1,'ghi':1,
    'sm':1,'smb':1,'smbi':2,
    'sd':1,'sdb':1,'sdi':2,'sdbi':2,
    'skp':1,'lskp':1,'lsnf':1,'lsdf':1,'lsz':1,'lsnz':1,
    'sex':1,'lda':1,'ret':1,'nop':1,'dec':1,'stxd':1,
    'shr':1,'shrc':1,'shl':1,'shlc':1,
    'rshr':1,
    'str':1,'ldn':1,
    'add':1,'adi':2,'adc':1,'adci':2,'str':1,'smi':2,'inc':1,
    'dis':1,'sav':1,
    'bz':2,'bnz':2,'br':2,'b3':2,'bn3':2,'bnf':2,'bdf':2,
    'b1':2,'bn1':2,'b2':2,'bn2':2,'b3':2,'bn3':2,'b4':2,'bn4':2,
    'equ': 0, 'db': -1,  'dw':2,'dd':4,
    'align':-1, #force alignment
    'pagefit':-1, #ensure bytes left on page
    'cretn': 1,'ccall': 3,
     'seq': 1,   'req': 1,
    'listing': 0,   'include': -3,
    'macexp_dft': 0,'function':0,'save':0,'restore':0,'set':0,
    'release': -1,'reserve': -1,
    'relaxed':0, 'macexp': 0,
    'ldx':1,'ldxa':1,'irx':1,
    'org':-1}
        
skipdefs={
    'lsnf': 'bnf $+4','lsdf': 'bdf $+4'
    }
ojumpdefs={
    'lbr': 'xbr','lbnz':'xbnz','lbz':'xbz','lbdf':'xbdf','lbnf':'xbnf'
    }
jumpdefs={
    'xbr': 5,'xbnz':5,'xbz':5,'xbdf':5,'xbnf':5
    }
jumpreps={
    'xbr': 'br','xbnz':'bnz','xbz':'bz','xbdf':'bdf','xbnf':'bnf'   
    }

