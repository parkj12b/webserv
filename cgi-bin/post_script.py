#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb
import os

# 에러 로깅 활성화
cgitb.enable()

# HTML 응답 생성

def print_result(name, age):
	print("content-type: text/html\r\n", end="")
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

def process_x_www_form_urlencoded_content():
	form = cgi.FieldStorage()
	name = form.getvalue('name')
	age = form.getvalue('age')
	print_result(name, age)

def process_json_content(f):
	try:
        # 요청 본문을 읽기
		input_data = ""
		for line in f.readlines():
			input_data += line
        json_data = json.loads(input_data)

        name = json_data.get('name')
        age = json_data.get('age')

        print_result(name, age)

    except (ValueError, json.JSONDecodeError):
        exit(1)

def main():
	content_type = os.environ.get("CONTENT_TYPE")
	if content_type != "application/x-www-form-urlencoded" and content_type != "application/json":
		exit(1)
	if content_type == "application/x-www-form-urlencoded":
		process_x_www_form_urlencoded_content()
	else:
		content_filename = os.environ.get("CONTENT_FILENAME")
		if os.path.isfile(content_filename):
			exit(1)
		f = open(content_filename, 'r')
		process_json_content(f)
		f.close()

if __name__ == "__main__":
	main()