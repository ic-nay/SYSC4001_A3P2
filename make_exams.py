from random import randint

for i in range(1, 20):
    with open(f"exam_{i}.txt", mode="w") as file:
        file.write(f"{randint(1000, 9998)}")

with open("exam_20.txt", mode="w") as file:
    file.write("9999")