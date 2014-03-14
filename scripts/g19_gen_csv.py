import subprocess
import random
n = 500 #Number of Iterations
r = 50 #Number of reruns
csv = open("data/g19_lab09data.csv", "w")
for i in range(n):
	for j in range(r):
		output = subprocess.Popen(['./mybins/cs296_19_exe', str(i+1)], stdout=subprocess.PIPE).communicate()[0].decode(encoding='UTF-8').split()
		towrite = str(i+1)+','+str(j+1)+','+output[9]+','+output[16]+','+output[24]+','+output[32]+','+output[38]
		print(towrite, file=csv)

csv.close()

csv = open("data/g19_lab09data.csv", "r")
csv_random = open("data/g19_lab09data_random.csv", "w")
i = 0
iterations = 0
for line in csv:
	if i % r == 0:
		iterations += 1
		to_pick = random.sample(range(r), 15)
		#print(iterations, ":", to_pick)
	if (i - (iterations - 1) * r) in to_pick:
		print(line.strip(), file=csv_random)
	i += 1
csv.close()
csv_random.close()
