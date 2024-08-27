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

$content_filename = getenv("CONTENT_FILENAME");
$file_content = file_get_contents($content_filename);


$_FILES['files'] = [
    'name' => getenv("CONTENT_FILENAME"),
    'type' => getenv("CONTENT_TYPE"),
    'tmp_name' => $file_content,
    'error' => UPLOAD_ERR_OK,
    'size' => getenv("CONTENT_LENGTH"),
];

// print_r($_POST);
// print_r($_SERVER);
print_r($_FILES);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {

    // Check if a file has been uploaded
    if (isset($_FILES['files'])) {
        // Loop through the uploaded files
        $fileCount = count($_FILES['files']['name']);
        
        for ($i = 0; $i < $fileCount; $i++) {
            // Temporary file path
            $tmpFile = $_FILES['files']['tmp_name'][$i];
            // Original file name
            $fileName = $_FILES['files']['name'][$i];
            // Path where the file will be saved
            $uploadFile = $uploadDir . basename($fileName);

            // Check if the file was uploaded without errors
            if ($_FILES['files']['error'][$i] === UPLOAD_ERR_OK) {
                // Move the file from temporary location to the destination
                if (move_uploaded_file($tmpFile, $uploadFile)) {
                    echo "File successfully uploaded: $fileName<br>";
                } else {
                    echo "Failed to move uploaded file: $fileName<br>";
                }
            } else {
                echo "Error uploading file: $fileName<br>";
            }
        }
    } else {
        echo "No files were uploaded.";
    }
} else {
    echo "Invalid request method.";
}
?>