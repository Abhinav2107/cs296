from sys import argv
import re
def mean(list):
	sum = 0
	for item in list:
		sum += item
	return sum/len(list)
	
def stddev(list):
	m = mean(list)
	sum = 0
	for item in list:
		sum = (item-m)**2
	return (sum/len(list))**0.5
from collections import OrderedDict
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
script = open(argv[1])

script = open(argv[1])
male_after = {'he', 'his'}
female_after = {'she', 'her'}
curr = ""
count = {}
for name in chars:
	count[name] = 0
counts = [0] * len(chars)
common_words = ['ALL', 'THE', 'OF', 'MAN', 'WOMAN', 'GUYS', 'CRAZY', 'OFFICER', 'POLICEMAN', 'GUARD', 'STRIPPER', 'A', 'D', 'U', 'PRISONER']

for line_orig in script:
	i=0
	line = line_orig.upper()
	for name in chars:
		counted = 0
		name_parts = name.split()
		if name_parts[0] in common_words and not (name_parts[-1] in common_words):
			counted = len(re.findall(name_parts[-1], line))
		elif name_parts[-1] in common_words and not (name_parts[0] in common_words):
			counted = len(re.findall(name_parts[0], line))
		elif name_parts[-1] in common_words and name_parts[0] in common_words:
			counted = len(re.findall(name, line))
		else:
			counted = len(re.findall(name_parts[0], line)) + len(re.findall(name_parts[-1], line)) - len(re.findall(name, line))
		counts[i] += counted
		i += 1	
	line = line_orig.strip()
	words = line.split()
	for word in words:
		for name in chars:
			if word.upper() in name.split() and not (word.upper() in common_words):
				curr = name
				break
		if word.lower() in male_after and curr != "":
			count[curr] += 1
		elif word.lower() in female_after and curr != "":
			count[curr] -= 1
male_names = ['BOY', 'MAN', 'HUSBAND', 'BOYFRIEND', 'GUY', 'GUYS', 'MEN', 'FATHER', 'SON', 'MALE', 'DAD', 'BROTHER', 'MR.', 'KING', 'PRINCE']
female_names = ['GIRL', 'WOMAN', 'WIFE', 'GIRLFRIEND', 'STRIPPER', 'WOMEN', 'DAUGHTER', 'MOTHER', 'FEMALE', 'MOM', 'SISTER', 'MRS.', 'MISS', 'CHICK', 'QUEEN', 'PRINCESS', 'MUM']
for name in chars:
	for word in name.split():
		if word in male_names:
			count[name] += 100
		elif word in female_names:
			count[name] -= 100
for name in count:
	if count[name] > 0:
		print(name, " : Male")
	elif count[name] < 0:
		print(name, " : Female")
	else:
		print(name, " : Cannot Determine")
	
dictionary = {}
for i in range(len(chars)):
	dictionary[chars[i]] = counts[i]
freq = list(OrderedDict(sorted(dictionary.items(), key=lambda t: t[1])).items())
top3 = [item[1] for item in freq[-3:]]
if(stddev(top3)/mean(top3) < 0.1):
	print("No heroes or villains in the movie")
else:
	print("Hero: ", freq[-1][0])
	print("Villain: ", freq[-2][0])