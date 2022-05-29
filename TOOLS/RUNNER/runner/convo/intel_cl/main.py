with open("mat_mul.cl", "r") as f:
    source = f.read()
    
break_lines = source.split("\n")

new_source = ""
for line in break_lines:
    new_source += "\""+line+"\\n\"\n"


f.close()

f = open("mat_mul.cl", "a")
f.flush()
f.write(new_source)
f.close()