#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb
import os

# 디버깅을 위한 설정
cgitb.enable()

def generate_html(query_params, headers):
	""" 쿼리 파라미터와 헤더에 기반한 HTML 페이지 생성 """
	# HTML 헤더 출력
	print("content-type: text/html\r\n", end="")
	print("status: 200\r\n", end="")
	print("<html>\r\n", end="")
	print("<head>\r\n", end="")
	print("<title>CGI Query String Example</title>\r\n", end="")
	print("</head>\r\n", end="")
	print("<body>\r\n", end="")
	print("<h1>CGI Query String Example</h1>\r\n", end="")

    # 쿼리 파라미터 출력
	print("<h2>Received query parameters:</h2>\r\n", end="")
	print("<ul>\r\n", end="")
	print(f"\t<li>name: {query_params.get('name') or ''}</li>\r\n", end="")
	print(f"\t<li>age: {query_params.get('age') or ''}</li>\r\n", end="")
	print("</ul>\r\n", end="")
	print("</body>\r\n", end="")
	print("</html>\r\n", end="")

def main():
    # CGI 환경에서 쿼리 문자열을 가져오기
	while True:
		continue
	form = cgi.FieldStorage()

    # 쿼리 문자열을 딕셔너리 형태로 저장
	query_params = {key: form.getvalue(key) for key in form.keys()}

    # 헤더를 딕셔너리 형태로 저장
	headers = {key: value for key, value in os.environ.items() if key.startswith("HTTP_")}

    # HTML 페이지 출력
	generate_html(query_params, headers)

if __name__ == "__main__":
	main()
