<?php
// 디버깅을 위한 설정
// error_reporting(E_ALL);
ini_set('display_errors', 1);

function generate_html($query_params, $headers) {
    // HTML 헤더 출력
    echo "content-type: text/html\r\n";
	echo "status: 200\r\n";
    echo "<html>\r\n";
    echo "<head>\r\n";
    echo "<title>CGI Query String Example</title>\r\n";
    echo "</head>\r\n";
    echo "<body>\r\n";
    echo "<h1>CGI Query String Example</h1>\r\n";

    // 쿼리 파라미터 출력
    echo "<h2>Received query parameters:</h2>\r\n";
    echo "<ul>\r\n";
    echo "\t<li>name: " . ($query_params['name'] ?? 'default_name') . "</li>\r\n";
    echo "\t<li>age: " . ($query_params['age'] ?? 'default_age') . "</li>\r\n";
    // foreach ($query_params as $key => $value) {
    //     echo "\t<li>$key: $value</li>\r\n";
    // }
    echo "</ul>\r\n";
    echo "</body>\r\n";
    echo "</html>\r\n";
}

function main() {
    // CGI 환경에서 쿼리 문자열을 가져오기
    $query_params = [];
    if (isset($_SERVER['QUERY_STRING']))
    {
        parse_str($_SERVER['QUERY_STRING'], $query_params);
    }
    // 헤더를 배열 형태로 저장
    $headers = [];
    foreach ($_SERVER as $key => $value) {
        if (strpos($key, 'HTTP_') === 0) {
            $headers[$key] = $value;
        }
    }

    // HTML 페이지 출력
    generate_html($query_params, $headers);
}

main();
?>
