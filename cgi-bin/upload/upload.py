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
        if isinstance(files, list):
            # Handle multiple files
            for file_item in files:
                if file_item.filename:
                    # Clean the filename to prevent security issues
                    filename = os.path.basename(file_item.filename)
                    filepath = os.path.join(upload_dir, filename)

                    # Save the uploaded file
                    with open(filepath, 'wb') as output_file:
                        output_file.write(file_item.file.read())

                    print(f"<p>File '{filename}' uploaded successfully!</p>")
        else:
            # Handle single file
            file_item = files
            if file_item.filename:
                filename = os.path.basename(file_item.filename)
                filepath = os.path.join(upload_dir, filename)

                # Save the uploaded file
                with open(filepath, 'wb') as output_file:
                    output_file.write(file_item.file.read())

                print(f"<p>File '{filename}' uploaded successfully!</p>")
    else:
        print("<h2>No file field in the form</h2>")

# Handle file uploads
handle_file_uploads()

# Optionally, you can output the rest of the form data
print("<h3>Form data:</h3>")
for key in form.keys():
    item = form.getvalue(key)
    if isinstance(item, list):
        for sub_item in item:
            print(f"<p><strong>{key}:</strong> {sub_item}</p>")
    else:
        print(f"<p><strong>{key}:</strong> {item}</p>")
