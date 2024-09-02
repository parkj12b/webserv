<?php

$uploadDir = getenv("UPLOAD_PATH");
$uploadedFiles = [];

if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
    if (isset($_FILES['file_php']) && is_array($_FILES['file_php']['name']))
    {
        foreach ($_FILES['file_php']['name'] as $key => $name)
        {
            if ($_FILES['file_php']['error'][$key] == UPLOAD_ERR_OK)
            {
                $tmp_name = $_FILES['file_php']['tmp_name'][$key];
                $destination = $uploadDir.basename($name);
                $fileExtension = pathinfo($name, PATHINFO_EXTENSION);
                if ($fileExtension === 'py' || $fileExtension === 'php')
                {
                    foreach ($uploadedFiles as $file)
                    {
                        unlink($file);
                    }
                    echo "status: 400\r\n";
                    exit;
                }
                // Move the uploaded file to the desired directory
                if (move_uploaded_file($tmp_name, $destination))
                {
                    continue ;
                }
                else
                {
                    foreach ($uploadedFiles as $file)
                    {
                        unlink($file);
                    }
                    echo "status: 400\r\n";
                    exit ;
                }
            }
            else
            {
                foreach ($uploadedFiles as $file)
                {
                    unlink($file);
                }
                echo "status: 400\r\n";
                exit ;
            }
        }
    }
    else
    {
        foreach ($uploadedFiles as $file)
        {
            unlink($file);
        }
        echo "status: 400\r\n";
        exit ;
    }
    echo "Status: 302\r\n";
    echo "location: /upload_success.html\r\n";
}
?>