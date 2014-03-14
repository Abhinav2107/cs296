import numpy as np
import pylab as pl

csv = open("data/g19_lab09data.csv")
n = 500 #Number of Iterations
r = 50 #Numer of Reruns
roll_no = 86
i = 1
step_sum = 0.0
collision_sum = 0.0
velocity_sum = 0.0
position_sum = 0.0
loop_sum = 0.0
step_times = []
collision_times = []
velocity_times = []
position_times = []
loop_times = []
err_low_step = []
err_high_step = []
max_step = 0.0
min_step = float("inf")
roll_no_step_times = []

for line in csv:
	values = line.split(",")
	if i >= (roll_no - 1) * r and i < roll_no * r:
		roll_no_step_times.append(float(values[2]))
	step_sum += float(values[2])
	if float(values[2]) > max_step:
		max_step = float(values[2])
	if float(values[2]) < min_step:
		min_step = float(values[2])
	collision_sum += float(values[3])
	velocity_sum += float(values[4])
	position_sum += float(values[5])
	loop_sum += float(values[6])
	if i % r == 0:
		step_sum = step_sum / r
		step_times.append(step_sum)			
		err_low_step.append(step_sum - min_step)
		err_high_step.append(max_step - step_sum)
		max_step = 0.0
		min_step = float("inf")
		step_sum = 0.0
		collision_sum = collision_sum / r
		collision_times.append(collision_sum)
		collision_sum = 0.0
		velocity_sum = velocity_sum / r
		velocity_times.append(velocity_sum)
		velocity_sum = 0.0
		position_sum = position_sum / r
		position_times.append(position_sum)
		position_sum = 0.0
		loop_sum = loop_sum / r
		loop_times.append(loop_sum)
		loop_sum = 0.0
	i += 1
#print(step_times)
#print(loop_times)
csv.close()
x = np.array([i + 1 for i in range(n)])

#Plot 1

pl.figure(figsize=(8,6), dpi=80)
fig, ax1 = pl.subplots()

min_loop_time = 0
max_loop_time = 0

for i in range(n):
	if loop_times[i] > loop_times[max_loop_time]:
		max_loop_time = i
	if loop_times[i] < loop_times[min_loop_time]:
		min_loop_time = i
ax1.bar(x, np.array(step_times), facecolor="r", edgecolor="r", label = "Step Time")
ax1.set_ylabel("Time in ms", color="r")
ax2 = ax1.twinx()
ax2.plot(x, np.array(loop_times), color="b", linewidth=1.0, linestyle="-", label = "Loop Time")
ax2.set_ylabel("Time in ms", color = "b")
ax1.legend(loc='upper left')
ax2.legend(loc='upper right')
pl.xlabel("Number of Iterations")
pl.annotate("Max Loop Time: "+str('%.2f'%(loop_times[max_loop_time])), xy=(max_loop_time + 1, loop_times[max_loop_time]))
pl.annotate("Min Loop Time: "+str('%.2f'%(loop_times[min_loop_time])), xy=(min_loop_time + 1, loop_times[min_loop_time]))
pl.title("Step & Loop Times")
pl.savefig("plots/g19_lab09_plot01.png")
pl.clf()

#Plot 2

pl.figure(figsize=(8,6), dpi=80)
pl.subplot(1,1,1)
pl.plot(x, np.array(step_times), color="red", linewidth=1.0, linestyle="-", label = "Step Time")
pl.plot(x, np.array(collision_times), color="blue", linewidth=1.0, linestyle="-", label = "Collision Time")
pl.plot(x, np.array(velocity_times), color="green", linewidth=1.0, linestyle="-", label = "Velocity Time")
pl.plot(x, np.array(position_times), color="maroon", linewidth=1.0, linestyle="-", label = "Position Time")
pl.plot(x, np.array([collision_times[i] + velocity_times[i] + position_times[i] for i in range(n)]), color="purple", linewidth=1.0, linestyle="-", label = "Sum of Times")
pl.legend(loc='upper right')
pl.xlabel("Number of Iterations")
pl.ylabel("Time in ms")
pl.title("Various Times")
pl.savefig("plots/g19_lab09_plot02.png")
pl.clf()

#Plot 3

pl.figure(figsize=(8,6), dpi=80)
pl.subplot(1,1,1)
#pl.plot(x, np.array(step_times), color="red", linewidth=1.0, linestyle="-", label = "Step Time")
pl.errorbar(x, np.array(step_times), yerr = [err_low_step, err_high_step], label = "Step Time")
pl.legend(loc='upper right')
pl.xlim(0, n+1)
pl.xlabel("Number of Iterations")
pl.ylabel("Time in ms")
pl.title("Step Times with error bars")
pl.savefig("plots/g19_lab09_plot03.png")
pl.clf()

#Plot 4

pl.figure(figsize=(8,6), dpi=80)
pl.subplot(1,1,1)
#pl.plot(x, np.array(step_times), color="red", linewidth=1.0, linestyle="-", label = "Step Time")
histogram = pl.hist(roll_no_step_times, edgecolor='k', facecolor='b', label = "Step Time Frequency")
cumm_line_x = []
cumm_line_y = np.cumsum(histogram[0])
for i in range(10):
	cumm_line_x.append((histogram[1][i]+histogram[1][i+1])/2)
pl.plot(cumm_line_x, cumm_line_y, color="red", label = "Cummulative Frequency", linestyle="-")
pl.legend(loc='upper left')
pl.xlabel("Time in ms")
pl.ylabel("Frequency")
pl.title("Step Times Frequency for "+str(roll_no)+" iterations")
pl.savefig("plots/g19_lab09_plot04.png")
pl.clf()

#Plot 5

csv = open("data/g19_lab09data_random.csv")
step_sum_random = 0.0
step_times_random = []
i = 1
for line in csv:
	values = line.split(",")
	step_sum_random += float(values[2])
	if i%15 == 0:
		step_sum_random = step_sum_random / 15
		step_times_random.append(step_sum_random)
		step_sum_random = 0.0
	i += 1	
step_line = pl.poly1d(pl.polyfit(x, np.array(step_times), 1))
step_line_random = pl.poly1d(pl.polyfit(x, np.array(step_times_random), 1))
pl.plot(x, np.array(step_times), "b.", x, step_line(x), "b-", label = "Average Step Times")
pl.plot(x, np.array(step_times_random), "r.", x, step_line_random(x), "r-", label = "Average Random Step Times")
pl.xlim(0, n+1)
pl.legend(loc="upper right")
pl.xlabel("Number of Iterations")
pl.ylabel("Time in ms")
pl.title("Regression on Step Times")
pl.savefig("plots/g19_lab09_plot05.png")
pl.clf()
