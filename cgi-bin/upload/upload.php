<?php

$uploadDir = getenv("UPLOAD_PATH");

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_FILES['file_php']) && is_array($_FILES['file_php']['name'])) {
        foreach ($_FILES['file_php']['name'] as $key => $name) {
            if ($_FILES['file_php']['error'][$key] == UPLOAD_ERR_OK) {
                $tmp_name = $_FILES['file_php']['tmp_name'][$key];
                $destination = '../../uploads/' . basename($name);

                // Move the uploaded file to the desired directory
                if (move_uploaded_file($tmp_name, $destination)) {
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