import cgi
import os
import cgitb
import queue
import sys
import urllib.parse

# Enable CGI error reporting
cgitb.enable()

def make_error(code):
    print(f"status: {code}\r")
    exit(0)

query_string = os.environ.get("QUERY_STRING", "")
if query_string == "":
    make_error(400)
query_params = urllib.parse.parse_qs(query_string)

q = queue.Queue()

upload_dir = os.environ.get("UPLOAD_PATH")

if (upload_dir is None) or (not os.path.isdir(upload_dir)):
    make_error(500)

def delete_file():
    """Delete a file if it exists."""
    while not q.empty():
        file = q.get()
        os.remove(file)

def checkFileExist():
    if not query_params:
        make_error(400)
    values = query_params.get("file")
    if not values:
        make_error(400)
    value = values[0]
    filename = os.path.basename(value)
    filepath = os.path.join(upload_dir, filename)
    if not os.path.exists(filepath):
        print(filepath, file=sys.stderr)
        make_error(404)
    q.put(filepath)

def main():
    checkFileExist()
    delete_file()
    print(f"status: 302\r")
    print(f"location: /delete_success.html\r")
    # Print the HTTP headers
    

if __name__ == "__main__":
    main()