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
    exit(1)

query_string = os.environ.get("QUERY_STRING", "")
print("query_string", query_string, file=sys.stderr)
if query_string == "":
    make_error(400)
query_params = urllib.parse.parse_qs(query_string)

print("hihihihihihihihihihihih", file=sys.stderr)
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
    values = query_params.get("file", [])
    if not values:
        make_error(400)
    for value in values:
        filename = os.path.basename(value)
        filepath = os.path.join(upload_dir, filename)
        if not os.path.exists(filepath):
            make_error(404)
        q.put(filepath)

def main():
    print("here", file=sys.stderr)
    checkFileExist()
    delete_file()
    print(f"status: 302\r")
    print(f"location: /delete_success.html\r")
    # Print the HTTP headers
    

if __name__ == "__main__":
    main()