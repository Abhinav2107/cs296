from sys import argv
import re

try:
	script = open(argv[1])
except:
	print("Error Opening File")
	exit()
i = 0
for line in script:
	line = line.strip()
	if len(line) != 0:
		if i == 2:
			break
		i += 1
chars = []
for line in script:
	lws = len(line) - len(line.lstrip())
	line = line.strip()
	if (lws <= 35 and lws >= 25) and line.isupper() and (not re.search('[\(\)-:!,&\[\]]', line)) and (not re.search('VOICE| AND |THE END', line)) and line != "MAN" and line != "WOMAN" and line != "ALL" and len(line) > 1:
		if not (line in chars):
			chars.append(line)
script.close()
try:
	script = open(argv[1])
except:
	print("Error Opening File")
	exit()
print("Characters in the Movie and the number of times they have been referred to:")
counts = [0] * len(chars)
common_words = ['ALL', 'THE', 'OF', 'MAN', 'WOMAN', 'GUYS', 'CRAZY', 'OFFICER', 'POLICEMAN', 'GUARD', 'STRIPPER', 'A', 'U', 'D', 'PRISONER']
for line in script:
	i=0
	for name in chars:
		count = 0
		name_parts = name.split()
		line = line.upper()
		if name_parts[0] in common_words and not (name_parts[-1] in common_words):
			count = len(re.findall(name_parts[-1], line))
		elif name_parts[-1] in common_words and not (name_parts[0] in common_words):
			count = len(re.findall(name_parts[0], line))
		elif name_parts[-1] in common_words and name_parts[0] in common_words:
			count = len(re.findall(name, line))
		else:
			count = len(re.findall(name_parts[0], line)) + len(re.findall(name_parts[-1], line)) - len(re.findall(name, line))
		counts[i] += count
		i += 1
for i in range(len(chars)):
	print(chars[i], " : ", counts[i]) 