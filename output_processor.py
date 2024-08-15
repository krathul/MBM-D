out_data = {}

with open("log/static") as log:
    for line in log:
        col, value = line.strip("\n").split(":")
        col = col.strip(" ")
        value = float(value.strip("ms"))
        if col in out_data.keys():
            out_data[col].append(value)
        else:
            out_data[col] = [value]

for i in out_data.keys():
    print(i)
    for j in out_data[i]:
        print(j, end="\t")
    print("")

print("\n")

out_data = {}
with open("log/dynamic") as log:
    for line in log:
        col, value = line.strip("\n").split(":")
        col = col.strip(" ")
        value = float(value.strip("ms"))
        if col in out_data.keys():
            out_data[col].append(value)
        else:
            out_data[col] = [value]

for i in out_data.keys():
    if i == 'static':
        continue
    print(i)
    for j in out_data[i]:
        print(j, end="\t")
    print("")
