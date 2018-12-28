import matplotlib.pyplot as plt
import numpy as np
import xlwt

add_results_lines_avl = []
with open("speedTestAddResultAvl.txt") as f:
    add_results_lines_avl = f.readlines()

delete_results_lines_avl = []
with open("speedTestDeleteResultAvl.txt") as f:
    delete_results_lines_avl = f.readlines()

find_results_lines_avl = []
with open("speedTestFindResultAvl.txt") as f:
    find_results_lines_avl = f.readlines()

add_results_lines_list = []
with open("speedTestAddResultList.txt") as f:
    add_results_lines_list = f.readlines()

delete_results_lines_list = []
with open("speedTestDeleteResultList.txt") as f:
    delete_results_lines_list = f.readlines()

find_results_lines_list = []
with open("speedTestFindResultList.txt") as f:
    find_results_lines_list = f.readlines()

# Add
time_avl_add = []
time_list_add = []
iter_add = []
for index, line in enumerate(add_results_lines_avl):
    line_splitted_avl = line.split(" ")
    line_splitted_list = add_results_lines_list[index].split(" ")
    iter_add.append(int(line_splitted_avl[2]))
    time_avl_add.append(int(line_splitted_avl[3].split('\\')[0]))
    time_list_add.append(int(line_splitted_list[3].split('\\')[0]))

# Delete
time_avl_delete = []
time_list_delete = []
iter_delete = []
for index, line in enumerate(delete_results_lines_avl):
    line_splitted_avl = line.split(" ")
    line_splitted_list = delete_results_lines_list[index].split(" ")
    iter_delete.append(int(line_splitted_avl[2]))
    time_avl_delete.append(int(line_splitted_avl[3].split('\\')[0]))
    time_list_delete.append(int(line_splitted_list[3].split('\\')[0]))

# Find
time_avl_find = []
time_list_find = []
iter_find = []
for index, line in enumerate(find_results_lines_avl):
    line_splitted_avl = line.split(" ")
    line_splitted_list = find_results_lines_list[index].split(" ")
    iter_find.append(int(line_splitted_avl[2]))
    time_avl_find.append(int(line_splitted_avl[3].split('\\')[0]))
    time_list_find.append(int(line_splitted_list[3].split('\\')[0]))


plt.figure(1)
plt.title("Add")
plt.xlabel("Iteration number")
plt.ylabel("Runtime")
plt.plot(np.array(iter_add), np.array(time_avl_add),label='AVL')
plt.plot(np.array(iter_add), np.array(time_list_add),label='LIST')
plt.legend(loc='upper left', shadow=True, fontsize='x-large')

plt.figure(2)
plt.title("Delete")
plt.xlabel("Iteration number")
plt.ylabel("Runtime")
plt.plot(np.array(iter_delete), np.array(time_avl_delete),label='AVL')
plt.plot(np.array(iter_delete), np.array(time_list_delete),label='LIST')
plt.legend(loc='upper left', shadow=True, fontsize='x-large')

plt.figure(3)
plt.title("Find")
plt.xlabel("Iteration number")
plt.ylabel("Runtime")
plt.plot(np.array(iter_find), np.array(time_avl_find),label='AVL')
plt.plot(np.array(iter_find), np.array(time_list_find),label='LIST')
plt.legend(loc='upper left', shadow=True, fontsize='x-large')

#plt.show()


book = xlwt.Workbook()

sheet_add = book.add_sheet("add")
sheet_add.write(0, 0, "AVL")
sheet_add.write(0, 1, "LIST")
sheet_add.write(0, 2, "n")

for row_index, not_relevant in enumerate(range(len(time_avl_add)),1):
    sheet_add.write(row_index, 0, time_avl_add[row_index - 1])
    sheet_add.write(row_index, 1, time_list_add[row_index - 1])
    sheet_add.write(row_index, 2, iter_add[row_index - 1])

sheet_delete = book.add_sheet("delete")
sheet_delete.write(0, 0, "AVL")
sheet_delete.write(0, 1, "LIST")
sheet_delete.write(0, 2, "n")

for row_index, not_relevant in enumerate(range(len(time_avl_delete)),1):
    sheet_delete.write(row_index, 0, time_avl_delete[row_index - 1])
    sheet_delete.write(row_index, 1, time_list_delete[row_index - 1])
    sheet_delete.write(row_index, 2, iter_delete[row_index - 1])

sheet_find = book.add_sheet("find")
sheet_find.write(0, 0, "AVL")
sheet_find.write(0, 1, "LIST")
sheet_find.write(0, 2, "n")

for row_index, not_relevant in enumerate(range(len(time_avl_find)),1):
    sheet_find.write(row_index, 0, time_avl_find[row_index - 1])
    sheet_find.write(row_index, 1, time_list_find[row_index - 1])
    sheet_find.write(row_index, 2, iter_find[row_index - 1])

book.save("wet1_dry_speedtest.xls")