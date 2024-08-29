<?php

// $content_filename = getenv("CONTENT_FILENAME");
// if ($content_filename === false || !is_file($content_filename)) {
//     echo "status: 400\r\n";
//     exit(1);
// }

// // 파일 내용을 읽어오기
// $file_content = file_get_contents($content_filename);
// if ($file_content === false) {
//     echo "status: 400\r\n";
//     exit(1);
// }

// $content_filename = getenv("CONTENT_FILENAME");
// $file_content = file_get_contents($content_filename);

// print_r($_POST);
// print_r($_SERVER);
// print_r($_FILES);
// ini_set('output_buffering', 'off');
// ini_set('zlib.output_compression', '0');
// ini_set('implicit_flush', '1');

// header_remove();  // 모든 헤더 제거
$uploadDir = getenv("UPLOAD_PATH");

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_FILES['file_php']) && is_array($_FILES['file_php']['name'])) {
        foreach ($_FILES['file_php']['name'] as $key => $name) {
            if ($_FILES['file_php']['error'][$key] == UPLOAD_ERR_OK) {
                $tmp_name = $_FILES['file_php']['tmp_name'][$key];
                $destination = '../../uploads/' . basename($name);

                // Move the uploaded file to the desired directory
                if (move_uploaded_file($tmp_name, $destination)) {
                    ob_start();
                    echo "status: 302\r\n";
                    echo "location: /upload_success.html\r\n";
                } else {
                    echo "status: 400\r\n";
                }
            } else {
                echo "status: 400\r\n";
            }
        }
    } else {
        echo "status: 400\r\n";
        // exit ;
    }
}
?>