<?php
// HTML 응답 생성 함수
function print_result($name, $age) {
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
function process_json_content($input_data) {
    try {
        $json_data = json_decode($input_data, true);
        if (json_last_error() !== JSON_ERROR_NONE) {
            echo "status: 400\r\n";
            exit(1);
        }

        $name = isset($json_data['name']) ? $json_data['name'] : null;
        $age = isset($json_data['age']) ? $json_data['age'] : null;
        print_result($name, $age);

    } catch (Exception $e) {
        echo "status: 400\r\n";
        exit(1);
    }
}

// application/x-www-form-urlencoded 콘텐츠 처리 함수
function process_x_www_form_urlencoded_content() {
    $name = isset($_POST['name']) ? $_POST['name'] : null;
    $age = isset($_POST['age']) ? $_POST['age'] : null;
    print_result($name, $age);
}

function main() {
    // 콘텐츠 타입 가져오기
    $content_type = isset($_SERVER["CONTENT_TYPE"]) ? $_SERVER["CONTENT_TYPE"] : null;

    echo "Content-Type: text/html\r\n";

    if ($content_type != "application/x-www-form-urlencoded" && $content_type != "application/json") {
        echo "status: 400\r\n";
        exit(1);
    }

    // 요청 메서드와 콘텐츠 길이 가져오기
    $request_method = $_SERVER["REQUEST_METHOD"];
    $content_length = isset($_SERVER["CONTENT_LENGTH"]) ? $_SERVER["CONTENT_LENGTH"] : null;

    // 콘텐츠 파일명 가져오기
    $content_filename = getenv("CONTENT_FILENAME");
    if ($content_filename === false || !is_file($content_filename)) {
        echo "status: 400\r\n";
        exit(1);
    }

    // 파일 내용을 읽어오기
    $file_content = file_get_contents($content_filename);
    if ($file_content === false) {
        echo "status: 400\r\n";
        exit(1);
    }

    // 콘텐츠 타입에 따른 처리
    if ($content_type == "application/x-www-form-urlencoded") {
        parse_str($file_content, $_POST);
        process_x_www_form_urlencoded_content();
    } else {
        $input_data = $file_content;
        process_json_content($input_data);
    }
}

main();
?>
