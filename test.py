import os

fp = open("/Users/devpark/Desktop/abc.png", 'rb')
fp2 = open("tempContent1", 'wb')
for line in fp.readlines():
	fp2.write(line)
fp.close()
fp2.close()