#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb

# 디버깅을 위한 설정
cgitb.enable()

def generate_html(query_params):
    """ 쿼리 파라미터에 기반한 HTML 페이지 생성 """
    # HTML 헤더 출력
    print("<html>")
    print("<head>")
    print("<title>CGI Query String Example</title>")
    print("</head>")
    print("<body>")
    print("<h1>CGI Query String Example</h1>")
    print("<p>Received query parameters:</p>")
    print("<ul>")

    # 쿼리 파라미터를 HTML 리스트로 변환
    for key, value in query_params.items():
        print(f"\t<li>{key}: {value}</li>")
    print("</ul>")
    print("</body>")
    print("</html>")

def main():
    # CGI 환경에서 쿼리 문자열을 가져오기
    form = cgi.FieldStorage()

    # 쿼리 문자열을 딕셔너리 형태로 저장
    query_params = {key: form.getvalue(key) for key in form.keys()}

    # HTML 페이지 출력
    generate_html(query_params)

if __name__ == "__main__":
    main()
