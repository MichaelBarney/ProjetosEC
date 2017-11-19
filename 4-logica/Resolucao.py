inputFile = open("Expressoes.in", "r")

width = 0

satisfativel = 0
refutavel = 0
myText = ""

    
def operatorValid(operator):
    if operator != '.' and operator != '+' and operator != '-' and operator != '>':
      return False
    else: return True

def getExpressions(expr):
    i = 0
    opens = 0
    closes = 0
    negated = 0
    has_true = 0
    plus = 0
    start = 0
    expressions = []    

    for char in expr:
        #print char
        if char == '(':
            start = i
            opens += 1
            if opens > 1:
                return "nao esta na FNC"
        elif char == ')':
            expressions.append(expr[start+1:i])
            opens = 0
            has_true = 0
        
        elif char == '+':
            plus += 1
            
        elif char == '.':
            if opens >= 1:
                return "nao esta na FNC"
                
        elif char == '>':
            return "nao esta na FNC"
            
        elif char == '-':
            negated = 1
        
        elif char != " ":
            if negated == 0:
                if(has_true == 0):
                    has_true = 1
                else:
                    return "nem todas as clausulas sao de horn"
            negated = 0
        i+=1
        
    #print expressions
    
    for expression in expressions:
        if "+" not in expression:
            if '-' in expression:
                if expression.replace("-", "") in expressions:
                    return "insatisfativel"
            else:
                if '-'+expression in expressions:
                    return "insatisfativel"
        
        else:
            subs = expression.split('+')
            newsubs = []
            for sub in subs:
                inverse = '-'+sub
                if '-' in sub: inverse = inverse.replace('-', '')
                #print "I: " + inverse
                if inverse not in subs and inverse not in expressions:
                    newsubs.append(sub)
            if len(newsubs) <= 0: return "insatisfativel"
            #print subs
            #print "Result: " + "+".join(newsubs)
            index = expressions.index(expression)
            expressions[index] = "+".join(newsubs)
            #print expressions
            
    return "satistativel"
        
#main code   
i = 0
for line in inputFile:
    line = line.replace("\n", "")
    line = line.replace("\r", "")
    if (i != 0):
       print "caso #" + str(i) + ": " + getExpressions(line)
       myText += "caso #" + str(i) + ": " + getExpressions(line) + "\n"
    i+=1
    
outputFile = open("Expressoes.out", "w")
outputFile.write(myText)