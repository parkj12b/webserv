#!/usr/bin/env php
<?php
// Get the content type from the environment
$content_type = getenv('CONTENT_TYPE');

// Function to handle 'application/x-www-form-urlencoded'
function handle_urlencoded() {
    echo "<html><body>";
    echo "<h2>Received Form Data (application/x-www-form-urlencoded)</h2>";
    foreach ($_POST as $key => $value) {
        echo "<p>$key: " . htmlspecialchars($value, ENT_QUOTES, 'UTF-8') . "</p>";
    }
    echo "</body></html>";
}

// Function to handle 'application/json'
function handle_json() {
    // Get the raw POST data
    $json_data = file_get_contents('php://input');
    // Decode the JSON data
    $data = json_decode($json_data, true);

    if (json_last_error() === JSON_ERROR_NONE) {
        echo "<html><body>";
        echo "<h2>Received JSON Data (application/json)</h2>";
        echo "<pre>";
        echo htmlspecialchars(json_encode($data, JSON_PRETTY_PRINT), ENT_QUOTES, 'UTF-8');
        echo "</pre>";
        echo "</body></html>";
    } else {
        echo "<html><body>";
        echo "<h2>Error: Invalid JSON Data</h2>";
        echo "</body></html>";
        exit(1); // Exit with status code 1 for JSON errors
    }
}

// Main logic based on content type
if ($content_type === 'application/x-www-form-urlencoded') {
    handle_urlencoded();
} elseif ($content_type === 'application/json') {
    handle_json();
} else {
    exit(1);
}
?>
