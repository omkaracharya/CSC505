import os,sys

k_val = [1,3,10,30,100]
for i in range(0,len(k_val)):
	print("For k=",i,":")
	for j in range(0,5):
		os.system("./qsort "+str(k_val[i])+" < input_s1000000.txt > /dev/null")
	print('\n')