#!/usr/bin/env python3
import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import json
import os

file = open("/Users/devpark/Documents/mini/tempContent1", "r")
input_data = ""

for line in file.readlines():
	input_data += line
content_length = len(input_data)
file.close()
json_data = json.loads(input_data)
name = json_data.get('name')
age = json_data.get('age')
print(name, age)