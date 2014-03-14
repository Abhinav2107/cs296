import sys
import re
try:
	script = open("doc/g19_prof_report.tex")
except:
	print("Error Opening File")
	exit()
	
webpage = open("doc/g19_lab09_report.html" , "w")



print("<html>" , file = webpage)

for line in script:
	for i in range(len(line)):
		if(line[i:i+8] == "\\textit{"):
			for j in range(len(line))[i:]:
				if(line[j] == "}"):
					line = line[0:i] + "<i>" + line[i+8:j] + "</i>" + line[j+1:]
					break
					
		if(line[i:i+6] == "\\href{"):
			for j in range(len(line))[i:]:
				if(line[j] == "}"):
					
					for k in range(len(line))[j+1:]:
						if(line[k] == "}"):
							line = line[0:i] + "<a href = \"" + line[i+6:j-1] + "\">" + line[j+2:k] + "</a>"
							break
					break
	
	if(line.strip() == ""):
		continue
	if(re.search(r"^\\title",line[0:6])):
		title=re.sub(r"[{}]","",re.sub(r"^\\title","",line))
		print("<head><title>" + title + "</title><style> body { font-size : 11 px;} img { text-align : center ; } caption { text-decoration : underline ; font-size : 14; text-align : center; font-family : \"verdana\"} h1 {text-align : center;} h2 { font-size : 30 px ; text-align : center;} h3 { font-size : 25 px ; text-align : center; }  </style></head>" + "<body>" , file = webpage)
		
	elif(re.search(r"^\\author",line[0:7])):
		author=re.sub(r".ac.in ",".ac.in \n",re.sub(r"[\\{}]","",re.sub(r"^\\author","",line)))
		author=author.split("\n")
		print("<h1>Authors</h1><h2>" , file = webpage)
		for aut in author:
			print(aut + "<br>" , file = webpage)
		print("</h2>" , file = webpage)
		
	elif(re.search(r"^\\section",line[0:8])):
		section=re.sub(r"[{}]","",re.sub(r"^\\section","",line))
		print("<h2>" + section + "</h2>" , file = webpage)
		
	elif(re.search(r"^\\subsection",line[0:11])):
		section=re.sub(r"[{}]","",re.sub(r"^\\subsection","",line))
		print("<h3>" + section + "</h3>" , file = webpage)
		
	elif(re.search(r"^\\caption",line[0:11])):
		section=re.sub(r"[{}]","",re.sub(r"^\\caption","",line))
		print("<caption><h3>" + section + "</h3></caption>" , file = webpage)

	elif(re.search(r"^\\includegraphics",line[0:20])):
		section=re.sub(r"[{}\n]","",re.sub(r"\[.*\]","",re.sub(r"^\\includegraphics","",line)))
		print("<img src = " + section + ">" , file = webpage)
		
	elif(re.search(r"^\\\\",line[0:4])):
		print("<br>" + re.sub(r"^\\\\","",line) , file = webpage)
	
	elif(not line[0] == "\\" and not line[0] == "%"):
		print("<p>" + line + "</p>" , file = webpage)
		
print("</body></html>" , file = webpage)
