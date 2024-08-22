#!/usr/bin/env python3

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi
import cgitb
import sys
import json

# Enable debugging
cgitb.enable()

# Read the HTTP content type
content_type = os.environ.get('CONTENT_TYPE')

# Function to handle 'application/x-www-form-urlencoded'
def handle_urlencoded():
    form = cgi.FieldStorage()
    print("<html><body>")
    print("<h2>Received Form Data (application/x-www-form-urlencoded)</h2>")
    for key in form.keys():
        print(f"<p>{key}: {form.getvalue(key)}</p>")
    print("</body></html>")

# Function to handle 'application/json'
def handle_json():
    try:
        # Get the content length
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        # Read the JSON data from stdin
        json_data = sys.stdin.read(content_length)
        # Parse the JSON data
        data = json.loads(json_data)
        print("<html><body>")
        print("<h2>Received JSON Data (application/json)</h2>")
        print("<pre>")
        print(json.dumps(data, indent=4))
        print("</pre>")
        print("</body></html>")
    except json.JSONDecodeError:
        print("<html><body>")
        print("<h2>Error: Invalid JSON Data</h2>")
        print("</body></html>")

# Main logic based on content type
if content_type == 'application/x-www-form-urlencoded':
    handle_urlencoded()
elif content_type == 'application/json':
    handle_json()
else
	sys.exit(1)