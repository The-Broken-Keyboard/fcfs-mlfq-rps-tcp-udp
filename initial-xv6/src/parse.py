import matplotlib.pyplot as plt
import seaborn as sns
file = open('newfile.txt', 'r')
lines_list = file.readlines()
lst1 = []
lst2 = []
lst3 = []
lst4 = []
lst5 = []
ticks = []
flag = 0

for line in lines_list:
    # if flag == 1:
    #     # Append a value to ticks even when the condition is not met
    #     ticks.append(int(line) - 38)
    #     flag = 0
    if "5 runble" in line or "5 run" in line:
        # Append the value to lst1
        lst1.append(int(line[-2]))
    if "6 runble" in line or "6 run" in line:
        lst2.append(int(line[-2]))
    if "7 runble" in line or "7 run" in line:
        lst3.append(int(line[-2]))
    if "8 runble" in line or "8 run" in line:
        lst4.append(int(line[-2]))
    if "9 runble" in line or "9 run" in line:
        lst5.append(int(line[-2]))
        # flag = 1

ax=sns.lineplot(lst1)
sns.lineplot(lst2)
sns.lineplot(lst3)
sns.lineplot(lst4)
sns.lineplot(lst5)
ax.set(xlabel="ticks",ylabel="queue level")
# plt.show()
plt.gcf().set_size_inches(20,8)
plt.savefig("mlfq_plot.png")