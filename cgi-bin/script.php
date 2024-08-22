#!/usr/bin/env php
<?php
// 쿼리 문자열 파라미터를 가져오기
$query_params = $_GET;

// HTML 페이지 출력
echo "<!DOCTYPE html>\n";
echo "<html lang='en'>\n";
echo "<head>\n";
echo "    <meta charset='UTF-8'>\n";
echo "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
echo "    <title>CGI Query String Example</title>\n";
echo "</head>\n";
echo "<body>\n";
echo "    <h1>CGI Query String Example</h1>\n";
echo "    <p>Received query parameters:</p>\n";
echo "    <ul>\n";

// 쿼리 파라미터를 HTML 리스트로 변환
foreach ($query_params as $key => $value) {
    // HTML 특수문자 이스케이프 처리
    $key = htmlspecialchars($key, ENT_QUOTES, 'UTF-8');
    $value = htmlspecialchars($value, ENT_QUOTES, 'UTF-8');
    echo "        <li>$key: $value</li>\n";
}

echo "    </ul>\n";
echo "</body>\n";
echo "</html>\n";
?>
