#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb
import os
import sys

# Enable CGI error reporting
cgitb.enable()

# Create an instance of FieldStorage to parse the form data
# 여기에서 file 이름 가져와서 fp=넣으면 될듯

#__init__(self, fp=None, headers=None, outerboundary='', environ={'LESS': '-X -i -Psline %l/%L$ -Pmline %l/%L (%p)$', 'DPKG_ARC..., keep_blank_values=0, strict_parsing=0)
#fd = 

# filename = os.environ['CONTENT_FILENAME']
# filename = "/Users/devpark/Documents/mini/tempContent1"
# file = open(filename, 'r')
# form = cgi.FieldStorage(fp=file, environ=os.environ)
form = cgi.FieldStorage(environ=os.environ)

# Define the directory where uploaded files will be stored
upload_dir = './uploads'

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

def main():
    print("Content-Type: text/html\r\n") 
    print("status: 200\r\n")
    print("<html>")
    print("<head>")
    print("<title>File Upload</title>")
    print("<meta charset=\"utf-8\">")
    print("</head>")
    print("</html>")
    
    handle_file_uploads()
    

if __name__ == "__main__":
    main()

