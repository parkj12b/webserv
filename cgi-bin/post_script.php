<?php
// HTML 응답 생성 함수
function print_result($name, $age) {
    echo "content-type: text/html\r\n";
    echo "status: 200\r\n";
    echo "<html>\r\n";
    echo "<head>\r\n";
    echo "<title>Form Response</title>\r\n";
    echo "</head>\r\n";
    echo "<body>\r\n";
    echo "<h1>Form Submitted</h1>\r\n";
    echo "<p>Name: " . htmlspecialchars($name) . "</p>\r\n";
    echo "<p>Age: " . htmlspecialchars($age) . "</p>\r\n";
    echo "</body>\r\n";
    echo "</html>\r\n";
}

// JSON 콘텐츠 처리 함수
function process_json_content($data) {
    // JSON 데이터를 배열로 변환
    $json_data = json_decode($data, true);

    // JSON 디코딩이 정상적으로 이루어졌는지 확인
    if (json_last_error() === JSON_ERROR_NONE) {
        // name과 age 변수 설정
        $name = isset($json_data['name']) ? $json_data['name'] : null;
        $age = isset($json_data['age']) ? $json_data['age'] : null;

        // 결과 출력 함수 호출
        print_result($name, $age);
    } else {
        // 오류 상태 코드 반환
		echo "status: 400\r\n";
        exit(1);
    }
}

// application/x-www-form-urlencoded 콘텐츠 처리 함수
function process_x_www_form_urlencoded_content() {
    // name과 age 변수 설정
    $name = isset($_POST['name']) ? $_POST['name'] : null;
    $age = isset($_POST['age']) ? $_POST['age'] : null;
    // 결과 출력 함수 호출
    print_result($name, $age);
}

function main() {
    // 환경 변수 가져오기
    $content_type = isset($_SERVER['CONTENT_TYPE']) ? $_SERVER['CONTENT_TYPE'] : '';
    $request_method = isset($_SERVER['REQUEST_METHOD']) ? $_SERVER['REQUEST_METHOD'] : '';
    $content_length = isset($_SERVER['CONTENT_LENGTH']) ? $_SERVER['CONTENT_LENGTH'] : 0;
	
    // 지원하지 않는 Content-Type 처리
    if ($content_type != "application/x-www-form-urlencoded" && $content_type != "application/json") {
        echo "status: 400\r\n"; // 상태 코드 400 반환
        exit(1);
    }
	
    // 요청 데이터 읽기
    $content_data = file_get_contents('php://stdin');
    
    // Content-Type에 따라 적절한 함수 호출
    if ($content_type == "application/x-www-form-urlencoded") {
        process_x_www_form_urlencoded_content();
    } else if ($content_type == "application/json") {
        process_json_content($content_data);
    }
}

// 아래의 main() 함수 호출
main();
?>
