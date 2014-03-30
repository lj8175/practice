import uuid

fh = open("data.txt", "w")
for i in range(100000):
    #print uuid.uuid4()
    fh.write(str(uuid.uuid4())+"\n")
fh.close()
