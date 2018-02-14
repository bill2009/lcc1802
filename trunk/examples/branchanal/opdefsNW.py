opsizes={
	'or':1,'ori':2,'xor':1,'xri':2,
	'and':1,'ani':2,
	'out':1,'inp':1,
	'sep':1,'ldi':2,'plo':1,'phi':1,'glo':1,'ghi':1,
	'sm':1,'smb':1,'smbi':2,
	'sd':1,'sdb':1,'sdi':2,'sdbi':2,
	'skp':1,'lskp':1,'lsnf':1,'lsdf':1,'lsz':1,'lsnz':1,
	'sex':1,'lda':1,'ret':1,'nop':1,'dec':1,'stxd':1,
	'shr':1,'shrc':1,'shl':1,'shlc':1,
	'str':1,'ldn':1,
	'add':1,'adi':2,'adc':1,'adci':2,'str':1,'smi':2,'inc':1,
	'dis':1,'sav':1,
	'bz':2,'bnz':2,'br':2,'b3':2,'bn3':2,'bnf':2,'bdf':2,
        'cpy2': 4, 'cpy1': 2, 'cpy4': 8,
        'zext': 3,'sext': 9,'zext4':4,'sext4':11,
        'negi2':9,'negi4':32,
        'alu2':12,'alu2i':8,
        'alu4':22,'alu4i':16,
        'ldad': 6,
        'lda2': -8,
        'shl2i': -6,'shri2i': -8,'shru2i': -6,
        'shl4':12,'shri4':14,
        'shl4i': -12,'shri4i': -14,
        'st1':-10,'st2':-13,'str1':2,
        'ld2':-12,'ld1':-10,
        'ldn1':2,'ldn2':4,
        'jzu2':8,
        'jnzu2':8,'jnzu1':4,'sjnzu2':6,'sjnzu1':3, #the sj codes are short branch variants
        'jeqi2':18,'jcu2':13,'jneu2': 18,
        'jci2': 20,'jci4': 28,'jcu4': 28,
        'jcu2i':9,
        'jni2i':17,'jnu2i':9,
        'jneu2i': 12,
        'jneu4':39,
        'jequ2i':12,'jci2i':17,
        'jcf4':69,
        'ld2z':4,
        'ldi4':12,'st4':-19,'ld4':-16, 
        'incm': -1,'decm': -1,
        'popr': 5, 'pushr': 4,
        'popf': 5, 'popl': 4,
        'lbr': 3,'lbnz':3,'lbz':3,'lbnf':3,'lbdf':3,
        'equ': 0, 'db': -1,  'dw':2,'dd':4,
        'align':-1, #force alignment
        'cretn': 1,'ccall': 3,
         'seq': 1,   'req': 1,
        'listing': 0,   'include': -3,
        'release': -1,'reserve': -1,
        'jumpv':10,
        'relaxed':0, 'macexp': 0,
        'ldx':1,'ldxa':1,'irx':1,
        'org':-1}
        
jumpdefs={
	'lbr': 3,'lbnz':3,'lbz':3,'lbdf':3,'lbnf':3
	}
jumpreps={
	'lbr': 'br','lbnz':'bnz','lbz':'bz','lbdf':'bdf','lbnf':'bnf'	
	}

