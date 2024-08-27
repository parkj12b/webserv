<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Check if a file has been uploaded
    print_r($_POST);
    print_r($_FILES);
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