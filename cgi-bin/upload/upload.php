<?php
echo "Content-Type: text/html\r\n";
echo "Status: 200\r\n";
echo "<html>\r\n";
echo "<head>\r\n";
echo "<title>Form Response</title>\r\n";
echo "<meta charset=\"utf-8\">\r\n";
echo "</head>\r\n";
echo "<body>\r\n";

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
print_r($_FILES);

$uploadDir = getenv("UPLOAD_PATH");

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_FILES['file_php']) && is_array($_FILES['file_php']['name'])) {
        foreach ($_FILES['file_php']['name'] as $key => $name) {
            if ($_FILES['file_php']['error'][$key] == UPLOAD_ERR_OK) {
                $tmp_name = $_FILES['file_php']['tmp_name'][$key];
                $destination = '../../uploads/' . basename($name);

                // Move the uploaded file to the desired directory
                if (move_uploaded_file($tmp_name, $destination)) {
                    echo "File $name uploaded successfully.<br>";
                } else {
                    echo "Failed to upload file $name.<br>";
                }
            } else {
                echo "Error uploading file $name.<br>";
            }
        }
    } else {
        echo "No files were uploaded.";
    }
}
echo "</body>\r\n";
?>