#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb
import os

# 에러 로깅 활성화
cgitb.enable()

# 폼 데이터 가져오기
form = cgi.FieldStorage()
name = form.getvalue('name')
age = form.getvalue('age')

# HTML 응답 생성
print("Content-type: text/html\r\n", end="")
print("<html>")
print("<head>")
print("<title>Form Response</title>")
print("</head>")
print("<body>")
print("<h1>Form Submitted</h1>")
if name and age:
    print(f"<p>Name: {name}</p>")
    print(f"<p>Age: {age}</p>")
else:
    exit(1)
print("</body>")
print("</html>")
