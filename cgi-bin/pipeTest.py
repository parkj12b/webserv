def generate_64kb_string():
    # 64KB에 해당하는 바이트 수
    size_in_bytes = 64 * 1024 * 13

    # 문자열 생성
    # 여기서는 'A' 문자를 반복해서 사용하여 64KB 크기의 문자열을 만듭니다
    test_string = 'A' * size_in_bytes

    return test_string

# 생성된 문자열 길이 확인
test_string = generate_64kb_string()
print(test_string)
print(f"Length of generated string: {len(test_string)} bytes")
