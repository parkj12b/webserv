<?php
echo "Content-Type: text/html\r\n";
echo "Status: 200\r\n";


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

print_r($_POST);
print_r($_SERVER);
print_r($_FILES);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Check if a file has been uploaded
    if (isset($_FILES['file'])) {
        $file = $_FILES['file'];

        // Check for upload errors
        if ($file['error'] === UPLOAD_ERR_OK) {
            // Move the file to the desired directory
            $uploadDir = './uploads';
            $uploadFile = $uploadDir . basename($file['name']);

            if (move_uploaded_file($file['tmp_name'], $uploadFile)) {
                echo "File successfully uploaded.";
            } else {
                echo "Failed to move uploaded file.";
            }
        } else {
            echo "File upload error: " . $file['error'];
        }
    } else {
        echo "No file uploaded.";
    }
} else {
    echo "Invalid request method.";
}
?>