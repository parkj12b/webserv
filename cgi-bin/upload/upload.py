#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb
import os

# Enable CGI error reporting
cgitb.enable()

# Create an instance of FieldStorage to parse the form data
form = cgi.FieldStorage()

# Define the directory where uploaded files will be stored
upload_dir = './'

# Function to handle file uploads
def handle_file_uploads():
    if 'file' in form:
        files = form['file']
        # Check if files is a list (multiple files)
        if isinstance(files, list):
            for file_item in files:
                process_file(file_item)
        else:
            # Single file upload
            process_file(files)
    else:
        print("<h2>No file field in the form</h2>")

# Function to process an individual file
def process_file(file_item):
    if file_item.filename:
        filename = os.path.basename(file_item.filename)

        # Check if the file has a .py or .php extension
        if filename.endswith('.py') or filename.endswith('.php'):
            print(f"<p>File '{filename}' is not allowed and was skipped.</p>")
            return

        # Define the path where the file will be saved
        filepath = os.path.join(upload_dir, filename)

        # Save the uploaded file
        with open(filepath, 'wb') as output_file:
            output_file.write(file_item.file.read())

        print(f"<p>File '{filename}' uploaded successfully!</p>")

# Handle file uploads
handle_file_uploads()
