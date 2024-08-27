#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb
import os
import json

# Enable LOGging
cgitb.enable()

# HTML 응답 생성

def print_result(name, age):
	print("content-type: text/html\r\n", end="")
	print("status: 200\r\n", end="")
	print("<html>\r\n", end="")
	print("<head>\r\n", end="")
	print("<title>Form Response</title>\r\n", end="")
	print("</head>\r\n", end="")
	print("<body>\r\n", end="")
	print("<h1>Form Submitted</h1>\r\n", end="")
	print(f"<p>Name: {name}</p>\r\n", end="")
	print(f"<p>Age: {age}</p>\r\n", end="")
	print("</body>\r\n", end="")
	print("</html>\r\n", end="")

def process_x_www_form_urlencoded_content(file, env):
	form = cgi.FieldStorage(fp=file, environ=env)
	name = form.getvalue('name')
	age = form.getvalue('age')
	print_result(name, age)

def process_json_content(file):
	try:
		# 요청 본문을 읽기
		input_data = ""
		for line in file.readlines():
			input_data += line
		json_data = json.loads(input_data)

		name = json_data.get('name')
		age = json_data.get('age')

		print_result(name, age)

	except (ValueError, json.JSONDecodeError):
		print("status: 400\r\n", end="")
		exit(1)

def main():
	content_type = os.environ.get("CONTENT_TYPE")
	if content_type != "application/x-www-form-urlencoded" and content_type != "application/json":
		print("status: 400\r\n", end="")
		exit(1)
	request_method = os.environ.get("REQUEST_METHOD")
	content_length = os.environ.get("CONTENT_LENGTH")
	environ = {
		'REQUEST_METHOD': request_method,
		'CONTENT_TYPE': content_type,
		'CONTENT_LENGTH': content_length,
	}
	content_filename = os.environ.get("CONTENT_FILENAME")
	if content_filename is None or os.path.isfile(content_filename) is False:
		print("status: 400\r\n", end="")
		exit(1)
	f = open(content_filename, 'r')
	if content_type == "application/x-www-form-urlencoded":
		process_x_www_form_urlencoded_content(f, environ)
	else:
		process_json_content(f)
	f.close()

if __name__ == "__main__":
	main()