<?php
// 오류 보고 설정
error_reporting(E_ALL);
ini_set('display_errors', 1);

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

function process_x_www_form_urlencoded_content() {
    $name = isset($_POST['name']) ? $_POST['name'] : '';
    $age = isset($_POST['age']) ? $_POST['age'] : '';
    print_result($name, $age);
}

function process_json_content() {
    $input_data = file_get_contents('php://input');
    $json_data = json_decode($input_data, true);

    if (json_last_error() === JSON_ERROR_NONE) {
        $name = isset($json_data['name']) ? $json_data['name'] : '';
        $age = isset($json_data['age']) ? $json_data['age'] : '';
        print_result($name, $age);
    } else {
        echo "status: 400\r\n";
        exit;
    }
}

function main() {
    $content_type = isset($_SERVER["CONTENT_TYPE"]) ? $_SERVER["CONTENT_TYPE"] : '';

    if ($content_type != "application/x-www-form-urlencoded" && $content_type != "application/json") {
        echo "status: 400\r\n";
        exit;
    }

    if ($_SERVER["REQUEST_METHOD"] === "POST") {
        if ($content_type === "application/x-www-form-urlencoded") {
            process_x_www_form_urlencoded_content();
        } else {
            process_json_content();
        }
    } else {
        echo "status: 400\r\n";
        exit;
    }
}

main();
?>
