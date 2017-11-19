inputFile = open("Expressoes.in", "r")

expressions = []

x = 1
y = 1
t = 1
z = 1

width = 0

satisfativel = 0
refutavel = 0
myText = ""

class node:
    def __init__(self , value):
        self.value = value
        self.left = None
        self.right = None

def if_then(a, b):
    if(a == 0 or b == 1): return 1
    else: return 0


def inverter(expression):
    global x, y, t, z
    if expression == 'x': x = 1 - x
    elif expression == 'y': y = 1 - y
    elif expression == 't': t = 1 - t
    elif expression == 'z': z = 1 - z
    
def operatorValid(operator):
    if operator != '.' and operator != '+' and operator != '-' and operator != '>':
      return False
    else: return True
    
def resolve(expressions):
    resolution = []
    global variables
    global satisfativel
    global refutavel 
    global myText
    
    for expression in expressions:
            
        if len(expression) == 1: #atomica
            if expression == 'x': resolution.append(x)
            elif expression == 'y': resolution.append(y)
            elif expression == 't': resolution.append(t)
            elif expression == 'z': resolution.append(z)
        
        elif expression[1] == '-':
            op_index = 1
            expr_r = expression[op_index+1:len(expression)-1]
            resolve_index = expressions.index(expr_r)
            resolution.append(1 - resolution[resolve_index])

        else:
            i = 0
            opens = 0
            closes = 0
            operator = ''
            for char in expression:
                if char == '(':
                    opens += 1
                    
                elif char == ')':
                    closes += 1
                    if opens - closes == 1:
                        operator = expression[i+1]
                        op_index = i+1
                        break
                i+=1
                
            if not operatorValid(operator):
                operator = expression[2]
                op_index = 2                
        
            expression_l = expression[1:op_index]
            expression_r = expression[op_index+1:len(expression)-1]

            l_value = resolution[expressions.index(expression_l)]
            r_value = resolution[expressions.index(expression_r)]
            
            if(operator == "+"): resolution.append(l_value or r_value)
            elif(operator == "."): resolution.append(l_value and r_value)
            elif(operator == ">"): resolution.append(if_then(l_value, r_value))
        
        if expression == line:
            if(resolution[len(resolution) - 1] == 1):
                satisfativel = 1
                
            elif(resolution[len(resolution) - 1] == 0):
                refutavel = 1
    
    
    
    
    #imprimir
    row = "|"
    h = 0
    for res in resolution:
        for k in range (0, len(expressions[h])-1):
            row+=" "
        row = row+str(res) +"|"
        h+=1
    print row
    myText+=row+"\n"
    row=""
    for j in range (0, width):
        row+='-'
    print row
    myText+=row+"\n"
  
def constructTree(expr):
    i = 0
    opens = 0
    closes = 0
    operator = ''
    op_index = 0
    
    print expr
    
    if expr not in expressions:
        expressions.append(expr)
    
    if(len(expr) == 1):
        item = node(expr)
        return item
        
    elif expr[1] == '-':
        operator = '-'
        op_index = i
        expr_r = expr[op_index+2:len(expr)-1]
        branch = node(operator)
        branch.right = constructTree(expr_r)
        return branch
        
    else:
        for char in expr:
            if char == '(':
                opens += 1
                
                
            elif char == ')':
                closes += 1
                if opens - closes == 1:
                    operator = expr[i+1]
                    op_index = i+1
                    break
            i+=1
            
        if not operatorValid(operator):
            operator = expr[2]
            op_index = 2                
                
    
        expr_l = expr[1:op_index]
        expr_r = expr[op_index+1:len(expr)-1]
        
        branch = node(operator)
        branch.left = constructTree(expr_l)
        branch.right = constructTree(expr_r)
        
        return branch
        
#main code   
i = 0
for line in inputFile:
    print("#")
    line = line.replace('\n', '')
    line = line.replace('\r', '')
    print repr(line)
    print len(line)
    print("#")
    
    if (i != 0):
        print "Tabela #" + str(i)
        myText+="Tabela #" + str(i)+"\n"
        r = constructTree(line)
        expressions = sorted(expressions, key=len)
        row = "|"
        variables = 0
        for expression in expressions:
            row = row+expression +"|"
            if len(expression) == 1: variables+=1
        print row
        myText+=row+"\n"
        
        width = len(row)
        row=""
        for j in range (0, width):
            row+='-'
        print(row)
        myText+=row+"\n"
        
        changes = pow(2, variables)
        for k in range (0, changes):
            inverter(expressions[variables - 1])
            if k%2 == 0 and variables >= 2: inverter(expressions[variables - 2])
            if k%4 == 0 and variables >= 3: inverter(expressions[variables - 3])
            if k%8 == 0 and variables == 4: inverter(expressions[variables - 4])
            resolve(expressions)
        
        row=""
        if satisfativel == 1: row+="satistafivel"
        else: row+= "insatisfativel"
        row+=" e "
        if refutavel == 0: row+="tautologia"
        else: row+= "refutavel"
        print row+"\n"
        myText+=row+"\n"
        expressions = []
    i+=1
    myText+="\n"
    
outputFile = open("Expressoes.out", "w")
outputFile.write(myText)