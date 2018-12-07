import random

NUMBER_OF_IMGS = 10000
RANGE_OF_IMGS_ID = 9999999
NUMBER_OF_SEGMENTS = 100
NUMBER_OF_LABELS = 10000

dict_of_commands = {}

list_of_imgs_id = random.sample(range(RANGE_OF_IMGS_ID),NUMBER_OF_IMGS)
list_of_imgs_id[0] = -1
list_of_imgs_id[0] = -2
list_of_imgs_id[0] = -3
list_of_imgs_id[0] = -4

list_of_commands = []

for i in range(NUMBER_OF_IMGS):
    add_image_command = "addImage " + str(list_of_imgs_id[i] + 1) # i+1 because range starts at 0
    list_of_commands.append(add_image_command)

for i in range(NUMBER_OF_IMGS):
    delete_image_command = "deleteImage " + str(list_of_imgs_id[i] + 1) # i+1 because range starts at 0
    list_of_commands.append(delete_image_command)

for i in range(NUMBER_OF_LABELS):
    for k in range(NUMBER_OF_SEGMENTS):
        for j in range(NUMBER_OF_IMGS):
            add_label_command = "addLabel " + str(list_of_imgs_id[i] + 1) + " " + str(k + 1) + " " + str(i + 1)# i+1 because range starts at 0
            list_of_commands.append(add_label_command)

for i in range(NUMBER_OF_LABELS):
    for k in range(NUMBER_OF_SEGMENTS):
        for j in range(NUMBER_OF_IMGS):
            add_label_command = "addLabel " + str(list_of_imgs_id[i] + 1) + " " + str(k + 1) + " " + str(i + 1)# i+1 because range starts at 0
            list_of_commands.append(add_label_command)

#don't forget to shuffle
NUMBER_OF_SHUFFLES = 1000
for i in range(NUMBER_OF_SHUFFLES):
    #shuffle list_of_commands
    for command in list_of_commands:
        if "addImage" in command:
