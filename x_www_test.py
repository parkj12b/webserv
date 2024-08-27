import cgi
import os

# 파일이나 스트림의 내용을 읽어오는 예시
file = open("/Users/devpark/Documents/mini/tempContent1", 'r')

fd = file.fileno()
file_stat = os.fstat(fd)
file_size = file_stat.st_size
# FieldStorage 생성 시 fp 인수로 데이터를 전달
form = cgi.FieldStorage(fp=file, environ={
    'REQUEST_METHOD': 'POST',
    'CONTENT_TYPE': 'application/x-www-form-urlencoded',
    'CONTENT_LENGTH': file_size,
})
file.close()

# 데이터 접근
username = form.getvalue('username')
password = form.getvalue('password')

print(f"Username: {username}")
print(f"Password: {password}")
