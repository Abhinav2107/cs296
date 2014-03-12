from sys import argv
try:
	script = open(argv[1])
except:
	print("Error Opening File")
	exit()
i = 0
for line in script:
	line = line.strip()
	if len(line) != 0:
		if i == 0:
			print("Title"+line)
		elif i == 2:
			print("Title"+line)
			break
		i += 1
script.close();