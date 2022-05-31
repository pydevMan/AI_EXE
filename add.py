import os

msg = input("msg: ")

os.system("git add .")
os.system(f"git commit -m \"{msg}\"")
os.system(f"git push -u main main2")